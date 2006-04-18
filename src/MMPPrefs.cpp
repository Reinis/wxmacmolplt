/*
 *  (c) 2005 Iowa State University
 *      see the LICENSE file in the top level directory
 */
/* MacMolPlt preferences file parsing routines

	6/2005 - First pass converting from the old resource based stuff, BMB
*/

#include "Globals.h"
#include "MyTypes.h"
#include <vector>
#include "XML.hpp"
#include "Prefs.h"

#include <sstream>
#include <string.h>

#include "MMPPrefs.h"

#if !defined(CarbonBuild) && !defined(__wxBuild__)
extern short		gAppResRef;
#include "StdPrefsLib.h"
#endif
#ifdef __wxBuild__
#include <wx/stdpaths.h>
#endif

extern WinPrefs *	gPreferences;
extern WinPrefs *	gPrefDefaults;

//copy in the factory default values from the gPrefDefaults
void WinPrefs::ReadAtomDefaults(void) {
	CopyAtomPrefs(gPrefDefaults);
}
void WinPrefs::CopyAtomPrefs(WinPrefs * Orig) {
	memcpy((Ptr) AtomColors, Orig->AtomColors, kMaxAtomTypes*sizeof(RGBColor));
	memcpy((Ptr) AtomPatterns, Orig->AtomPatterns, kMaxAtomTypes*sizeof(short));
	memcpy((Ptr) AtomLabels, Orig->AtomLabels, kMaxAtomTypes*3*sizeof(char));
	memcpy((Ptr) AtomSizes, Orig->AtomSizes, kMaxAtomTypes*sizeof(int));
	memcpy((Ptr) AtomMasses, Orig->AtomMasses, kMaxAtomTypes*sizeof(float));
}
void WinPrefs::ReadBondDefaults(void) {
	for (int i=0; i<kMaxBondTypes; i++) {
		BondColors[i] = gPrefDefaults->BondColors[i];
		BondPatterns[i] = gPrefDefaults->BondPatterns[i];
	}
	VectorColor = gPrefDefaults->VectorColor;
	VectorPattern = gPrefDefaults->VectorPattern;
	RotateMode = gPrefDefaults->RotateMode;
	VectorScale = gPrefDefaults->VectorScale;
	AnimateMode = gPrefDefaults->AnimateMode;
}
void WinPrefs::CopyBondPrefs(WinPrefs * Orig) {
	memcpy(BondColors, Orig->GetBondColorLoc(0), 4*sizeof(RGBColor));
	memcpy(BondPatterns, Orig->GetBondPatternLoc(0), 4*sizeof(short));
	memcpy(&VectorColor, Orig->GetVectorColorLoc(), sizeof(short));
	memcpy(&VectorPattern, Orig->GetVectorPatternLoc(), sizeof(short));
	RotateMode = Orig->GetRotateMode();
	VectorScale = Orig->GetVectorScale();
	AnimateMode = Orig->GetAnimateMode();
}
void WinPrefs::CopyDisplayPrefs(WinPrefs * Orig) {
	DrawWireFrame(Orig->DrawWireFrame());
	ColorBondHalves(Orig->ColorBondHalves());
	OutLineBonds(Orig->OutLineBonds());
}
void WinPrefs::ReadDisplayDefaults(void) {
	CopyDisplayPrefs(gPrefDefaults);
}
void WinPrefs::ReadEnergyDefaults(void) {
	CopyEnergyPrefs(gPrefDefaults);
}
void WinPrefs::CopyEnergyPrefs(WinPrefs * Orig) {
	EnergyOptions * temp = Orig->GetEnergyOptions();
	EnergyPlotOptions = *temp;
	GraphOptions * tempPOpts = Orig->GetGraphOptions();
	PlotOptions = *tempPOpts;
}
void WinPrefs::ReadSurfaceDefaults(void) {
	CopySurfacePrefs(gPrefDefaults);
}
void WinPrefs::CopySurfacePrefs(WinPrefs * Orig) {
	SurfaceOptions * temp = Orig->GetSurfaceOptions();
	SurfaceOpts = *temp;
}
void WinPrefs::ReadFontDefaults(void) {
	CopyFontPrefs(gPrefDefaults);
}
void WinPrefs::CopyFontPrefs(WinPrefs * Orig) {
	LabelFontID = Orig->GetLabelFontID();
	LabelSize = Orig->GetLabelSize();
}
void WinPrefs::ReadFileDefaults(void) {
	CopyFilePrefs(gPrefDefaults);
}
void WinPrefs::CopyFilePrefs(WinPrefs * Orig) {
	AutoBondScale = Orig->GetAutoBondScale();
	AutoBondFlag = Orig->GetAutoBond();
	HHBondFlag = Orig->GetHHBondFlag();
	DetermineBondOrder(Orig->DetermineBondOrder());
	AllowHydrogenBonds(Orig->AllowHydrogenBonds());
	DRCnFileSkip = Orig->GetDRCSkip();
	SetPrompt4Save(Orig->GetPrompt4Save());
	SetShowAngles(Orig->GetShowAngles());
	ChangeFileType(Orig->ChangeFileType());
}
void WinPrefs::ReadStereoDefaults(void) {
	StereoOffset = gPrefDefaults->GetStereoOffset();
	UseStereo(gPrefDefaults->UseStereo());
}
void WinPrefs::ReadScalingDefaults(void) {
	CopyScalingPrefs(gPrefDefaults);
}
void WinPrefs::CopyScalingPrefs(WinPrefs * Orig) {
	AnimationSpeed = Orig->GetAnimationSpeed();
	AtomScale = Orig->GetAtomScale();
	ZScale = Orig->GetZScale();
	AnimateTime = Orig->GetAnimateTime();
}
void WinPrefs::ReadQD3DDefaults(void) {
	CopyQD3DPrefs(gPrefDefaults);
}
void WinPrefs::CopyQD3DPrefs(WinPrefs * Orig) {
	BondWidth = Orig->GetQD3DBondWidth();
	QD3DAtomQuality = Orig->GetQD3DAtomQuality();
	QD3DFillBrightness = Orig->GetQD3DFillBrightness();
	QD3DPointBrightness = Orig->GetQD3DPointBrightness();
	CylindersForLines(Orig->CylindersForLines());
	Orig->CylindersForLines(true);
	QD3DLineWidth = Orig->GetQD3DLineWidth();
	Orig->CylindersForLines(CylindersForLines());
	UseQD3DHardware(Orig->UseQD3DHardware());
	BackColor = Orig->BackColor;
}
void WinPrefs::GetAtomLabel(long AtomNum, Str255 text) {
	int		TextLength=0;
	while (AtomLabels[AtomNum][TextLength]) {
		text[1+TextLength] = AtomLabels[AtomNum][TextLength];
		TextLength++;
		if (TextLength >= 3) break;		//Maximum of 3 char labels
	}
	text[0] = TextLength;	//Copy the length byte
}
void WinPrefs::SetAtomLabel(long AtomNum, Str255 text) {
	int		TextLength = text[0], pos=0;
	if (TextLength > 3) TextLength = 3;		//truncate labels longer than 3 char
	for (; pos < TextLength; pos++) {	//I know, I know. This copies in reverse order, but its only 3 bytes
		AtomLabels[AtomNum][pos] = text[1+pos];
	}
	if (pos < 3) AtomLabels[AtomNum][pos] = '\0';
}
long WinPrefs::SetAtomSize(long AtomNum, long NewSize) {
	if ((NewSize >= 0) && (NewSize < 1000))	//Don't allow huge or negative atom sizes
		AtomSizes[AtomNum] = NewSize;
	return AtomSizes[AtomNum];
}
float WinPrefs::SetAtomMass(long AtomNum, float NewMass) {
	if ((NewMass > 0.0) && (NewMass < 1.0e5))
		AtomMasses[AtomNum] = sqrt(NewMass);	//Masses are stored as sqrt's
	return (AtomMasses[AtomNum]*AtomMasses[AtomNum]);
}
WinPrefs::WinPrefs(void) {
	RendererName = NULL;
	BitOptions = 0;
	BackColor.red = BackColor.green = BackColor.blue = 65532;
}
WinPrefs::~WinPrefs(void) {
	if (RendererName) delete [] RendererName;
}

