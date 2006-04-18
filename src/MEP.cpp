/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	MEP.cpp
	Molecular Electrostatic Potential calculations
	Adapted from GAMESS by BMB 10/1997
	bug fixes and MEP surface code moved here by BMB 1/1998
	Removed Mac window dependancies BMB 4/1998
	Fixed MP stack size problem in MEP3D surfaces BMB 9/2000
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BasisSet.h"
#include "RysPolynomial.h"
#include "GaussHermite.h"
#include "SurfaceTypes.h"
#if defined(powerc) && defined(MacintoshBuild)
#include <Multiprocessing.h>
#endif

extern Boolean		gMPAware;
extern long			gNumProcessors;


#define kMaxAngularMomentum		35	//s-g functions
class AngularIndeces {
	public:
		short	x[kMaxAngularMomentum], y[kMaxAngularMomentum], z[kMaxAngularMomentum];

	AngularIndeces(void);
};

//setup arrays of x, y, z powers for each angular momenta (through g)
AngularIndeces::AngularIndeces(void) {
	for (long i=0; i<kMaxAngularMomentum; i++)
		x[i] = y[i] = z[i] = 0;
	x[1] = y[2] = z[3] = 1;	//P x, y, z
	x[4] = y[5] = z[6] = 2;	//D x2, y2, z2
	x[7] = x[8] = y[7] = y[9] = z[8] = z[9] = 1;//D xy,xz,yz
	x[10] = y[11] = z[12] = 3;	//F x3, y3, z3
	x[13] = x[14] = y[15] = y[16] = z[17] = z[18] = 2;//F x2y, x2z, xy2, y2z, xz2, yz2, xyz
	x[15] = x[17] = x[19] = y[13] = y[18] = y[19] = z[14] = z[16] = z[19] = 1;
	x[20] = y[21] = z[22] = 4;	//G x4, y4, z4
	x[23] = x[24] = y[25] = y[26] = z[27] = z[28] = 3;	//G x3y, x3z, xy3, y3z, xz3, yz3
	x[29] = x[30] = x[32] =	//G x2y2, x2z2, y2z2, x2yz, xy2z, xyz2
		y[29] = y[31] = y[33] = z[30] = z[31] = z[34] = 2;
	x[25] = x[27] = x[33] = x[34] = y[23] = y[28] = y[32] = y[34] =
		z[24] = z[26] = z[32] = z[33] = 1;
}
//NOTE: This routine assumes that the atomic coordinates are in Angstroms and are thus
//converted to Bohr when used.
void MEP2DSurface::CalculateMOGrid(MoleculeData *lData, Progress * lProgress) {
	long		NumPoints = NumGridPoints*NumGridPoints;
	GaussHermiteData	GHData;

//ProfilerClear();
//ProfilerSetStatus(1);
	InitGaussHermite(&GHData);	//setup the Gauss-Hermite roots and weights data

	Frame *	lFrame = lData->GetCurrentFramePtr();
	BasisSet * Basis = lData->GetBasisSet();
	AODensity * TotalAODensity = lFrame->GetAODensity(Basis, OrbSet);

	if ((Grid && (GridAllocation != NumPoints))||!TotalAODensity) {
		FreeGrid();
	}

	if (!Grid) {
			//Attempt to allocate memory for the 2D grid
		AllocateGrid(NumPoints);
	}
		//If the memory allocation failed the MO calculation can not be done
	if ((Grid == NULL)||(TotalAODensity==NULL)) return;
		//If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
#ifdef UseHandles
	HLock(Grid);
	lGrid = (float *) *Grid;
#else
	lGrid = Grid;
#endif
		//Store the Grid mins and incs at the beginning of the grid
	GridMax = -1.0e20;
	GridMin = 1.0e20;
		//loop over the plotting grid in the x, y, and z directions
	long n=0;
	float XGridValue, YGridValue, ZGridValue, junk1, junk2;
	CPoint3D	lOrigin, lXInc, lYInc;

	lOrigin = Origin * kAng2BohrConversion;
	lXInc = XInc * kAng2BohrConversion;
	lYInc = YInc * kAng2BohrConversion;
	for (long iXPt=0; iXPt<NumGridPoints; iXPt++) {
			//Give up the CPU and check for cancels
		if (!lProgress->UpdateProgress(100*iXPt/NumGridPoints)) {	//User canceled so clean things up and abort
			FreeGrid();
			return;
		}

		XGridValue = lOrigin.x + iXPt*lXInc.x;
		YGridValue = lOrigin.y + iXPt*lXInc.y;
		ZGridValue = lOrigin.z + iXPt*lXInc.z;
		for (long iYPt=0; iYPt<NumGridPoints; iYPt++) {
			lGrid[n] = lFrame->CalculateMEP(XGridValue, YGridValue, ZGridValue,
				Basis, TotalAODensity, &GHData, &junk1, &junk2);
			
			GridMax = MAX(GridMax, lGrid[n]);
			GridMin = MIN(GridMin, lGrid[n]);
			n++;
			XGridValue += lYInc.x;
			YGridValue += lYInc.y;
			ZGridValue += lYInc.z;
		}
	}
//ProfilerSetStatus(0);
//ProfilerDump("\pMEP profile");
#ifdef UseHandles
		//Unlock the grid handle and return
	HUnlock(Grid);
#endif
}
#ifdef powerc
typedef struct MEP3DGridData {
	MEP3DSurface *	Surf;
	long	xStart;
	long	xEnd;
	Frame *	lFrame;
	BasisSet *	Basis;
	AODensity	*	TotalAODensity;
	GaussHermiteData *	GHData;
	float		GridMax;
	long		PercentDone;
	MPQueueID	TerminationQueueID;
} MEP3DGridData;
static OSErr Calc3DMEPGrid(MEP3DGridData * Data) {
	Data->GridMax = Data->Surf->CalculateGrid(Data->xStart, Data->xEnd, Data->lFrame, Data->Basis,
		Data->TotalAODensity, Data->GHData,
		NULL, &(Data->PercentDone), true);
	return noErr;
}
#endif
float MEP3DSurface::CalculateGrid(long xStart, long xEnd, Frame * lFrame, BasisSet * Basis,
			AODensity * TotalAODensity, GaussHermiteData * GHData,
			Progress * lProgress, long * PercentDone, bool MPTask) {
	float lGridMax = -1.0e20, junk1, junk2;
	float lGridMin = 1.0e20;
	float XGridValue, YGridValue, ZGridValue;
	long	iXPt, iYPt, iZPt;
	long n=xStart*(NumYGridPoints*NumZGridPoints);
#ifdef UseHandles
	float * lGrid = (float *) *Grid;	//The Grid should already be locked down
#else
	float * lGrid = Grid;
#endif

	XGridValue = Origin.x + xStart * XGridInc;
	for (iXPt=xStart; iXPt<xEnd; iXPt++) {
			//Note the percent done for MP tasks such that the progress dlog can be updated
		if (MPTask) *PercentDone = 100*(iXPt-xStart)/(xEnd-xStart);
		YGridValue = Origin.y;
		for (iYPt=0; iYPt<NumYGridPoints; iYPt++) {
			if (!MPTask) {
					//Give up the CPU and check for cancels
				if (!lProgress->UpdateProgress(100*iXPt/xEnd)) {	//User canceled so clean things up and abort
					FreeGrid();
					return 0.0;
				}
			}
			ZGridValue = Origin.z;
			for (iZPt=0; iZPt<NumZGridPoints; iZPt++) {
				lGrid[n] = lFrame->CalculateMEP(XGridValue, YGridValue, ZGridValue,
					Basis, TotalAODensity, GHData, &junk1, &junk2);
				lGridMax = MAX(lGridMax, lGrid[n]);
				lGridMin = MIN(lGridMin, lGrid[n]);
				n++;
				ZGridValue += ZGridInc;
			}
			YGridValue += YGridInc;
		}
		XGridValue += XGridInc;
	}
	lGridMax = MAX(lGridMax, fabs(lGridMin));
	return lGridMax;
}
//Generates a 3D grid over the specified MO in the plane specified.
//NOTE: This routine assumes that the atomic coordinates are in Angstroms and are thus
//converted to Bohr when used.
void MEP3DSurface::CalculateMEPGrid(MoleculeData *lData, Progress * lProgress) {
	GaussHermiteData	GHData;

	InitGaussHermite(&GHData);	//setup the Gauss-Hermite roots and weights data

	Frame *	lFrame = lData->GetCurrentFramePtr();
	BasisSet * Basis = lData->GetBasisSet();
	AODensity * TotalAODensity = lFrame->GetAODensity(Basis, OrbSet);
	if (!TotalAODensity) throw MemoryError();;

	SetupGridParameters(lFrame);	//Define the 3D volume

		//allocate the memory needed for the grid
	long		NumPoints = NumXGridPoints*NumYGridPoints*NumZGridPoints;

	if (Grid && (GridAllocation != NumPoints)) FreeGrid();
	if (!Grid) {
			//Attempt to allocate memory for the 2D grid
		AllocateGrid(NumPoints);
	}
		//If the memory allocation failed the MO calculation can not be done
	if (Grid == NULL) return;
		//If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
#ifdef UseHandles
	HLock(Grid);
	lGrid = (float *) *Grid;
#else
	lGrid = Grid;
#endif
		//Get the appropriate MO vector
		//Store the Grid mins and incs at the beginning of the grid
	GridMax = -1.0e20;
#if defined(powerc) && defined(MacintoshBuild)
	if (gMPAware && (gNumProcessors>=1)) {
		OSErr	status;
		long	i, start=0;
		MEP3DGridData * DataPtrs = new MEP3DGridData[gNumProcessors];
		if (!DataPtrs) throw MemoryError();
			//Test allocations
		MPQueueID	* TerminationQueues = new MPQueueID[gNumProcessors];
		MPTaskID	* TaskIDs = new MPTaskID[gNumProcessors];
		if (!TerminationQueues || !TaskIDs) {
			delete [] DataPtrs;
			throw MemoryError();
		}
		for (i=0; i<gNumProcessors; i++) {
			status = MPCreateQueue(&(TerminationQueues[i]));	/* Create the queue which will report the completion of the task. */
			if (status != noErr) throw DataError();
			DataPtrs[i].Surf = this;
			DataPtrs[i].TerminationQueueID = TerminationQueues[i];
			DataPtrs[i].xStart = start;
			start += NumXGridPoints/gNumProcessors;
			if (i+1 == gNumProcessors) start = NumXGridPoints;
			DataPtrs[i].xEnd = start;
			DataPtrs[i].lFrame = lFrame;
			DataPtrs[i].Basis = Basis;
			DataPtrs[i].TotalAODensity = TotalAODensity;
			DataPtrs[i].GHData = &GHData;
			DataPtrs[i].PercentDone = 0;
			status = MPCreateTask((TaskProc)Calc3DMEPGrid,	/* This is the task function. */
                        &(DataPtrs[i]),		/* This is the parameter to the task function. */
                        16*1024,	/* stack size, 0 will use system default of 4KB which is too small for MEPs */
                        TerminationQueues[i],		/* We'll use this to sense task completion. */
                        0,						/* We won't use the first part of the termination message. */
                        0,						/* We won't use the second part of the termination message. */
						0,	/* Use the normal task options. (Currently this *must* be zero!) */
						&(TaskIDs[i]));					/* Here's where the ID of the new task will go. */
			if (status != noErr) throw DataError();
		}
		long NumTasksRunning=gNumProcessors;
		long	TotalPercent;
		lProgress->SetRunTime(1);
		lProgress->SetSleepTime(20);
		while (NumTasksRunning > 0) {
			TotalPercent = 0;
			for (i=0; i<gNumProcessors; i++) {
				TotalPercent += DataPtrs[i].PercentDone;
				if (TerminationQueues[i] != 0) {
						long temp;
					status = MPWaitOnQueue(TerminationQueues[i], 0, 0, (void **) &temp, kDurationImmediate);
					if (status == noErr) {
						GridMax = MAX(GridMax, DataPtrs[i].GridMax);
						status = MPDeleteQueue(TerminationQueues[i]);
						TerminationQueues[i] = 0;
						NumTasksRunning --;
					}
				}
			}
			TotalPercent /= gNumProcessors;
				//Give up the CPU and check for cancels
			if (!lProgress->UpdateProgress(TotalPercent)) {	//User canceled so clean things up and abort
				for (i=0; i<gNumProcessors; i++) {
					if (TerminationQueues[i] != 0) {
  						MPTerminateTask(TaskIDs[i], noErr);
  							long temp;
						status = MPWaitOnQueue(TerminationQueues[i], 0, 0, (void **) &temp, kDurationForever);
						if (status == noErr) {
							status = MPDeleteQueue(TerminationQueues[i]);
							TerminationQueues[i] = 0;
							NumTasksRunning --;
						}
					}
				}
				FreeGrid();
			}
		}
		delete [] TerminationQueues;
		delete [] TaskIDs;
		delete [] DataPtrs;
		lProgress->ResetTimes();
	} else 
#endif
	{
		long junk;	//just a placeholder when cooperative task
		GridMax = CalculateGrid(0,NumXGridPoints,lFrame,  Basis,
			TotalAODensity, &GHData, lProgress, &junk, false);
	}
		//Unlock the grid handle and return
#ifdef UseHandles
	if (Grid) HUnlock(Grid);
#endif
	Origin *= kBohr2AngConversion;
	XGridInc *= kBohr2AngConversion;
	YGridInc *= kBohr2AngConversion;
	ZGridInc *= kBohr2AngConversion;
}
void TEDensity3DSurface::CalculateSurfaceValues(MoleculeData *lData, Progress * lProgress) {
//	long		NumPoints = 3*NumPosContourTriangles;
	long		backpoint;
	GaussHermiteData	GHData;

	if (!ContourHndl) return;
	InitGaussHermite(&GHData);	//setup the Gauss-Hermite roots and weights data

	Frame *	lFrame = lData->GetCurrentFramePtr();
	BasisSet * Basis = lData->GetBasisSet();
	AODensity * TotalAODensity = lFrame->GetAODensity(Basis, OrbSet);

	if ((List && (ListAllocation != NumVertices))||!TotalAODensity)
		FreeList();

	if (!List) {
			//Attempt to allocate memory for the psuedo 2D grid
		AllocateList(NumVertices);
	}
		//If the memory allocation failed the MO calculation can not be done
	if (List == NULL) return;
		//If sufficient memory is available then setup pointers for fast local use
		float * lGrid;
		CPoint3D * Contour;
#ifdef UseHandles
	HLock(List);
	lGrid = (float *) *List;
	HLock(ContourHndl);
	Contour = (CPoint3D *) *ContourHndl;
#else
	lGrid = List;
	Contour = ContourHndl;
#endif
		//Store the Grid mins and incs at the beginning of the grid
		//loop over the plotting grid in the x, y, and z directions
	float XGridValue, YGridValue, ZGridValue, junk1, junk2;
	backpoint = 0;
	for (long iPt=0; iPt<NumVertices; iPt++) {
		if (backpoint > 30) {
				//Give up the CPU and check for cancels
			if (!lProgress->UpdateProgress(100*iPt/NumVertices)) {	//User canceled so clean things up and abort
				FreeList();
				return;
			}
			backpoint = 0;
		}

		XGridValue = Contour[iPt].x*kAng2BohrConversion;	//Contour values must be converted to bohrs
		YGridValue = Contour[iPt].y*kAng2BohrConversion;
		ZGridValue = Contour[iPt].z*kAng2BohrConversion;

		lGrid[iPt] = lFrame->CalculateMEP(XGridValue, YGridValue, ZGridValue,
			Basis, TotalAODensity, &GHData, &junk1, &junk2);
		backpoint++;
	}
		//Unlock the grid handle and return
#ifdef UseHandles
	HUnlock(List);
	HUnlock(ContourHndl);
#endif
}

