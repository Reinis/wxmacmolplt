/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Internals.cpp

	Classes to handle internal coordinates

	BMB 4/1998
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "Internals.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "Math3D.h"

Internals::Internals(void) {
	MOPacStyle = NULL;
	GeneralStyle = NULL;
}
Internals::~Internals(void) {
	if (MOPacStyle) delete MOPacStyle;
	if (GeneralStyle) delete GeneralStyle;
}
void Internals::CreateMOPacInternals(long Num) {
	if (!MOPacStyle) {
		MOPacStyle = new MOPacInternals(Num);
	}
}
long Internals::GetSize(BufferFile * Buffer) {
	Boolean	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = Write(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
long Internals::Write(BufferFile * Buffer) {
	long total = 0;
	
	if (MOPacStyle) {
		long code = 1;
		total = Buffer->Write((Ptr) &code, sizeof(long));
		long length = MOPacStyle->GetSize(Buffer);
		total += Buffer->Write((Ptr) &length, sizeof(long));
		total += MOPacStyle->Write(Buffer);
	}
	return total;
}
Internals * Internals::Read(BufferFile * Buffer) {
		long code;
		Internals * temp=NULL;
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code == 1) {
		temp = new Internals;
		if (!temp) throw DataError();
		Buffer->Read((Ptr) &code, sizeof(long));
		temp->MOPacStyle = new MOPacInternals(Buffer);
	}
	return temp;
}
long MOPacInternals::GetSize(BufferFile * Buffer) {
	Boolean	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = Write(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
long MOPacInternals::Write(BufferFile * Buffer) {
	long code = 1;
	long total = Buffer->Write((Ptr) &code, sizeof(long));
	long length = sizeof(MOPacInternals);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) this, length);

	code = 2;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = Count * sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) ConnectionAtoms, length);
	
	code = 3;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = Count * sizeof(float);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) Values, length);
	
	code = 4;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = Count * sizeof(char);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) Type, length);
	
	return total;
}
MOPacInternals::MOPacInternals(BufferFile * Buffer) {
		long code;
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 1) throw DataError();
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code != sizeof(MOPacInternals)) throw DataError();
	Buffer->Read((Ptr) this, sizeof(MOPacInternals));
	ConnectionAtoms = new long[Count];
	Values = new float[Count];
	Type = new char[Count];
	if (!ConnectionAtoms || !Values || !Type) throw MemoryError();
	Allocation = Count;

	Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 2) throw DataError();
	Buffer->Read((Ptr) &code, sizeof(long));
	Buffer->Read((Ptr) ConnectionAtoms, code);
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 3) throw DataError();
	Buffer->Read((Ptr) &code, sizeof(long));
	Buffer->Read((Ptr) Values, code);
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 4) throw DataError();
	Buffer->Read((Ptr) &code, sizeof(long));
	Buffer->Read((Ptr) Type, code);
}
MOPacInternals::MOPacInternals(long Num) {
	ConnectionAtoms = new long[Num];
	Values = new float[Num];
	Type = new char[Num];
	if (!ConnectionAtoms || !Values || !Type) throw MemoryError();
	Allocation = Num;
	Count = 0;
}
MOPacInternals::~MOPacInternals(void) {
	if (ConnectionAtoms) delete [] ConnectionAtoms;
	if (Values) delete [] Values;
	if (Type) delete [] Type;
}
//Guess the connection atom set, then setup the values
void MOPacInternals::GuessInit(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (3*lFrame->NumAtoms > Allocation) return;
	if (lFrame->NumAtoms < 2) return;
		// First pair of atoms
	ConnectionAtoms[3] = 0;
		CPoint3D BondVector, testVector;
	BondVector.x = lFrame->Atoms[1].Position.x - lFrame->Atoms[0].Position.x;
	BondVector.y = lFrame->Atoms[1].Position.y - lFrame->Atoms[0].Position.y;
	BondVector.z = lFrame->Atoms[1].Position.z - lFrame->Atoms[0].Position.z;
	Values[3] = BondVector.Magnitude();
	Type[3] = 0;
		long BondedAtom, AngleAtom, DihedralAtom, i;
		float BondLength, SecondLength, ThirdLength, testLength;
	for (long iatom=2; iatom<lFrame->NumAtoms; iatom++) {
			//Find the closest atom to choose as the bonded atom
		BondedAtom = 0;
		BondVector.x = lFrame->Atoms[iatom].Position.x - lFrame->Atoms[0].Position.x;
		BondVector.y = lFrame->Atoms[iatom].Position.y - lFrame->Atoms[0].Position.y;
		BondVector.z = lFrame->Atoms[iatom].Position.z - lFrame->Atoms[0].Position.z;
		BondLength = BondVector.Magnitude();
		SecondLength = ThirdLength = 1.0E10;
		AngleAtom = DihedralAtom = 0;
		for (i=1; i<iatom; i++) {
			testVector.x = lFrame->Atoms[iatom].Position.x - lFrame->Atoms[i].Position.x;
			testVector.y = lFrame->Atoms[iatom].Position.y - lFrame->Atoms[i].Position.y;
			testVector.z = lFrame->Atoms[iatom].Position.z - lFrame->Atoms[i].Position.z;
			testLength = testVector.Magnitude();
			if (testLength<BondLength) {
				ThirdLength = SecondLength;
				DihedralAtom = AngleAtom;
				SecondLength = BondLength;
				AngleAtom = BondedAtom;
				BondedAtom = i;
				BondLength = testLength;
				BondVector = testVector;
			} else if (testLength<SecondLength) {
				ThirdLength = SecondLength;
				DihedralAtom = AngleAtom;
				SecondLength = testLength;
				AngleAtom = i;
			} else if (testLength<ThirdLength) {
				ThirdLength = testLength;
				DihedralAtom = i;
			}
		}	//the three closest atoms are now known
		ConnectionAtoms[3*iatom] = BondedAtom;
		Values[3*iatom] = BondLength;
		Type[3*iatom] = 0;
			//Bond angle - atom -> BondedAtom -> AngleAtom
		ConnectionAtoms[3*iatom+1] = AngleAtom;
		Type[3*iatom+1] = 1;
		if (iatom>2) {
				//Dihedral Angle
			ConnectionAtoms[3*iatom+2] = DihedralAtom;
			Type[3*iatom+2] = 2;
		}
	}
	Count = 3*lFrame->NumAtoms;
	CartesiansToInternals(MainData);
}
//Take the given cartesians and connection list and recalculate
//the values of each internal coordinate
void MOPacInternals::CartesiansToInternals(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (3*lFrame->NumAtoms > Count) return;
	if (lFrame->NumAtoms < 2) return;
		long BondedAtom, AngleAtom, DihedralAtom;
		float BondLength, BondAngle, Dihedral;
		// First pair of atoms
	BondedAtom = ConnectionAtoms[3];
		CPoint3D BondVector, offset2, offset3;
	BondVector.x = lFrame->Atoms[1].Position.x - lFrame->Atoms[BondedAtom].Position.x;
	BondVector.y = lFrame->Atoms[1].Position.y - lFrame->Atoms[BondedAtom].Position.y;
	BondVector.z = lFrame->Atoms[1].Position.z - lFrame->Atoms[BondedAtom].Position.z;
	Values[3] = BondVector.Magnitude();
	for (long iatom=2; iatom<lFrame->NumAtoms; iatom++) {
			//Bond Length
		BondedAtom = ConnectionAtoms[3*iatom];
		BondVector.x = lFrame->Atoms[BondedAtom].Position.x - lFrame->Atoms[iatom].Position.x;
		BondVector.y = lFrame->Atoms[BondedAtom].Position.y - lFrame->Atoms[iatom].Position.y;
		BondVector.z = lFrame->Atoms[BondedAtom].Position.z - lFrame->Atoms[iatom].Position.z;
		BondLength = BondVector.Magnitude();
		Values[3*iatom] = BondLength;
			//Bond angle
		AngleAtom = ConnectionAtoms[3*iatom+1];
		offset2.x = lFrame->Atoms[iatom].Position.x - lFrame->Atoms[AngleAtom].Position.x;
		offset2.y = lFrame->Atoms[iatom].Position.y - lFrame->Atoms[AngleAtom].Position.y;
		offset2.z = lFrame->Atoms[iatom].Position.z - lFrame->Atoms[AngleAtom].Position.z;
		float length3 = offset2.Magnitude();
		offset2.x = lFrame->Atoms[AngleAtom].Position.x - lFrame->Atoms[BondedAtom].Position.x;
		offset2.y = lFrame->Atoms[AngleAtom].Position.y - lFrame->Atoms[BondedAtom].Position.y;
		offset2.z = lFrame->Atoms[AngleAtom].Position.z - lFrame->Atoms[BondedAtom].Position.z;
		float length2 = offset2.Magnitude();
		if ((fabs(BondLength)>0.0001)&&(fabs(length3)>0.0001)&&(fabs(length2)>0.0001)) {
			float Radians = (BondLength*BondLength+length2*length2-length3*length3)/
							(2*BondLength*length2);
				//Make sure the angle is within the allowable values for acos (-1 to 1)
			if (Radians > 1.0) Radians = 1.0;
			else if (Radians < -1.0) Radians = -1.0;
			BondAngle = acos(Radians);
			BondAngle *= kRadToDegree;
		} else BondAngle = 0.0;
		Values[3*iatom+1] = BondAngle;
			//Dihedral angle
		if (iatom > 2) {
			DihedralAtom = ConnectionAtoms[3*iatom+2];
			offset3.x = lFrame->Atoms[DihedralAtom].Position.x - lFrame->Atoms[AngleAtom].Position.x;
			offset3.y = lFrame->Atoms[DihedralAtom].Position.y - lFrame->Atoms[AngleAtom].Position.y;
			offset3.z = lFrame->Atoms[DihedralAtom].Position.z - lFrame->Atoms[AngleAtom].Position.z;
		//	float length3 = offset3.Magnitude();
			CPoint3D UnitIJ = BondVector;
			CPoint3D UnitJK = offset2;
			CPoint3D UnitKL = offset3;
			Normalize3D(&UnitIJ);
			Normalize3D(&UnitJK);
			Normalize3D(&UnitKL);
			CPoint3D Normal1, Normal2;
			CrossProduct3D(&UnitIJ, &UnitJK, &Normal1);
			CrossProduct3D(&UnitJK, &UnitKL, &Normal2);
			float DotPJ = DotProduct3D(&UnitIJ, &UnitJK);
			float DotPK = DotProduct3D(&UnitJK, &UnitKL);
			DotPJ = 1.0 - DotPJ*DotPJ;
			DotPK = 1.0 - DotPK*DotPK;
			if ((DotPJ > 0.0)||(DotPK > 0.0)) {	//3 of the atom are linear, Bad!
				float SinPJ = sqrt(DotPJ);
				float SinPK = sqrt(DotPK);
				float Dot = DotProduct3D(&Normal1, &Normal2)/(SinPJ*SinPK);
				if (fabs(Dot) <= kCosErrorTolerance) {		//Bad value for a cos
					if (Dot > 0.9999) Dot = 1.0;
					else if (Dot < -0.9999) Dot = -1.0;
					Dihedral = acos(Dot);
					float Pi = acos(-1.0);
					if (fabs(Dihedral) < kZeroTolerance) Dihedral = 0.0;
					else if (fabs(Dihedral-Pi) < kZeroTolerance) Dihedral = Pi;
					float Sense = DotProduct3D(&Normal2, &BondVector);
					if (Sense < 0.0) Dihedral = -Dihedral;
					Dihedral *= 180.0/Pi;
				}
				Values[3*iatom+2] = Dihedral;
			}
		}
	}
}
//Take the connections list and values to create a set of cartesian coordinates
void MOPacInternals::InternalsToCartesians(MoleculeData * MainData, WinPrefs * Prefs,
		long ChangedAtom) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	double DegToRad = acos(-1.0)/180.0;

	if (lFrame->NumAtoms>1) {
	 mpAtom * tempAtoms = new Atom[lFrame->NumAtoms];
		if (tempAtoms) {
				long i;
			for (i=0; i<lFrame->NumAtoms; i++) tempAtoms[i].Type = lFrame->Atoms[i].Type;
			if (ChangedAtom < 3) {
				tempAtoms[0].Position.x = tempAtoms[0].Position.y = tempAtoms[0].Position.z = 0;
				tempAtoms[1].Position.x = tempAtoms[1].Position.y = 0.0;
				tempAtoms[1].Position.z = Values[3];
			} else {
				tempAtoms[0].Position.x = lFrame->Atoms[0].Position.x;
				tempAtoms[0].Position.y = lFrame->Atoms[0].Position.y;
				tempAtoms[0].Position.z = lFrame->Atoms[0].Position.z;
				tempAtoms[1].Position.x = lFrame->Atoms[1].Position.x;
				tempAtoms[1].Position.y = lFrame->Atoms[1].Position.y;
				tempAtoms[1].Position.z = lFrame->Atoms[1].Position.z;
			}
			if (lFrame->NumAtoms > 2) {
				if (ChangedAtom < 3) {
					tempAtoms[2].Position.x = Values[6]*sin(Values[7]*DegToRad);
					tempAtoms[2].Position.y = 0.0;
					if (ConnectionAtoms[6]==0)
						tempAtoms[2].Position.z = Values[6]*cos(Values[7]*DegToRad);
					else tempAtoms[2].Position.z = tempAtoms[1].Position.z - 
						Values[6]*cos(Values[7]*DegToRad);
					i=3;	//add atoms using simple formula as long as molecule is linear
					while ((tempAtoms[i-1].Position.x <= 1.0e-5)&&(i<lFrame->NumAtoms)) {
						tempAtoms[i].Position.x = Values[3*i]*sin(Values[3*i+1]*DegToRad);
						tempAtoms[i].Position.y = 0.0;
						float test = tempAtoms[ConnectionAtoms[3*i]].Position.z -
							tempAtoms[ConnectionAtoms[3*i+1]].Position.z;
						tempAtoms[i].Position.z = tempAtoms[ConnectionAtoms[3*i]].Position.z -
							Values[3*i]*cos(Values[3*i+1]*DegToRad)*((test>=0.0)?1.0:0.0);
						i++;
					}
				} else {
					for (long j=2; j<ChangedAtom; j++) {
						tempAtoms[j].Position.x = lFrame->Atoms[j].Position.x;
						tempAtoms[j].Position.y = lFrame->Atoms[j].Position.y;
						tempAtoms[j].Position.z = lFrame->Atoms[j].Position.z;
					}
					i = ChangedAtom;
				}	//Now for general 3D molecules
				for (; i<lFrame->NumAtoms; i++) {
						long j, k, l;
					j = ConnectionAtoms[3*i];
					k = ConnectionAtoms[3*i+1];
					l = ConnectionAtoms[3*i+2];
						CPoint3D Vkl, Vjk;
					Vkl.x = tempAtoms[k].Position.x - tempAtoms[l].Position.x;
					Vkl.y = tempAtoms[k].Position.y - tempAtoms[l].Position.y;
					Vkl.z = tempAtoms[k].Position.z - tempAtoms[l].Position.z;
					CPoint3D Unitkl = Vkl;
					Normalize3D(&Unitkl);
					Vjk.x = tempAtoms[j].Position.x - tempAtoms[k].Position.x;
					Vjk.y = tempAtoms[j].Position.y - tempAtoms[k].Position.y;
					Vjk.z = tempAtoms[j].Position.z - tempAtoms[k].Position.z;
					CPoint3D Unitjk = Vjk;
					Normalize3D(&Unitjk);
						CPoint3D KLxJK;
					CrossProduct3D(&Unitkl, &Unitjk, &KLxJK);
						CPoint3D A3;
					double klDotjk = DotProduct3D(&Unitkl, &Unitjk);
					klDotjk = 1.0 - klDotjk*klDotjk;
						double SinA3;
					if (klDotjk > 0.0001)
						SinA3 = sqrt(klDotjk);
					else
						SinA3 = 0.001;
					A3.x = KLxJK.x/SinA3;
					A3.y = KLxJK.y/SinA3;
					A3.z = KLxJK.z/SinA3;
						CPoint3D A4;
					CrossProduct3D(&A3, &Unitjk, &A4);
     					CPoint3D AtomVector;
     				AtomVector.x = Values[3*i]*((-Unitjk.x*cos(Values[3*i+1]*DegToRad)) +
     					A4.x*sin(Values[3*i+1]*DegToRad)*cos(Values[3*i+2]*DegToRad) +
     					A3.x*sin(Values[3*i+1]*DegToRad)*sin(Values[3*i+2]*DegToRad));
     				AtomVector.y = Values[3*i]*((-Unitjk.y*cos(Values[3*i+1]*DegToRad)) +
     					A4.y*sin(Values[3*i+1]*DegToRad)*cos(Values[3*i+2]*DegToRad) +
     					A3.y*sin(Values[3*i+1]*DegToRad)*sin(Values[3*i+2]*DegToRad));
     				AtomVector.z = Values[3*i]*((-Unitjk.z*cos(Values[3*i+1]*DegToRad)) +
     					A4.z*sin(Values[3*i+1]*DegToRad)*cos(Values[3*i+2]*DegToRad) +
     					A3.z*sin(Values[3*i+1]*DegToRad)*sin(Values[3*i+2]*DegToRad));
     				tempAtoms[i].Position.x = tempAtoms[j].Position.x + AtomVector.x;
     				tempAtoms[i].Position.y = tempAtoms[j].Position.y + AtomVector.y;
     				tempAtoms[i].Position.z = tempAtoms[j].Position.z + AtomVector.z;
				}
			}	//rotate/translate the new coordinates to match the existing coords
				//note this only attempts to match the first four atoms, the others
				//will be allowed to wander
//			MinimizeDifferences(lFrame->Atoms, tempAtoms, lFrame->NumAtoms, Prefs,
//				max(lFrame->NumAtoms, 4));
			if (ChangedAtom < 4)
				MinimizeDifferences(lFrame->Atoms, tempAtoms, lFrame->NumAtoms, Prefs,
					lFrame->NumAtoms);
			for (i=0; i<lFrame->NumAtoms; i++)
				lFrame->Atoms[i].Position = tempAtoms[i].Position;
			delete [] tempAtoms;
		}
	}
}
void MOPacInternals::AddAtom(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (3*lFrame->NumAtoms > Allocation) {
		long * templ = new long[3*lFrame->NumAtoms+12];
		if (templ) {
			BlockMoveData(ConnectionAtoms, templ, Count*sizeof(long));
			delete [] ConnectionAtoms;
			ConnectionAtoms = templ;
		} else throw MemoryError();
		float * tempVal = new float[3*lFrame->NumAtoms+12];
		if (tempVal) {
			BlockMoveData(Values, tempVal, Count*sizeof(float));
			delete [] Values;
			Values = tempVal;
		} else throw MemoryError();
		char * tempType = new char [3*lFrame->NumAtoms+12];
		if (tempType) {
			BlockMoveData(Type, tempType, Count*sizeof(char));
			delete [] Type;
			Type = tempType;
		} else throw MemoryError();
		Allocation = 3*lFrame->NumAtoms + 12;
	}
		//Now add the new atoms connections to the end of the list - valid since
		//new atoms are always appended to the end of the atom list
	long newAtom = Count/3;
	ConnectionAtoms[3*newAtom] = 0;		//Just put in some valid defaults
	ConnectionAtoms[3*newAtom+1] = 1;
	ConnectionAtoms[3*newAtom+2] = 2;
	Count += 3;
	CartesiansToInternals(MainData);	//Calculate the values for the new atom
}
void MOPacInternals::DeleteAtom(MoleculeData * /*MainData*/, long WhichAtom) {
//	Frame * lFrame = MainData->GetCurrentFramePtr();
	for (long i=3*WhichAtom; i<(Count-3); i++) {	//pull down the atoms higher in the list
		ConnectionAtoms[i] = ConnectionAtoms[i+3];
		if (ConnectionAtoms[i] >= WhichAtom) ConnectionAtoms[i]--;	//reduce connection atom count
		Values[i] = Values[i+3];
		Type[i] = Type[i+3];
	}
	if (Count > 0) Count -= 3;	//reduce the number of internal coordinates
}
void MOPacInternals::UpdateAtoms(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	while (lFrame->NumAtoms*3 > Count) {	//More atoms, just append atoms to the end of the list
		AddAtom(MainData);
	}
	while (lFrame->NumAtoms*3 < Count) {
		DeleteAtom(MainData, lFrame->NumAtoms+1);
	}
}