long SurfaceOptions::SetNumContours(long NewNum) {
	if ((NewNum > 0)&&(NewNum < 1000)) NumContours = NewNum;
	return NumContours;
}
long SurfaceOptions::SetNumGridPoints(long NewNum) {
	if ((NewNum>10)&&(NewNum<300)) NumGridPoints = NewNum;
	return NumGridPoints;
}
float SurfaceOptions::SetGridSize(float NewSize) {
	if ((NewSize>0.0)&&(NewSize<10.0)) GridSize = NewSize;
	return GridSize;
}
float SurfaceOptions::SetMaxContour(float NewMax) {
	if (NewMax>0.0) MaxContourValue = NewMax;
	return MaxContourValue;
}
bool SurfaceOptions::SetShowZeroContour(bool State) {
	if (Options & 1) Options--;
	if (State) Options++;
	return (Options & 1);
}
bool SurfaceOptions::SetRotate2DMap(bool State) {
	if (Options & (1<<1)) Options-= (1<<1);
	if (State) Options+= (1<<1);
	return ((Options & (1<<1))?true:false);
}
bool SurfaceOptions::Set2DHashMarks(bool State) {
	if (Options & (1<<2)) Options-= (1<<2);
	if (State) Options+= (1<<2);
	return ((Options & (1<<2))?true:false);
}

void WinPrefs::ReadDefaultPrefs(void) {
	//Read the application default preferences
#if defined(__wxBuild__)
	wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
	wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
	//but xcode is putting them in Resources.
	pathname.Remove(pathname.Length() - 13);
	pathname += "Resources";
#endif
	pathname += wxT("/MacMolPlt.Prefs.xml");
	
	FILE * preffile = fopen(pathname.mb_str(wxConvUTF8), "r");
	if (preffile) {
		fseek(preffile, 0, SEEK_END);
		long ByteCount = ftell(preffile);
		fseek(preffile, 0, SEEK_SET);
		
		char * buffer = new char[ByteCount + 1];
		
		long test = fread(buffer, 1, ByteCount, preffile);
		buffer[ByteCount] = '\0';
		if (test != ByteCount) MessageAlert("Error reading the default prefs file. This is bad!");
		fclose(preffile);

		XMLSetup();
		
		XMLDocument * xDoc = NULL;
		try{
			xDoc = new XMLDocument(buffer, ByteCount, true);
			if (xDoc->parse()) {
				XMLElement * root = xDoc->getDocumentRoot();
				ReadMMPPrefs(root);
			}
			delete xDoc;
		}
		catch (...) {
			if (xDoc != NULL) delete xDoc;
			MessageAlert("XML Exception while parsing the default prefs file. This is bad!");
		}
		
		XMLShutdown();
	}
#elif defined(CarbonBuild)
	CFBundleRef myBundle = CFBundleGetMainBundle();
	CFURLRef tempURL = CFBundleCopyResourceURL (myBundle, CFSTR("MacMolPlt.Prefs.xml"),
									   NULL, NULL);
	FSRef myFSRef;
	OSErr myErr = fnfErr;
	if (CFURLGetFSRef (tempURL, &myFSRef)) {
		// Open the data fork now
		HFSUniStr255 dataFName;
		FSGetDataForkName(&dataFName);
		SInt16 fileRefNum;
		myErr = FSOpenFork(&myFSRef, dataFName.length, dataFName.unicode,
						   fsRdPerm, &fileRefNum);
		if (myErr == noErr) {
			long fsize;
			GetEOF(fileRefNum, &fsize);
			char * buffer = new char[fsize + 1];
			myErr = FSRead(fileRefNum, &fsize, buffer);
				//Done with the file
			FSCloseFork(fileRefNum);

			XMLSetup();
			
			XMLDocument * xDoc = NULL;
			try{
				xDoc = new XMLDocument(buffer, fsize, true);
				if (xDoc->parse()) {
					XMLElement * root = xDoc->getDocumentRoot();
					ReadMMPPrefs(root);
				}
				delete xDoc;
			}
			catch (...) {
				if (xDoc != NULL) delete xDoc;
				MessageAlert("\pXML Exception");
			}
			
			XMLShutdown();
		}
	}
	if (myErr!=noErr) {
		//hmm not being able to read the preferences from the app bundle is a bad sign
		MessageAlert("\pUnable to read application default preferences. This probably indicates a corrupt installation of MacMolPlt.");
	}
#else
		//For MacOS 9 we don't have a bundle so  read it out of a resource.
	Handle lpreference=NULL;
	OSErr result;
	//ok what resource type should I use for this?
	result = ReadPreference (gAppResRef, resourceType, &resourceID, &lpreference);
	if (result==noErr) {
		long size = GetHandleSize(lpreference);
		if (size>0) {
			char * buffer = new char[size + 1];
			BlockMoveData(*lpreference, buffer, size);
			XMLSetup();
			
			XMLDocument * xDoc = NULL;
			try{
				xDoc = new XMLDocument(buffer, fsize, true);
				if (xDoc->parse()) {
					XMLElement * root = xDoc->getDocumentRoot();
					ReadMMPPrefs(root);
				}
				delete xDoc;
			}
			catch (...) {
				if (xDoc != NULL) delete xDoc;
				MessageAlert("\pXML Exception");
			}
			
			XMLShutdown();
		}
	}
	if (lpreference) DisposeHandle(lpreference);
#endif
}