//Calculates the MEP value at the specified x,y,z coordinate
float Frame::CalculateMEP(float XValue, float YValue, float ZValue,
		BasisSet *Basis, AODensity * TotalAODensity, GaussHermiteData * GHData,
		float * ElectronicMEP, float * NuclearMEP) {
	double	ElectronicPotential=0.0, NuclearPotential=0.0;	//results of the MEP calculation
	long	iatom, jatom, iprim, jprim, iroot, ijx[225], ijy[225], ijz[225],
			iang, jang, jmax, ij,nn, mm, NumJPrims, JPrimMax, TotalIShells, TotalJShells,
			nx, ny, nz, in, jn, li, lj;
	double	xi, yi, zi, xj, yj, zj, r2, temp;
	double	xin[125], yin[125], zin[125], wint[225], dij[225];
	Root	RysRoots;
	GaussHermiteIntegral	GHInt;
	AngularIndeces AngIndex;
	BasisShell * Shells = Basis->Shells;
	float * Density = TotalAODensity->GetDensityArray();
	long * DensityIndex = TotalAODensity->GetDensityIndex();
	short * DensityCheck = TotalAODensity->GetDensityCheck();
	long * NuclearCharge = Basis->GetNuclearChargeArray();

	double Pi212= 1.1283791670955;

	TotalIShells = -1;
	long CheckIndex = -1;
	for (iatom=0; iatom < NumAtoms; iatom++) {
		if (iatom > Basis->MapLength) continue;
		xi = Atoms[iatom].Position.x*kAng2BohrConversion;
		yi = Atoms[iatom].Position.y*kAng2BohrConversion;
		zi = Atoms[iatom].Position.z*kAng2BohrConversion;
		GHInt.xi = xi;
		GHInt.yi = yi;
		GHInt.zi = zi;
		for (long ishell=Basis->BasisMap[2*iatom]; ishell<=Basis->BasisMap[2*iatom+1]; ishell++) {
			TotalIShells ++;
			long Lishell = Shells[ishell].GetShellType();
			long NumIFuncs = Shells[ishell].GetNumFuncs(false);
			long IAngMin = Shells[ishell].GetAngularStart(false);
			long IAngMax = IAngMin + NumIFuncs;
			long iprimMax = Shells[ishell].GetNumPrimitives();
				//IAngMin is subtracted because it is added back later when the index is used
			long LocIindex = DensityIndex[TotalIShells] - IAngMin;
			
			TotalJShells = -1;
			for (jatom=0; jatom <= iatom; jatom++) {
				xj = Atoms[jatom].Position.x*kAng2BohrConversion;
				yj = Atoms[jatom].Position.y*kAng2BohrConversion;
				zj = Atoms[jatom].Position.z*kAng2BohrConversion;
				GHInt.xj = xj;
				GHInt.yj = yj;
				GHInt.zj = zj;
				r2 = (xi-xj)*(xi-xj) + (yi-yj)*(yi-yj) + (zi-zj)*(zi-zj);
				long jShellMax = Basis->BasisMap[2*jatom+1];
				if (iatom == jatom) jShellMax = ishell;
				for (long jshell=Basis->BasisMap[2*jatom]; jshell<=jShellMax; jshell++) {
					TotalJShells ++;
					bool IequalsJ = (iatom == jatom) && (ishell == jshell);
					long Ljshell = Shells[jshell].GetShellType();
					long NumJFuncs = Shells[jshell].GetNumFuncs(false);
					long JAngMin = Shells[jshell].GetAngularStart(false);
					long JAngMax = JAngMin + NumJFuncs;
						//JAngMin is subtracted because it is added back later when the index is used
					long LocJindex = DensityIndex[TotalJShells]-JAngMin;
		//Attempt to screen based on the density
					CheckIndex ++;
					if ( ! DensityCheck[CheckIndex]) continue;

					NumJPrims = JPrimMax = Shells[jshell].GetNumPrimitives();
					
					RysRoots.nRoots = (Lishell + Ljshell)/2 + 1;

					ij=0;
					jmax = JAngMax;
					for (iang=IAngMin; iang<IAngMax; iang++) {
						nx = 5*AngIndex.x[iang];
						ny = 5*AngIndex.y[iang];
						nz = 5*AngIndex.z[iang];

						if (IequalsJ) jmax = iang+1;
						for (jang=JAngMin; jang<jmax; jang++) {
							ijx[ij] = nx + AngIndex.x[jang];
							ijy[ij] = ny + AngIndex.y[jang];
							ijz[ij] = nz + AngIndex.z[jang];
							ij++;
						}
					}
					for (iprim=0; iprim<225; iprim++) wint[iprim]=0.0;

					for (iprim=0; iprim<iprimMax; iprim++) {
						float Ai = Shells[ishell].Exponent[iprim];

						if (IequalsJ) JPrimMax = iprim+1;
						for (jprim=0; jprim<JPrimMax; jprim++) {
							float Aj = Shells[jshell].Exponent[jprim];
							
							float AiAj = Ai + Aj;
							double OneDAiAj = 1.0/AiAj;
							double Fi = Pi212 * OneDAiAj;

							double AAx = (Ai*xi + Aj*xj);
							double AAy = (Ai*yi + Aj*yj);
							double AAz = (Ai*zi + Aj*zj);

							double Ax = AAx*OneDAiAj;
							double Ay = AAy*OneDAiAj;
							double Az = AAz*OneDAiAj;

							temp = Ai*Aj*r2*OneDAiAj;
								//skip products with exponentials smaller than exp^-46 (GAMESS default)
							if (temp > 46.0) continue;
							double ExpFactor = Fi*exp(-temp);
							bool Double = IequalsJ && (iprim != jprim);
							nn=0;
							jmax = JAngMax;
							double DensityFactorI, DensityFactorJ;
									//Compute the primitive coefficient factors
							for (iang=IAngMin; iang<IAngMax; iang++) {
								if (IAngMin == iang)
									DensityFactorI = ExpFactor * Shells[ishell].NormCoef[iprim];
								else if ((iang == 1) && (NumIFuncs == 4))
									DensityFactorI = ExpFactor * Shells[ishell].NormCoef[iprim+iprimMax];
	//Angular normalization factors are applied already in the AO density
								if (IequalsJ) jmax = iang+1;
								for (jang=JAngMin; jang<jmax; jang++) {
									if (JAngMin == jang) {
										DensityFactorJ = DensityFactorI * Shells[jshell].NormCoef[jprim];
										if (Double) {
											if (jang != 0 || iang == 0) DensityFactorJ += DensityFactorJ;
											else DensityFactorJ += Shells[ishell].NormCoef[iprim]*
												Shells[jshell].NormCoef[jprim+NumJPrims]*ExpFactor;
										}
									} else if ((jang == 1) && (NumJFuncs == 4)) {
										DensityFactorJ = DensityFactorI * Shells[jshell].NormCoef[jprim+NumJPrims];
										if (Double) DensityFactorJ += DensityFactorJ;
									}

									dij[nn] = DensityFactorJ;
									nn++;
								}
							}

							RysRoots.x = AiAj *((Ax-XValue)*(Ax-XValue) + (Ay-YValue)*(Ay-YValue) + (Az-ZValue)*(Az-ZValue));
							if (RysRoots.nRoots <= 3) Root123(&RysRoots);
							else if (RysRoots.nRoots == 4) Root4(&RysRoots);
							else if (RysRoots.nRoots == 5) Root5(&RysRoots);
							else Root6(&RysRoots);

							mm=0;
							for (iroot=0; iroot<RysRoots.nRoots; iroot++) {
								double UU = AiAj*RysRoots.root[iroot];
								double WW = RysRoots.weight[iroot];
								double tt = 1.0/(AiAj+UU);

								GHInt.T = sqrt(tt);
								GHInt.x0 = (AAx + UU*XValue)*tt;
								GHInt.y0 = (AAy + UU*YValue)*tt;
								GHInt.z0 = (AAz + UU*ZValue)*tt;

								in = mm;
								for (iang=0; iang<=Lishell; iang++) {
									GHInt.NumI = iang;
									for (jang=0; jang<=Ljshell; jang++) {
										jn = in+jang;
										GHInt.NumJ = jang;
										DoGaussHermite(GHData, &GHInt);

										xin[jn] = GHInt.xIntegral;
										yin[jn] = GHInt.yIntegral;
										zin[jn] = GHInt.zIntegral * WW;
									}
									in += 5;
								}
								mm += 25;
							}	//Loop over orbital products
							for (iang=0; iang<ij; iang++) {
								nx = ijx[iang];
								ny = ijy[iang];
								nz = ijz[iang];
								double sum = 0.0;
								mm = 0;
								for (iroot=0; iroot<RysRoots.nRoots; iroot++) {
									sum += xin[nx+mm] * yin[ny+mm] * zin[nz+mm];
									mm += 25;
								}
								wint[iang] += sum*dij[iang];
							}
						}
					}	//End of primitive loops
					long index = 0;
					double dw;
					jmax = JAngMax;
					temp = 0.0;
					for (iang=IAngMin; iang<IAngMax; iang++) {
						li = LocIindex + iang;
						in = li*(li+1)/2;
						if (IequalsJ) jmax = iang+1;
						for (jang=JAngMin; jang<jmax; jang++) {
							lj = LocJindex + jang;
							dw = Density[lj + in]*wint[index];
							if (li != lj) dw += dw;
							temp += dw;
							index ++;
						}
					}
					ElectronicPotential -= temp;
				}
			}
		}	//now for the nuclear contribution
		xi -= XValue;
		yi -= YValue;
		zi -= ZValue;
		r2 = sqrt(xi*xi + yi*yi + zi*zi);
		if (r2 > 0.001) NuclearPotential += NuclearCharge[iatom] / r2;
	}
	*ElectronicMEP = ElectronicPotential;
	*NuclearMEP = NuclearPotential;
	return (ElectronicPotential + NuclearPotential);
}
AODensity::AODensity(void) {
	DensityArray = NULL;
	IndexArray = NULL;
	DensityCheckArray=NULL;
	Dimension = 0;
}
AODensity::~AODensity(void) {
	if (DensityArray) delete DensityArray;
	if (IndexArray) delete IndexArray;
	if (DensityCheckArray) delete DensityCheckArray;
}
void AODensity::SetupMemory(long NumBasisFunctions) {
	if (DensityArray) {delete DensityArray; DensityArray = NULL;}
	if (IndexArray) {delete IndexArray; IndexArray = NULL;}
	if (DensityCheckArray) {delete DensityCheckArray; DensityCheckArray=NULL;}
	if (NumBasisFunctions > 0) {
		DensityArray = new float[(NumBasisFunctions*(NumBasisFunctions+1))/2];
		IndexArray = new long[NumBasisFunctions];
		DensityCheckArray = new short[(NumBasisFunctions*(NumBasisFunctions+1))/2];
		if (!DensityArray || !IndexArray || !DensityCheckArray) throw MemoryError();
		Dimension = NumBasisFunctions;
	}
}
AODensity * OrbitalRec::GetAODensity(BasisSet * lBasis, long NumAtoms) {
	if (!TotalAODensity) {
//		if (!EigenVectors) throw DataError();
		long NumBasisFunctions = lBasis->GetNumBasisFuncs(false);
		TotalAODensity = new AODensity;
		if (TotalAODensity) {
			TotalAODensity->SetupMemory(NumBasisFunctions);
			GetAODensityMatrix(TotalAODensity->GetDensityArray(), NumOccupiedAlphaOrbs,
				NumOccupiedBetaOrbs, NumBasisFunctions);
			lBasis->GetShellIndexArray(TotalAODensity->GetDensityIndex());

			BasisShell * Shells = lBasis->Shells;
			float * Density = TotalAODensity->GetDensityArray();
			long * DensityIndex = TotalAODensity->GetDensityIndex();
			short * DensityCheck = TotalAODensity->GetDensityCheck();
			long iatom, jatom, TotalJShells, iang, jang, li, in, lj, jmax;
			double	DensityFactorI, DensityFactorJ;
			long CheckIndex=0;
			long TotalIShells = -1;
			double sqrt3 = sqrt(3.0);
			double onedsqrt3 = 1/sqrt3;
			double sqrt5 = sqrt(5.0);
			double sqrt7 = sqrt(7.0);
			for (iatom=0; iatom < NumAtoms; iatom++) {
				if (iatom > lBasis->MapLength) continue;
				for (long ishell=lBasis->BasisMap[2*iatom]; ishell<=lBasis->BasisMap[2*iatom+1]; ishell++) {
					TotalIShells ++;
	//				long Lishell = Shells[ishell].GetShellType();
					long NumIFuncs = Shells[ishell].GetNumFuncs(false);
					long IAngMin = Shells[ishell].GetAngularStart(false);
					long IAngMax = IAngMin + NumIFuncs;
						//IAngMin is subtracted because it is added back later when the index is used
					long LocIindex = DensityIndex[TotalIShells] - IAngMin;
					
					TotalJShells = -1;
					for (jatom=0; jatom <= iatom; jatom++) {
						long jShellMax = lBasis->BasisMap[2*jatom+1];
						if (iatom == jatom) jShellMax = ishell;
						for (long jshell=lBasis->BasisMap[2*jatom]; jshell<=jShellMax; jshell++) {
							TotalJShells ++;
							bool IequalsJ = (iatom == jatom) && (ishell == jshell);
	//						long Ljshell = Shells[jshell].GetShellType();
							long NumJFuncs = Shells[jshell].GetNumFuncs(false);
							long JAngMin = Shells[jshell].GetAngularStart(false);
							long JAngMax = JAngMin + NumJFuncs;
								//JAngMin is subtracted because it is added back later when the index is used
							long LocJindex = DensityIndex[TotalJShells]-JAngMin;
				//Attempt to screen based on the density
							short	NonZeroShells=0;
							jmax = JAngMax;
							for (iang=IAngMin; iang<IAngMax; iang++) {
								if (IAngMin == iang)
									DensityFactorI = 1.0;
								if (iang >= (IAngMin+3)) {
									switch (iang) {	//properly normalize d, f, and g's
										case 7:
										case 19:
										case 32:
											DensityFactorI *= sqrt3;
										break;
										case 13:
											DensityFactorI *= sqrt5;
										break;
										case 23:
											DensityFactorI *= sqrt7;
										break;
										case 29:
											DensityFactorI *= sqrt5*onedsqrt3;
										break;
									}
								}
								li = LocIindex + iang;
								in = li*(li+1)/2;
								if (IequalsJ) jmax = iang+1;
								for (jang=JAngMin; jang<jmax; jang++) {
									lj = LocJindex + jang;
									if (JAngMin == jang) 
										DensityFactorJ = DensityFactorI;
									if (jang >= (JAngMin+3)) {
										switch (jang) {	//properly normalize d, f, and g's
											case 7:
											case 19:
											case 32:
												DensityFactorJ *= sqrt3;
											break;
											case 13:
												DensityFactorJ *= sqrt5;
											break;
											case 23:
												DensityFactorJ *= sqrt7;
											break;
											case 29:
												DensityFactorJ *= sqrt5*onedsqrt3;
											break;
										}
									}
									Density[lj+in] *= DensityFactorJ;
									if (fabs(Density[lj+in]) > 0.0001) NonZeroShells++;
								}
							}
							DensityCheck[CheckIndex] = NonZeroShells;
							CheckIndex++;
						}
					}
				}
			}

		}
		if (!TotalAODensity) throw MemoryError();
	}
	return TotalAODensity;
}

void OrbitalRec::GetAODensityMatrix(float * AODensityArray, long NumOccAlpha, long NumOccBeta,
		long NumBasisFuncs) const {
	long	n, i, j, m;

	if (!AODensityArray) throw MemoryError();	//Memory must be allocated by the caller
	float	*OccupancyA=NULL, *OccupancyB=NULL;
	if (BaseWavefunction == RHF) {
		OccupancyA = new float[NumOccAlpha];
		if (OccupancyA) for (i=0; i<NumOccAlpha; i++) OccupancyA[i]=2.0;
	} else if (BaseWavefunction == ROHF) {
		OccupancyA = new float[NumOccAlpha];
		if (OccupancyA) for (i=0; i<NumOccAlpha; i++) {
			if (i<NumOccBeta) OccupancyA[i]=2.0;
			else OccupancyA[i]=1.0;
		}
	} else if ((BaseWavefunction == UHF)&&(OrbitalType != NaturalOrbital)) {
		OccupancyA = new float[NumOccAlpha];
		if (OccupancyA) for (i=0; i<NumOccAlpha; i++) OccupancyA[i]=1.0;
		OccupancyB = new float[NumOccBeta];
		if (OccupancyB) for (i=0; i<NumOccBeta; i++) OccupancyB[i]=1.0;
	} else if ((BaseWavefunction == MCSCF)||(BaseWavefunction == RHFMP2)||(BaseWavefunction == UHF)) {
		OccupancyA = OrbOccupation;
	}
	if (!OccupancyA) throw MemoryError();
		//clear the AO density array
	n=0;
	for (i=0; i<NumBasisFuncs; i++) {
		for (j=0; j<=i; j++) {
			AODensityArray[n]=0.0;
			n++;
		}
	}

	for (m=0; m<NumOccAlpha; m++) {
		n=0;
		for (i=0; i<NumBasisFuncs; i++) {
			for (j=0; j<=i; j++) {
				AODensityArray[n] += Vectors[i+m*NumBasisFuncs]*OccupancyA[m]*Vectors[j+m*NumBasisFuncs];
				n++;
			}
		}
	}
	if ((BaseWavefunction == UHF)&&(OrbitalType != NaturalOrbital)) {
		if (!OccupancyB) throw MemoryError();
		for (m=0; m<NumOccBeta; m++) {
			n=0;
			for (i=0; i<NumBasisFuncs; i++) {
				for (j=0; j<=i; j++) {
					AODensityArray[n] += VectorsB[i+m*NumBasisFuncs]*OccupancyB[m]*VectorsB[j+m*NumBasisFuncs];
					n++;
				}
			}
		}
	}
	if ((BaseWavefunction != MCSCF)&&(BaseWavefunction != RHFMP2)&&
		!((BaseWavefunction == UHF)&&(OrbitalType == NaturalOrbital)))
		delete OccupancyA;
	if (OccupancyB) delete OccupancyB;
}