bool WinPrefs::ReadUserPrefs(void) {
	//locate the user preferences file in the users's preferences folder, read it and parse it
	bool result=false;

#ifdef __wxBuild__
	wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
	wxString pathname = gStdPaths.GetUserConfigDir();
#ifdef __UNIX__
	//The standarad unix path is the user's home dir. Thus the file should be "hidden".
	pathname += wxT("/.MacMolPlt.Prefs.xml");
#else
	pathname += wxT("/MacMolPlt.Prefs.xml");
#endif	
	FILE * preffile = fopen(pathname.mb_str(wxConvUTF8), "r");
	if (preffile) {
		fseek(preffile, 0, SEEK_END);
		long ByteCount = ftell(preffile);
		fseek(preffile, 0, SEEK_SET);
		
		char * buffer = new char[ByteCount + 1];
		
		long test = fread(buffer, 1, ByteCount, preffile);
		buffer[ByteCount] = '\0';
		if (test != ByteCount) {
			MessageAlert("Error reading the user prefs file.");
			return false;
		}
		fclose(preffile);
		
		XMLSetup();
		
		XMLDocument * xDoc = NULL;
		try{
			xDoc = new XMLDocument(buffer, ByteCount, true);
			if (xDoc->parse()) {
				XMLElement * root = xDoc->getDocumentRoot();
				ReadMMPPrefs(root);
			}
			delete xDoc;
		}
		catch (...) {
			if (xDoc != NULL) delete xDoc;
			MessageAlert("XML Exception while parsing the default prefs file. They may be corrupt!");
		}
		
		XMLShutdown();
	}
#else
	OSErr		myErr;
	short		prefVRefNum, FileRefNum;
	long		prefDirNum;
	FSSpec		prefsFSSpec;
	
	myErr = FindFolder(kOnSystemDisk, kPreferencesFolderType, kDontCreateFolder, &prefVRefNum, &prefDirNum);
	if (myErr == noErr) {
		myErr = FSMakeFSSpec(prefVRefNum, prefDirNum, "\pMacMolPlt.Prefs.xml", &prefsFSSpec);
		//FSMakeFSSpec will return fnfErr if the file does not exist
		if (myErr == noErr) {
			FInfo	myFInfo;
			HGetFInfo(prefVRefNum, prefDirNum, prefsFSSpec.name, &myFInfo);
			if (myFInfo.fdCreator == (OSType) 'BMBm') {
				myErr = FSpOpenDF(&prefsFSSpec, fsRdPerm, &FileRefNum);
				if (myErr == noErr) {
					long fsize;
					GetEOF(FileRefNum, &fsize);
					char * buffer = new char[fsize + 1];
					myErr = FSRead(FileRefNum, &fsize, buffer);
					//Done with the file
					FSClose(FileRefNum);
					
					XMLSetup();
					
					XMLDocument * xDoc = NULL;
					try{
						xDoc = new XMLDocument(buffer, fsize, true);
						if (xDoc->parse()) {
							XMLElement * root = xDoc->getDocumentRoot();
							ReadMMPPrefs(root);
						}
						delete xDoc;
							//getting means the file existed and was more or less valid
						result = true;
					}
					catch (...) {
						if (xDoc != NULL) delete xDoc;
						MessageAlert("\pXML Exception");
					}
					
					XMLShutdown();
				}
			}
		}
	}
#endif
	return result;
}

long WinPrefs::ReadMMPPrefs(XMLElement * root) {
	const char * val;
	long	longVal;
	float	floatVal;
	bool	boolVal;
	XMLElementList * children = root->getChildren();
	for (int i=0; i<children->length(); i++) {
		XMLElement * child = children->item(i);
		MMPPref_Element elName;
		MMPPref_convert(child->getName(), elName);
		switch (elName) {
			case MMPPref_MolDisplayPrefs:
			{
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_AnimateTime), longVal))
					SetAnimateTime(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_3DAtomQuality), longVal))
					SetQD3DAtomQuality(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_BondWidth), floatVal))
					SetQD3DBondWidth(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_AtomScale), floatVal))
					SetAtomScale(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_ZScale), floatVal))
					SetZScale(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_VectorScale), floatVal))
					SetVectorScale(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_3DFillBrightness), floatVal))
					SetQD3DFillBrightness(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_3DPointBrightness), floatVal))
					SetQD3DPointBrightness(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_3DLineWidth), floatVal))
					SetQD3DLineWidth(floatVal);
				val = child->getAttributeValue(MMPPref_convert(MMPMolDisplay_LabelFontID));
#ifdef __wxBuild__
#warning Need to handle the font pref
#else
				LabelFontID = systemFont;
				if (val) {
					long l = strlen(val);
					Str255 FontName, TestFontName;
					strncpy((char *)&(FontName[1]), val, l);
					FontName[0] = l;
					LabelFontID = FMGetFontFamilyFromName(FontName);
					FMGetFontFamilyName(LabelFontID, TestFontName);
					if (!(EqualString(FontName, TestFontName, false, false)))
						LabelFontID = systemFont;
				}
#endif
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_LabelSize), longVal))
					SetLabelSize(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_AnimationSpeed),longVal))
					SetAnimationSpeed(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_StereoOffset),longVal))
					SetStereoOffset(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_RotateMode), boolVal))
					SetRotateMode(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_AnimateMode), boolVal))
					SetAnimateMode(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_ShowAngles), boolVal))
					SetShowAngles(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_Stereo), boolVal))
					UseStereo(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_ColorHalfBonds), boolVal))
					ColorBondHalves(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_WireframeMode), boolVal))
					DrawWireFrame(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_OutlineBonds), boolVal))
					OutLineBonds(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_UseCylinders), boolVal))
					CylindersForLines(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_Use3DByDefault), boolVal))
					Default3DOn(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPMolDisplay_Use3DHardwareAccel), boolVal))
					UseQD3DHardware(boolVal);
				XMLElementList * molChildren = child->getChildren();
				for (int im=0; im<molChildren->length(); im++) {
					XMLElement * molchild = molChildren->item(im);
					MMPMolDisplayElments elName;
					MMPPref_convert(molchild->getName(), elName);
					switch (elName) {
						case MMPMolDisplay_BackColor:
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), floatVal))
								BackColor.red = (unsigned short)(floatVal*65535.0);
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), floatVal))
								BackColor.green = (unsigned short)(floatVal*65535.0);
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), floatVal))
								BackColor.blue = (unsigned short)(floatVal*65535.0);
							break;
						case MMPMolDisplay_VectorColor:
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), floatVal))
								VectorColor.red = (unsigned short)(floatVal*65535.0);
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), floatVal))
								VectorColor.green = (unsigned short)(floatVal*65535.0);
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), floatVal))
								VectorColor.blue = (unsigned short)(floatVal*65535.0);
							if (molchild->getAttributeValue(MMPPref_convert(MMPPref_Pattern), longVal))
								VectorPattern = longVal;
							break;
						case MMPMolDisplay_BondColor:
							if (molchild->getAttributeValue(MMPPref_convert(MMPMolDisplay_BondOrder), longVal)) {
								int bondOrder = longVal - 1;
								if ((bondOrder>=0)&&(bondOrder<kMaxBondTypes)) {
									if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), floatVal))
										BondColors[bondOrder].red = (unsigned short)(floatVal*65535.0);
									if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), floatVal))
										BondColors[bondOrder].green = (unsigned short)(floatVal*65535.0);
									if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), floatVal))
										BondColors[bondOrder].blue = (unsigned short)(floatVal*65535.0);
									if (molchild->getAttributeValue(MMPPref_convert(MMPPref_Pattern), longVal))
										BondPatterns[bondOrder] = longVal;
								}
							}
							break;
					}
				}
				delete molChildren;
			}
				break;
			case MMPPref_FileOptions:
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_SetCreator), boolVal))
					ChangeFileType(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_CustomIcon), boolVal))
					CreateCustomIcon(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_Prompt4Save), boolVal))
					SetPrompt4Save(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_EOLCharMode), boolVal))
					NativeEOLChar(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_SetBondOrder), boolVal))
					DetermineBondOrder(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_CreateHBonds), boolVal))
					AllowHydrogenBonds(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_DRCSkipPoint), longVal))
					SetDRCSkip(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_AutoBond), boolVal))
					SetAutoBond(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_AutoBondTolerance), floatVal))
					SetAutoBondScale(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_MaxBondLength), floatVal))
					SetMaxBondLength(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPFileOpt_PreventHHBonds), boolVal))
					SetHHBondFlag(boolVal);
				break;
			case MMPPref_AtomPrefs:
			{
				XMLElementList * atmList = child->getElementsByName(MMPPref_convert(MMPPref_AtomElement));
				for (int im=0; im<atmList->length(); im++) {
					XMLElement * atmchild = atmList->item(im);
					long atmNumber;
					if (atmchild->getAttributeValue(MMPPref_convert(MMPAtomAtt_AtomicNumber), atmNumber)) {
						atmNumber--;
						if ((atmNumber>=0)&&(atmNumber<kMaxAtomTypes)) {
							if (atmchild->getAttributeValue(MMPPref_convert(MMPAtomAtt_AtomicSize), longVal))
								AtomSizes[atmNumber] = longVal;
							if (atmchild->getAttributeValue(MMPPref_convert(MMPAtomAtt_AtomicMass), floatVal))
								AtomMasses[atmNumber] = floatVal;
							val = atmchild->getAttributeValue(MMPPref_convert(MMPAtomAtt_AtomLabel));
							AtomLabels[atmNumber][0] = AtomLabels[atmNumber][1] = AtomLabels[atmNumber][2] = '\0';
							if (val) {
								int j=0;
								for ( ; j<3; j++) {
									if (val[j] == '\0') break;
									AtomLabels[atmNumber][j] = val[j];
								}
							}
							XMLElementList * colorList = atmchild->getElementsByName(MMPPref_convert(MMPPref_ColorXML));
							if (colorList->length() > 0) {
								XMLElement * clr = colorList->item(0);
								if (clr->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), floatVal))
									AtomColors[atmNumber].red = (unsigned short)(floatVal*65535.0);
								if (clr->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), floatVal))
									AtomColors[atmNumber].green = (unsigned short)(floatVal*65535.0);
								if (clr->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), floatVal))
									AtomColors[atmNumber].blue = (unsigned short)(floatVal*65535.0);
								if (clr->getAttributeValue(MMPPref_convert(MMPPref_Pattern), longVal))
									AtomPatterns[atmNumber] = longVal;
							}
							delete colorList;
						}
					}
				}
				delete atmList;
			}
				break;
			case MMPPref_CoordWinPrefs:
				break;
			case MMPPref_BondsWinPrefs:
				break;
			case MMPPref_EnergyWinPrefs:
			{
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_y1Zero), floatVal))
					EnergyPlotOptions.SetY1Zero(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_y2Zero), floatVal))
					EnergyPlotOptions.SetY1Zero(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_yAdjustFactor), floatVal))
					EnergyPlotOptions.SetYAdjustment(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_indicatorSize), longVal))
					EnergyPlotOptions.SetEPlotPointSize(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_NumDigits), longVal))
					EnergyPlotOptions.SetNumDigits(longVal);
				XMLElementList * eChildren = child->getChildren();
				for (int im=0; im<eChildren->length(); im++) {
					XMLElement * echild = eChildren->item(im);
					MMPEnergyOptions elName;
					MMPPref_convert(echild->getName(), elName);
					RGBColor * clr;
					switch (elName) {
						case MMPEnergyOpt_TEColor:
							clr = EnergyPlotOptions.GetTEColor();
							break;
						case MMPEnergyOpt_MPColor:
							clr = EnergyPlotOptions.GetMPColor();
							break;
						case MMPEnergyOpt_KEColor:
							clr = EnergyPlotOptions.GetKEColor();
							break;
						case MMPEnergyOpt_PEColor:
							clr = EnergyPlotOptions.GetPEColor();
							break;
					}
					if (echild->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), floatVal))
						clr->red = (unsigned short)(floatVal*65535.0);
					if (echild->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), floatVal))
						clr->green = (unsigned short)(floatVal*65535.0);
					if (echild->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), floatVal))
						clr->blue = (unsigned short)(floatVal*65535.0);
				}
				delete eChildren;
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotTE), boolVal))
					EnergyPlotOptions.SetPlotEnergy(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotMP), boolVal))
					EnergyPlotOptions.SetPlotMPEnergy(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotKE), boolVal))
					EnergyPlotOptions.SetPlotKEnergy(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotPE), boolVal))
					EnergyPlotOptions.SetPlotPEnergy(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_Plotkcal), boolVal))
					if (boolVal) EnergyPlotOptions.SetDisplayUnits(kKCalPerMole);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotRMSGrad), boolVal))
					PlotOptions.SetPlotRMSGradient(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotMaxGrad), boolVal))
					PlotOptions.SetPlotMaxGradient(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotBondLength), boolVal))
					PlotOptions.SetPlotBondLength(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotBondAngle), boolVal))
					PlotOptions.SetPlotBondAngle(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_PlotDihedral), boolVal))
					PlotOptions.SetPlotDihedral(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_Atom1), longVal))
					PlotOptions.Set1stAtom(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_Atom2), longVal))
					PlotOptions.Set2ndAtom(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_Atom3), longVal))
					PlotOptions.Set3rdAtom(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPEnergyOpt_Atom4), longVal))
					PlotOptions.Set4thAtom(longVal);
			}
			case MMPPref_FreqWinPrefs:
				break;
			case MMPPref_SurfaceWinPrefs:
			{
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_NumContours), longVal))
					SurfaceOpts.SetNumContours(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_NumGridPoints), longVal))
					SurfaceOpts.SetNumGridPoints(longVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_GridSize), floatVal))
					SurfaceOpts.SetGridSize(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_MaxContourValue), floatVal))
					SurfaceOpts.SetMaxContour(floatVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_ShowZeroContour), boolVal))
					SurfaceOpts.SetShowZeroContour(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_Rotate2DMap), boolVal))
					SurfaceOpts.SetRotate2DMap(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_Show2DHashMarks), boolVal))
					SurfaceOpts.Set2DHashMarks(boolVal);
				if (child->getAttributeValue(MMPPref_convert(MMPSurfWin_ShowZeroContour), boolVal))
					SurfaceOpts.SetShowZeroContour(boolVal);
				XMLElementList * molChildren = child->getChildren();
				for (int im=0; im<molChildren->length(); im++) {
					XMLElement * molchild = molChildren->item(im);
					MMPSurfaceWinElements elName;
					MMPPref_convert(molchild->getName(), elName);
					RGBColor temp;
					if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), floatVal))
						temp.red = (unsigned short)(floatVal*65535.0);
					if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), floatVal))
						temp.green = (unsigned short)(floatVal*65535.0);
					if (molchild->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), floatVal))
						temp.blue = (unsigned short)(floatVal*65535.0);
					switch (elName) {
						case MMPSurfWin_PosColor:
							SurfaceOpts.SetPosColor(&temp);
							break;
						case MMPSurfWin_NegColor:
							SurfaceOpts.SetNegColor(&temp);
							break;
					}
				}
				delete molChildren;
			}
				break;
		}
	}
	return 1;
}
void WinPrefs::WriteUserPrefs(void) const {
#ifdef __wxBuild__
#warning Add code to write the user prefs!
#else
	// There seem to be a couple of ways to go with this. There is the old style FindFolder/
	// FSpec based routines where you directly interact with a file in the "preferences" directory
	// or there is the new Core Foundation CFPrefs routines that are intended to store key value
	// pairs automatically in the "right" place. Since what I really want is to simply store an
	// XML message in basically the same fashion as what I store in saved files I think it looks
	// easier to go with the older file based routine.
	
	OSErr		myErr;
	short		prefVRefNum, FileRefNum;
	long		prefDirNum;
	FSSpec		prefsFSSpec;
	
	myErr = FindFolder(kOnSystemDisk, kPreferencesFolderType, kDontCreateFolder, &prefVRefNum, &prefDirNum);
	if (myErr == noErr) {
		myErr = FSMakeFSSpec(prefVRefNum, prefDirNum, "\pMacMolPlt.Prefs.xml", &prefsFSSpec);
		//FSMakeFSSpec will return fnfErr if the file does not exist
		if (myErr == noErr) {
			FInfo	myFInfo;
			HGetFInfo(prefVRefNum, prefDirNum, prefsFSSpec.name, &myFInfo);
			if ((myFInfo.fdCreator != (OSType) 'BMBm')||(myFInfo.fdType != (OSType) 'Pref')) {
				FSpDelete(&prefsFSSpec);
				myErr = fnfErr;
			}
		}
		if (myErr == fnfErr) {	//Create a new file if necessary
			myErr = FSpCreate(&prefsFSSpec, 'BMBm', 'Pref', smSystemScript);
			if (myErr != noErr) {
				MessageAlertByID(kerrstrings, 24);
				return;
			}
		}
		myErr = FSpOpenDF(&prefsFSSpec, fsRdWrPerm, &FileRefNum);
		myErr = SetEOF(FileRefNum, 0);
	}
	if (myErr != noErr) {
		MessageAlertByID(kerrstrings, 24);
		return;
	}

	XMLSetup();
	XMLDocument * xDoc = new XMLDocument(MMPPref_convert(MMPPref_Wrapper), true, "MacMolPlt");
	XMLElement * xmlRoot = xDoc->getDocumentRoot();
	
	WriteMMPPrefs(xmlRoot);
	
	std::ostringstream Preftext;
	Preftext << *xDoc;
	delete xDoc;
	XMLShutdown();
	long length = Preftext.str().length();
	FSWrite(FileRefNum, &length, Preftext.str().c_str());
	FSClose(FileRefNum);
#endif
}

long WinPrefs::WriteMMPPrefs(XMLElement * root) const {
	std::ostringstream outbuf;
	XMLElement * molElement = root->addChildElement(MMPPref_convert(MMPPref_MolDisplayPrefs));

	outbuf << GetAnimateTime();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_AnimateTime), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetQD3DAtomQuality();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_3DAtomQuality), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetQD3DBondWidth();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_BondWidth), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetAtomScale();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_AtomScale), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetZScale();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_ZScale), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetVectorScale();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_VectorScale), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetQD3DFillBrightness();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_3DFillBrightness), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetQD3DPointBrightness();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_3DPointBrightness), outbuf.str().c_str());
	outbuf.str("");
	outbuf << QD3DLineWidth;
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_3DLineWidth), outbuf.str().c_str());
#ifdef __wxBuild__
#warning Need to handle the font pref
#else
	Str255 FontName;
#ifdef CarbonBuild
	FMGetFontFamilyName(GetLabelFontID(), FontName);
#else
	GetFontName(GetLabelFontID(), FontName);	/* Store the font name -- not the ID */
#endif
	if (FontName[0] > 0) {
		FontName[FontName[0]+1] = '\0';
		molElement->addAttribute(MMPPref_convert(MMPMolDisplay_LabelFontID), (char *)&(FontName[1]));
	}
#endif
	outbuf.str("");
	outbuf << GetLabelSize();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_LabelSize), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetAnimationSpeed();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_AnimationSpeed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetStereoOffset();
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_StereoOffset), outbuf.str().c_str());
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_RotateMode), (GetRotateMode()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_AnimateMode), (GetAnimateMode()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_ShowAngles), (GetShowAngles()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_Stereo), (UseStereo()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_ColorHalfBonds), (ColorBondHalves()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_WireframeMode), (DrawWireFrame()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_OutlineBonds), (OutLineBonds()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_UseCylinders), (CylindersForLines()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_Use3DByDefault), (Default3DOn()?trueXML:falseXML));
	molElement->addAttribute(MMPPref_convert(MMPMolDisplay_Use3DHardwareAccel), (UseQD3DHardware()?trueXML:falseXML));
	
	XMLElement * color = molElement->addChildElement(MMPPref_convert(MMPMolDisplay_BackColor));
	outbuf.str("");
	outbuf << (BackColor.red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (BackColor.green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (BackColor.blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());

	color = molElement->addChildElement(MMPPref_convert(MMPMolDisplay_VectorColor));
	outbuf.str("");
	outbuf << (VectorColor.red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (VectorColor.green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (VectorColor.blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	outbuf.str("");
	outbuf << VectorPattern;
	color->addAttribute(MMPPref_convert(MMPPref_Pattern), outbuf.str().c_str());
	
	for (int i=0; i<kMaxBondTypes; i++) {
		color = molElement->addChildElement(MMPPref_convert(MMPMolDisplay_BondColor));
		outbuf.str("");
		outbuf << (i+1);
		color->addAttribute(MMPPref_convert(MMPMolDisplay_BondOrder), outbuf.str().c_str());
		outbuf.str("");
		outbuf << (BondColors[i].red/65535.0);
		color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
		outbuf.str("");
		outbuf << (BondColors[i].green/65535.0);
		color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
		outbuf.str("");
		outbuf << (BondColors[i].blue/65535.0);
		color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
		outbuf.str("");
		outbuf << BondPatterns[i];
		color->addAttribute(MMPPref_convert(MMPPref_Pattern), outbuf.str().c_str());
	}
// File options	
	XMLElement * fileElement = root->addChildElement(MMPPref_convert(MMPPref_FileOptions));
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_SetCreator), (ChangeFileType()?trueXML:falseXML));
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_CustomIcon), (CreateCustomIcon()?trueXML:falseXML));
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_Prompt4Save), (GetPrompt4Save()?trueXML:falseXML));
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_EOLCharMode), (NativeEOLChar()?trueXML:falseXML));
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_SetBondOrder), (DetermineBondOrder()?trueXML:falseXML));
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_CreateHBonds), (AllowHydrogenBonds()?trueXML:falseXML));
	outbuf.str("");
	outbuf << GetDRCSkip();
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_DRCSkipPoint), outbuf.str().c_str());
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_AutoBond), (GetAutoBond()?trueXML:falseXML));
	outbuf.str("");
	outbuf << GetAutoBondScale();
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_AutoBondTolerance), outbuf.str().c_str());
	outbuf.str("");
	outbuf << GetMaxBondLength();
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_MaxBondLength), outbuf.str().c_str());
	fileElement->addAttribute(MMPPref_convert(MMPFileOpt_PreventHHBonds), (GetHHBondFlag()?trueXML:falseXML));
	// Atom options
	XMLElement * AtomArrayElement = root->addChildElement(MMPPref_convert(MMPPref_AtomPrefs));
	for (int i=0; i<kMaxAtomTypes; i++) {
		XMLElement * atm = AtomArrayElement->addChildElement(MMPPref_convert(MMPPref_AtomElement));
		outbuf.str("");
		outbuf << (i+1);
		atm->addAttribute(MMPPref_convert(MMPAtomAtt_AtomicNumber), outbuf.str().c_str());
		outbuf.str("");
		outbuf << AtomSizes[i];
		atm->addAttribute(MMPPref_convert(MMPAtomAtt_AtomicSize), outbuf.str().c_str());
		outbuf.str("");
		outbuf << AtomMasses[i];
		atm->addAttribute(MMPPref_convert(MMPAtomAtt_AtomicMass), outbuf.str().c_str());
		outbuf.str("");
		int j = 0;
		for ( ; j<3; j++) {
			if (AtomLabels[i][j] == '\0') break;
			outbuf << AtomLabels[i][j];
		}
		if (j>0) atm->addAttribute(MMPPref_convert(MMPAtomAtt_AtomLabel), outbuf.str().c_str());
		XMLElement * color = atm->addChildElement(MMPPref_convert(MMPPref_ColorXML));
			//Add the color, note the xml file will always use the GL style float values
		outbuf.str("");
		outbuf << (AtomColors[i].red/65535.0);
		color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
		outbuf.str("");
		outbuf << (AtomColors[i].green/65535.0);
		color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
		outbuf.str("");
		outbuf << (AtomColors[i].blue/65535.0);
		color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
		outbuf.str("");
		outbuf << AtomPatterns[i];
		color->addAttribute(MMPPref_convert(MMPPref_Pattern), outbuf.str().c_str());
	}

//	XMLElement * CWinlement = root->addChildElement(MMPPref_convert(MMPPref_CoordWinPrefs));
	
//	XMLElement * BWinElement = root->addChildElement(MMPPref_convert(MMPPref_BondsWinPrefs));

	XMLElement * EWinElement = root->addChildElement(MMPPref_convert(MMPPref_EnergyWinPrefs));
	outbuf.str("");
	outbuf << EnergyPlotOptions.GetY1Zero();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_y1Zero), outbuf.str().c_str());
	outbuf.str("");
	outbuf << EnergyPlotOptions.GetY2Zero();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_y2Zero), outbuf.str().c_str());
	outbuf.str("");
	outbuf << EnergyPlotOptions.GetYAdjustment();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_yAdjustFactor), outbuf.str().c_str());
	outbuf.str("");
	outbuf << EnergyPlotOptions.GetEPlotPointSize();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_indicatorSize), outbuf.str().c_str());
	outbuf.str("");
	outbuf << EnergyPlotOptions.GetNumDigits();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_NumDigits), outbuf.str().c_str());
	color = EWinElement->addChildElement(MMPPref_convert(MMPEnergyOpt_TEColor));
	const RGBColor * clr;
	clr = EnergyPlotOptions.GetTEColorPtr();
	outbuf.str("");
	outbuf << (clr->red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	color = EWinElement->addChildElement(MMPPref_convert(MMPEnergyOpt_MPColor));
	clr = EnergyPlotOptions.GetMPColorPtr();
	outbuf.str("");
	outbuf << (clr->red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	color = EWinElement->addChildElement(MMPPref_convert(MMPEnergyOpt_KEColor));
	clr = EnergyPlotOptions.GetKEColorPtr();
	outbuf.str("");
	outbuf << (clr->red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	color = EWinElement->addChildElement(MMPPref_convert(MMPEnergyOpt_PEColor));
	clr = EnergyPlotOptions.GetPEColorPtr();
	outbuf.str("");
	outbuf << (clr->red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (clr->blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotTE), (EnergyPlotOptions.PlotEnergy()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotMP), (EnergyPlotOptions.PlotMPEnergy()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotKE), (EnergyPlotOptions.PlotKEnergy()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotPE), (EnergyPlotOptions.PlotPEnergy()?trueXML:falseXML));
	//ok this may not be the right way to deal with units.
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_Plotkcal), (EnergyPlotOptions.GetDisplayUnits()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotRMSGrad), (PlotOptions.PlotRMSGradient()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotMaxGrad), (PlotOptions.PlotMaxGradient()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotBondLength), (PlotOptions.PlotBondLength()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotBondAngle), (PlotOptions.PlotBondAngle()?trueXML:falseXML));
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_PlotDihedral), (PlotOptions.PlotDihedral()?trueXML:falseXML));
	outbuf.str("");
	outbuf << PlotOptions.Get1stAtom();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_Atom1), outbuf.str().c_str());
	outbuf.str("");
	outbuf << PlotOptions.Get2ndAtom();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_Atom2), outbuf.str().c_str());
	outbuf.str("");
	outbuf << PlotOptions.Get3rdAtom();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_Atom3), outbuf.str().c_str());
	outbuf.str("");
	outbuf << PlotOptions.Get4thAtom();
	EWinElement->addAttribute(MMPPref_convert(MMPEnergyOpt_Atom4), outbuf.str().c_str());
	
//	XMLElement * FWinElement = root->addChildElement(MMPPref_convert(MMPPref_FreqWinPrefs));

	XMLElement * SWinElement = root->addChildElement(MMPPref_convert(MMPPref_SurfaceWinPrefs));
	outbuf.str("");
	outbuf << SurfaceOpts.GetNumContours();
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_NumContours), outbuf.str().c_str());
	outbuf.str("");
	outbuf << SurfaceOpts.GetNumGridPoints();
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_NumGridPoints), outbuf.str().c_str());
	outbuf.str("");
	outbuf << SurfaceOpts.GetGridSize();
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_GridSize), outbuf.str().c_str());
	outbuf.str("");
	outbuf << SurfaceOpts.GetMaxContour();
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_MaxContourValue), outbuf.str().c_str());
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_ShowZeroContour), (SurfaceOpts.GetShowZeroContour()?trueXML:falseXML));
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_Rotate2DMap), (SurfaceOpts.GetRotate2DMap()?trueXML:falseXML));
	SWinElement->addAttribute(MMPPref_convert(MMPSurfWin_Show2DHashMarks), (SurfaceOpts.Get2DHashMarks()?trueXML:falseXML));
	color = SWinElement->addChildElement(MMPPref_convert(MMPSurfWin_PosColor));
	RGBColor temp;
	SurfaceOpts.GetPosColor(&temp);
	outbuf.str("");
	outbuf << (temp.red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (temp.green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (temp.blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	color = SWinElement->addChildElement(MMPPref_convert(MMPSurfWin_NegColor));
	SurfaceOpts.GetNegColor(&temp);
	outbuf.str("");
	outbuf << (temp.red/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (temp.green/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), outbuf.str().c_str());
	outbuf.str("");
	outbuf << (temp.blue/65535.0);
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), outbuf.str().c_str());
	
	return 1;
}

#pragma mark -
const char * MMPPref_convert(MMPPref_Element t)
{       
    switch(t)
    {   
        case MMPPref_Wrapper:
            return "MacMolPltPreferences";
        case MMPPref_MolDisplayPrefs:
            return "MoleculeDisplayPreferences";
        case MMPPref_FileOptions:
            return "FileOptions";
        case MMPPref_AtomPrefs:
            return "AtomPreferences";
        case MMPPref_AtomElement:
            return "Atom";
		case MMPPref_CoordWinPrefs:
            return "CoordinateWindowPreferences";
        case MMPPref_BondsWinPrefs:
            return "BondWindowPreferences";
        case MMPPref_EnergyWinPrefs:
            return "EnergyWindowPreferences";
        case MMPPref_FreqWinPrefs:
            return "FrequencyWindowPreferences";
		case MMPPref_SurfaceWinPrefs:
            return "SurfaceWindowOptions";
		default:
            return "invalid";
    }
}  

const char * MMPPref_convert(MMPPref_Color t)
{       
    switch(t)
    {   
        case MMPPref_ColorXML:
            return "Color";
        case MMPPref_ColorRed:
            return "red";
        case MMPPref_ColorGreen:
            return "green";
        case MMPPref_ColorBlue:
            return "blue";
        case MMPPref_Pattern:
            return "pattern";
		default:
            return "invalid";
    }
}  
const char * MMPPref_convert(MMPref_WinRect t)
{       
    switch(t)
    {   
        case MMPPref_Rect_top:
            return "top";
        case MMPPref_Rect_left:
            return "left";
        case MMPPref_Rect_bottom:
            return "bottom";
        case MMPPref_Rect_right:
            return "right";
        case MMPPref_Window_Open:
            return "windowOpen";
		default:
            return "invalid";
    }
}  

const char * MMPPref_convert(MMPFileOpts t)
{       
    switch(t)
    {   
        case MMPFileOpt_SetCreator:
            return "SetFileCreator";
        case MMPFileOpt_CustomIcon:
            return "CreateCustomIcon";
        case MMPFileOpt_Prompt4Save:
            return "PromptToSaveChanges";
        case MMPFileOpt_EOLCharMode:
            return "UseNativeEOLChars";
		case MMPFileOpt_SetBondOrder:
            return "SetBondOrder";
        case MMPFileOpt_CreateHBonds:
            return "CreateHydrogenBonds";
        case MMPFileOpt_DRCSkipPoint:
            return "ReadEverynDRCPoints";
        case MMPFileOpt_AutoBond:
            return "AutoCreateBonds";
        case MMPFileOpt_AutoBondTolerance:
            return "AutoBondTolerance";
        case MMPFileOpt_MaxBondLength:
            return "MaxBondLength";
        case MMPFileOpt_PreventHHBonds:
            return "PreventHHBonds";
		default:
            return "invalid";
    }
}  

const char * MMPPref_convert(MMPAtomAttribs t)
{       
    switch(t)
    {   
        case MMPAtomAtt_AtomicNumber:
            return "Number";
        case MMPAtomAtt_AtomicSize:
            return "Size";
        case MMPAtomAtt_AtomicMass:
            return "Mass";
		case MMPAtomAtt_AtomLabel:
            return "Label";
		default:
            return "invalid";
    }
}  

const char * MMPPref_convert(MMPMolDisplayElments t)
{       
    switch(t)
    {   
        case MMPMolDisplay_BackColor:
            return "BackgroundColor";
        case MMPMolDisplay_VectorColor:
            return "VectorColor";
        case MMPMolDisplay_BondColor:
            return "BondColor";
        case MMPMolDisplay_BondOrder:
            return "BondOrder";
        case MMPMolDisplay_AnimateTime:
            return "AnimationDelay";
		case MMPMolDisplay_3DAtomQuality:
            return "Atom3DQuality";
        case MMPMolDisplay_BondWidth:
            return "BondWidth";
        case MMPMolDisplay_AtomScale:
            return "AtomScale";
        case MMPMolDisplay_ZScale:
            return "DepthCue";
        case MMPMolDisplay_VectorScale:
            return "VectorScale";
        case MMPMolDisplay_3DFillBrightness:
            return "FillBrightness3D";
        case MMPMolDisplay_3DPointBrightness:
            return "PointBrightness3D";
        case MMPMolDisplay_3DLineWidth:
            return "LineWidth3D";
        case MMPMolDisplay_LabelFontID:
            return "LabelFontId";
        case MMPMolDisplay_LabelSize:
            return "LabelSize";
        case MMPMolDisplay_AnimationSpeed:
            return "AnimationSpeed";
        case MMPMolDisplay_StereoOffset:
            return "StereoOffset";
        case MMPMolDisplay_RotateMode:
            return "RotateMode";
        case MMPMolDisplay_AnimateMode:
            return "AnimateMode";
        case MMPMolDisplay_ShowAngles:
            return "ShowAngles";
        case MMPMolDisplay_Stereo:
            return "StereoActive";
        case MMPMolDisplay_ColorHalfBonds:
            return "ColorBondHalves";
        case MMPMolDisplay_WireframeMode:
            return "WireframeMode";
        case MMPMolDisplay_OutlineBonds:
            return "OutlineBonds";
        case MMPMolDisplay_UseCylinders:
            return "UseCylinders";
        case MMPMolDisplay_Use3DByDefault:
            return "Use3DByDefault";
        case MMPMolDisplay_Use3DHardwareAccel:
            return "Use3DHardware";
		default:
            return "invalid";
    }
}  

const char * MMPPref_convert(MMPSurfaceWinElements t)
{       
    switch(t)
    {   
        case MMPSurfWin_Rect:
            return "WindowRect";
        case MMPSurfWin_NumContours:
            return "NumberContours";
        case MMPSurfWin_NumGridPoints:
            return "NumberGridPoints";
        case MMPSurfWin_GridSize:
            return "GridSize";
		case MMPSurfWin_MaxContourValue:
            return "MaximumContourValue";
        case MMPSurfWin_PosColor:
            return "PositiveContourColor";
        case MMPSurfWin_NegColor:
            return "NegativeContourColor";
        case MMPSurfWin_ShowZeroContour:
            return "ShowZeroContour";
        case MMPSurfWin_Rotate2DMap:
            return "Rotate2DMap";
        case MMPSurfWin_Show2DHashMarks:
            return "Show2DHashMarks";
		default:
            return "invalid";
    }
}  

const char * MMPPref_convert(MMPEnergyOptions t)
{       
    switch(t)
    {   
        case MMPEnergyOpt_y1Zero:
            return "y1Zero";
        case MMPEnergyOpt_y2Zero:
            return "y2Zero";
        case MMPEnergyOpt_yAdjustFactor:
            return "YAdjustmentFactor";
        case MMPEnergyOpt_xAdjustFactor:
            return "XAdjustmentFactor";
		case MMPEnergyOpt_indicatorSize:
            return "IndicatorSize";
        case MMPEnergyOpt_NumDigits:
            return "NumberDigits";
        case MMPEnergyOpt_TEColor:
            return "TEColor";
        case MMPEnergyOpt_MPColor:
            return "MPColor";
        case MMPEnergyOpt_KEColor:
            return "KEColor";
        case MMPEnergyOpt_PEColor:
            return "PEColor";
        case MMPEnergyOpt_PlotTE:
            return "PlotTotalEnergy";
        case MMPEnergyOpt_PlotMP:
            return "PlotMPEnergy";
        case MMPEnergyOpt_PlotKE:
            return "PlotKineticEnergy";
        case MMPEnergyOpt_PlotPE:
            return "PlotPotentialEnergy";
        case MMPEnergyOpt_Plotkcal:
            return "UseKCal";
        case MMPEnergyOpt_PlotRMSGrad:
            return "PlotRMSGradient";
        case MMPEnergyOpt_PlotMaxGrad:
            return "PlotMaximumGradient";
        case MMPEnergyOpt_PlotBondLength:
            return "PlotBondLength";
        case MMPEnergyOpt_PlotBondAngle:
            return "PlotBondAngle";
        case MMPEnergyOpt_PlotDihedral:
            return "PlotDihedralAngle";
        case MMPEnergyOpt_Atom1:
            return "Atom1";
        case MMPEnergyOpt_Atom2:
            return "Atom2";
        case MMPEnergyOpt_Atom3:
            return "Atom3";
        case MMPEnergyOpt_Atom4:
            return "Atom4";
		default:
            return "invalid";
    }
}  

#define MATCH(s, t, j, k, type)                                            \
if (!s || !*s) return false;                                             \
for (int i = (int) j; i != (int) k; ++i)                                 \
{                                                                        \
    if (strcmp(s, MMPPref_convert((type) i)) == 0)                             \
    {                                                                      \
		t = (type) i;                                                        \
			return true;                                                     \
    }                                                                      \
}                                                                        \
return false;                                                            \

bool MMPPref_convert(const char * s, MMPPref_Element & t)
{
    MATCH(s, t, 0, NumberMMPPrefElements, MMPPref_Element)
}
bool MMPPref_convert(const char * s, MMPPref_Color & t)
{
    MATCH(s, t, 0, NumberMMPColorElements, MMPPref_Color)
}
bool MMPPref_convert(const char * s, MMPref_WinRect & t)
{
    MATCH(s, t, 0, NumberMMPPrefWinRectElements, MMPref_WinRect)
}
bool MMPPref_convert(const char * s, MMPFileOpts & t)
{
    MATCH(s, t, 0, NumberMMPFileOpts, MMPFileOpts)
}
bool MMPPref_convert(const char * s, MMPAtomAttribs & t)
{
    MATCH(s, t, 0, NumberMMPAtomAttribs, MMPAtomAttribs)
}
bool MMPPref_convert(const char * s, MMPMolDisplayElments & t)
{
    MATCH(s, t, 0, NumberMMPMolDisplayElements, MMPMolDisplayElments)
}
bool MMPPref_convert(const char * s, MMPSurfaceWinElements & t)
{
    MATCH(s, t, 0, NumberMMPSurfWinElements, MMPSurfaceWinElements)
}
bool MMPPref_convert(const char * s, MMPEnergyOptions & t)
{
    MATCH(s, t, 0, NumberMMPEnergyOptions, MMPEnergyOptions)
}