/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * MolDisplayWin.cpp
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "MolDisplayWin.h"
#include "Prefs.h"
#include "main.h"
#include "Frame.h"
#include "Math3D.h"
#include "myFiles.h"
#include "setscreenplane.h"
#include "bondsdlg.h"

extern WinPrefs * gPreferences;

//I think these are only needed in this file.
//They are custom ids used to tie the event table to the menus
enum MMP_EventID {
	MMP_SHRINK10=wxID_HIGHEST+1,
	MMP_ENLARGE10,
	MMP_SHOWMODE,
	MMP_SHOWAXIS,
	MMP_CENTER,
	MMP_ROTATESUBMENU,
	MMP_ROTATETOXAXIS,
	MMP_ROTATETOYAXIS,
	MMP_ROTATETOZAXIS,
	MMP_ROTATE180HOR,
	MMP_ROTATE180VER,
	MMP_ROTATEPRINC,
	MMP_ROTATEOTHER,
	MMP_CONVERTTOBOHR,
	MMP_CONVERTTOANGSTROMS,
	MMP_INVERTNORMALMODE,
	MMP_BONDSWINDOW,
	
	Number_MMP_Ids
};

BEGIN_EVENT_TABLE(MolDisplayWin, wxFrame)
#ifndef __WXMAC__
	EVT_MENU (wxID_OPEN,			MolDisplayWin::menuFileOpen)
#endif
	EVT_MENU (wxID_SAVE,			MolDisplayWin::menuFileSave)
    EVT_MENU (wxID_SAVEAS,			MolDisplayWin::menuFileSave_as)
	EVT_MENU (wxID_CLOSE,			MolDisplayWin::menuFileClose)
	EVT_CLOSE(						MolDisplayWin::FileClose)
    EVT_MENU (wxID_PRINT_SETUP,		MolDisplayWin::menuFilePage_setup)
    EVT_MENU (wxID_PREVIEW,			MolDisplayWin::menuFilePrint_preview)
    EVT_MENU (wxID_PRINT,			MolDisplayWin::menuFilePrint)

    EVT_MENU (wxID_UNDO,			MolDisplayWin::menuEditUndo)
    EVT_MENU (wxID_CUT,				MolDisplayWin::menuEditCut)
    EVT_MENU (wxID_COPY,			MolDisplayWin::menuEditCopy)
    EVT_MENU (wxID_PASTE,			MolDisplayWin::menuEditPaste)
    EVT_MENU (wxID_CLEAR,			MolDisplayWin::menuEditClear)
    EVT_MENU (wxID_SELECTALL,		MolDisplayWin::menuEditSelect_all)

	EVT_MENU (MMP_SHOWMODE,			MolDisplayWin::menuViewShowNormalMode)
	EVT_MENU (MMP_SHOWAXIS,			MolDisplayWin::menuViewShowAxis)
	EVT_MENU (MMP_SHRINK10,			MolDisplayWin::menuViewShrink_10)
	EVT_MENU (MMP_ENLARGE10,		MolDisplayWin::menuViewEnlarge_10)
	EVT_MENU (MMP_CENTER,			MolDisplayWin::menuViewCenter)
	EVT_MENU (MMP_ROTATETOXAXIS,	MolDisplayWin::menuViewRotateTo_X_axis)
	EVT_MENU (MMP_ROTATETOYAXIS,	MolDisplayWin::menuViewRotateTo_Y_axis)
	EVT_MENU (MMP_ROTATETOZAXIS,	MolDisplayWin::menuViewRotateTo_Z_axis)
	EVT_MENU (MMP_ROTATE180HOR,		MolDisplayWin::menuViewRotate180_horizontal)
	EVT_MENU (MMP_ROTATE180VER,		MolDisplayWin::menuViewRotate180_vertical)
	EVT_MENU (MMP_ROTATEPRINC,		MolDisplayWin::menuViewRotatePrinciple_orientation)
	EVT_MENU (MMP_ROTATEOTHER,		MolDisplayWin::menuViewRotateOther)

	EVT_MENU (MMP_CONVERTTOBOHR,	MolDisplayWin::menuMoleculeConvertToBohr)
	EVT_MENU (MMP_CONVERTTOANGSTROMS,	MolDisplayWin::menuMoleculeConvertToAngstroms)
	EVT_MENU (MMP_INVERTNORMALMODE,	MolDisplayWin::menuMoleculeInvertNormalMode)

	EVT_MENU (MMP_BONDSWINDOW,	MolDisplayWin::menuWindowBonds)

	EVT_CHAR (MolDisplayWin::KeyHandler)
END_EVENT_TABLE()

MolDisplayWin::MolDisplayWin(const wxString &title,
                         const wxPoint  &position,
                         const wxSize   &size,
                         long            style,
                         const wxString &name)
            :wxFrame((wxWindow *)NULL, wxID_ANY, title,
                     position, size, style, name) {

    createMenuBar();
    SetMenuBar(menuBar);

    // TODO:  Make room for other controls (Frame picker, etc.)
    MainData = new MoleculeData;
	Prefs = new WinPrefs;
	*Prefs = *gPreferences;
	Dirty = false;
	OperationInProgress = false;
	ProgressInd = NULL;
	OpenGLData = NULL;
	bondsWindow = NULL;
	InitGLData();
	
    glCanvas = new MpGLCanvas(this);
	glCanvas->setPrefs(Prefs);
	
    Show(true);
}

MolDisplayWin::~MolDisplayWin() {
    // TODO:  Destroy any dialogs that are still in existence.

	if (ProgressInd != NULL) {
		delete ProgressInd;
		ProgressInd = NULL;
	}
	
    if(MainData != NULL) {
        glCanvas->setMolData(NULL);
        delete MainData;
    }
	if (Prefs != NULL) {
		delete Prefs;
	}
}

void MolDisplayWin::createMenuBar(void) {
    menuBar = new wxMenuBar;

    menuFile = new wxMenu;
    menuEdit = new wxMenu;
    menuView = new wxMenu;
    menuMolecule = new wxMenu;
    menuWindow = new wxMenu;
    menuHelp = new wxMenu;

    // TODO:  Create IDs for custom menu items
    menuFile->Append(wxID_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"));
    menuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
    menuFile->Append(wxID_SAVEAS, wxT("Save &as ...\tCtrl+Shift+S"));
    menuFile->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
    menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Append New Frame"));
    //menuFile->Append(, wxT("Add Frames from File ..."));
    //menuFile->Append(, wxT("Delete Frame"));
    //menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Import"));
    //menuFile->Append(, wxT("Export"));
    //menuFile->AppendSeparator();
    menuFile->Append(wxID_PRINT_SETUP, wxT("Page Set&up ..."));
    menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view\tCtrl+Shift+P"));
    menuFile->Append(wxID_PRINT, wxT("&Print ...\tCtrl+P"));
    // TODO:  Make Mac display Quit menu item in the correct place
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
    
    menuEdit->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
    menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    //menuEdit->Append(,wxT("Copy Coordinates"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_PREFERENCES, wxT("Pr&eferences ..."));

    menuView->AppendCheckItem(MMP_SHOWMODE, wxT("Show &Normal Mode"));
    menuView->AppendCheckItem(MMP_SHOWAXIS, wxT("Show &Axis"));
    menuView->Append(MMP_SHRINK10, wxT("&Shrink 10%\tCtrl+-"));
    menuView->Append(MMP_ENLARGE10, wxT("&Enlarge 10%\tCtrl+="));
    menuView->Append(MMP_CENTER, wxT("&Center View"));
	
	menuViewRotate = new wxMenu;
    menuView->Append(MMP_ROTATESUBMENU, wxT("&Rotate ..."), menuViewRotate);

	menuViewRotate->Append(MMP_ROTATETOXAXIS, wxT("to &X-axis"));
	menuViewRotate->Append(MMP_ROTATETOYAXIS, wxT("to &Y-axis"));
	menuViewRotate->Append(MMP_ROTATETOZAXIS, wxT("to &Z-axis"));
	menuViewRotate->Append(MMP_ROTATE180HOR, wxT("180 degrees &Horizontal"));
	menuViewRotate->Append(MMP_ROTATE180VER, wxT("180 degrees &Vertical"));
	menuViewRotate->Append(MMP_ROTATEPRINC, wxT("to &Principle Orientation"));
	menuViewRotate->Append(MMP_ROTATEOTHER, wxT("&Other..."));

	menuMolecule->Append(MMP_CONVERTTOBOHR, wxT("Convert to &Bohr"));
    menuMolecule->Append(MMP_CONVERTTOANGSTROMS, wxT("Convert to &Angstroms"));
    menuMolecule->Append(MMP_INVERTNORMALMODE, wxT("&Invert Normal Mode"));
// TODO:  Create menu items for remaining menus

    menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"));
    // TODO:  Make Mac handle help menu properly
    // TODO:  Make Mac display About menu item in the correct place
    menuHelp->Append(wxID_ABOUT, wxT("&About ..."));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuMolecule, wxT("&Molecule"));
    menuBar->Append(menuWindow, wxT("&Subwindow"));
    menuBar->Append(menuHelp, wxT("&Help"));
}

void MolDisplayWin::ClearMenus(void) {
	menuFile->Enable(wxID_SAVE, false);
	
	menuEdit->Enable(wxID_UNDO, false);
	menuView->Enable(MMP_SHOWMODE, false);
	menuMolecule->Enable(MMP_INVERTNORMALMODE, false);
}
void MolDisplayWin::AdjustMenus(void) {
	ClearMenus();
	menuFile->Enable(wxID_SAVE, Dirty);
	
	if (MainData->cFrame->Vibs) {
		menuView->Enable(MMP_SHOWMODE, true);
		menuMolecule->Enable(MMP_INVERTNORMALMODE, true);
	}
}

/* Event handler functions */

/* File menu */
void MolDisplayWin::menuFileOpen(wxCommandEvent &event) {
	//Its possible we could handle this here if the current data is empty?
	//On the Mac Open always opens a new window
#ifndef __WXMAC__
	if (!Dirty && (MainData->NumFrames == 1) && (MainData->MaxAtoms == 0)) {
		//First need to use an open file dialog
		wxString filename = wxFileSelector(wxT("Choose a file to open"));
		//If the user chooses a file, create a window and have it process it.
		if (filename.length() > 0) {
	//		MolDisplayWin * temp = new MolDisplayWin(filename);
	//		MolWinList.push_back(temp);
			//Ok we have a problem. Abort open can't close the last window!
			long r = OpenFile(filename);
	//		if (r>0) temp->Show(true);
		}
	} else
#endif
		//otherwise just skip the event to pass it up the chain to the app handler
		event.Skip();
}

void MolDisplayWin::menuFileSave(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileSave_as(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileClose(wxCommandEvent &event) {
	Close();
}

void MolDisplayWin::FileClose(wxCloseEvent &event) {
	bool canVeto = event.CanVeto();
	//First we should check to see if a save is needed which could abort the close
	if (Dirty && Prefs->GetPrompt4Save()) {
		//prompt the user to see if they want the file saved
		//Note the message should be slightly different if we can't abort the close
		int style = wxYES_NO | wxICON_QUESTION;
		if (canVeto) style = style | wxCANCEL;
		int r = wxMessageBox(wxT("The file filename here has unsaved changes."),
							 wxT("Do you wish to save the current data and customizations before closing?"),
							 style, this);
		if (r == wxCANCEL) {
			event.Veto(true);
			return;
		} else if (r == wxYES) {
			//process the save
		}
	}
	MpApp & app = wxGetApp();
	//Once we decide to close the window it may be system dependant whether we 
	//leave an empty window up. On the Mac the window is always destroyed.
#ifndef __WXMAC__
/*	if ((app.WindowCount() <= 1) && canVeto) {	
		//This is the last window! Clear it out, but leave it open
		delete MainData;
		MainData = new MoleculeData;
		delete Prefs;
		Prefs = new WinPrefs;
		*Prefs = *gPreferences;
		Dirty = false;
		SetTitle(wxT("Untitled"));
		SetName(wxT("Untitled"));
		event.Veto(true);
		return;
	}*/
#endif
	Destroy();
	app.destroyMainFrame(this);
}

void MolDisplayWin::menuFilePage_setup(wxCommandEvent &event) {
}

void MolDisplayWin::menuFilePrint_preview(wxCommandEvent &event) {
}

void MolDisplayWin::menuFilePrint(wxCommandEvent &event) {
}

/* Edit menu */

void MolDisplayWin::menuEditUndo(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCut(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCopy(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditPaste(wxCommandEvent &event) {
}
void MolDisplayWin::PasteText(void) {
	//relax this restriction later (while in build mode)
	if (MainData->cFrame->NumAtoms != 0) return;	//Do not allow pasting if there are already atoms in this frame
//	if (gScrap.GetText()) {
		long		iline, test, Type;
		CPoint3D	Position, offset;
		
//		BufferFile * TextBuffer = new BufferFile(gScrap.GetText(), GetHandleSize(gScrap.GetText()));
//		if (!TextBuffer) return;
		
		if (MainData->NumFrames == 1) {	//If this is the only frame, make sure it is init'ed
			InitRotationMatrix(MainData->TotalRotation);
		}
		long NumLines =0;
	//	long NumLines = TextBuffer->GetNumLines(-1);
		// There may be up to NumLines atoms so dimension memory accordingly
		if (!MainData->SetupFrameMemory(NumLines, 0)) {
//			delete TextBuffer;
			return;
		}
		
		/*Now interpert each of the lines*/
		for (iline=0; iline < NumLines; iline++) {
			char LineText[kMaxLineLength];
			
//			TextBuffer->GetLine(LineText);
			//Parse the current line: All values will be set in place if the
			//parse is successful (test will return (-) if not). Normal modes
			//shouldn't be pasted so pass in 0 for Mode.
			test = ParseCartLine(LineText, &Type,&Position, &offset, 0);
			if (test > 0) {	/*something was wrong with this line so skip it*/
				
				//A special atom was entered, store its offset.
				if (Type > 115) {
					if (Type > 255)
						if (((Type - 255) < 1)||((Type - 255) > NumLines)) break;
					if (!MainData->cFrame->AddSpecialAtom(offset, iline)) break;
				}
				
				MainData->cFrame->AddAtom(Type, Position);
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.x));
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.y));
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.z));
			}
		}
		//Done with the text handle so unlock it
//		delete TextBuffer;
		
		if (iline == 0) {	/*No atoms were found so clear the memory I just allocated*/
//			MainData->ResetFrameMemory();
			return;
		}

//If there were special atoms found turn on drawing by default
		if (MainData->cFrame->SpecialAtoms)	MainData->SetSpecialAtomDrawMode(true);

		if (Prefs->GetAutoBond())
		MainData->cFrame->SetBonds(Prefs, false);
		//Now reset the display to take into account the new atoms
		MainData->CenterModelWindow();
		MainData->WindowSize = 2.0*MainData->MaxSize;
		MainData->ResetRotation();
		FrameChanged();
//	}
}

void MolDisplayWin::menuEditClear(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditSelect_all(wxCommandEvent &event) {
}
void MolDisplayWin::menuViewShowNormalMode(wxCommandEvent &event) {
	MainData->SetDrawMode(1-MainData->GetDrawMode());
	ResetModel(false);
	Dirty = true;
}
void MolDisplayWin::menuViewCenter(wxCommandEvent &event) {
	MainData->CenterModelWindow();
	ResetModel(false);
	Dirty = true;
}
void MolDisplayWin::menuViewShowAxis(wxCommandEvent &event) {
	MainData->SetShowAxis(1-MainData->ShowAxis());
	UpdateModelDisplay();
	Dirty = true;
}
void MolDisplayWin::menuViewShrink_10(wxCommandEvent &event) {
	MainData->WindowSize *= 1.1;
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewEnlarge_10(wxCommandEvent &event) {
	MainData->WindowSize *= 0.9;
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotateTo_X_axis(wxCommandEvent &event) {
	MainData->TotalRotation[0][2] = MainData->TotalRotation[1][1] = 1.0;
	MainData->TotalRotation[2][0] = -1.0;
	MainData->TotalRotation[0][0] = MainData->TotalRotation[2][2] =
		MainData->TotalRotation[1][0] = MainData->TotalRotation[0][1] =
		MainData->TotalRotation[1][2] = MainData->TotalRotation[2][1] = 0.0;
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotateTo_Y_axis(wxCommandEvent &event) {
	MainData->TotalRotation[0][0] = MainData->TotalRotation[1][2] = 1.0;
	MainData->TotalRotation[2][1] = -1.0;
	MainData->TotalRotation[1][1] = MainData->TotalRotation[2][2] =
		MainData->TotalRotation[0][1] = MainData->TotalRotation[0][2] =
		MainData->TotalRotation[1][0] = MainData->TotalRotation[2][0] = 0.0;
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotateTo_Z_axis(wxCommandEvent &event) {
	InitRotationMatrix(MainData->TotalRotation);
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotate180_horizontal(wxCommandEvent &event) {
	MainData->FlipRotation(0);
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotate180_vertical(wxCommandEvent &event) {
	MainData->FlipRotation(1);
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotatePrinciple_orientation(wxCommandEvent &event) {
	MessageAlert("Sure would be nice if this was implemented...");
}
void MolDisplayWin::menuViewRotateOther(wxCommandEvent &event) {
	SetScreenPlane * temp = new SetScreenPlane(this);
	temp->Show();
}
void MolDisplayWin::menuMoleculeConvertToBohr(wxCommandEvent &event) {
	MainData->UnitConversion(0);
	ResetAllWindows();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeConvertToAngstroms(wxCommandEvent &event) {
	MainData->UnitConversion(1);
	ResetAllWindows();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeInvertNormalMode(wxCommandEvent &event) {
	MainData->InvertMode();
	ResetModel(false);
	Dirty = true;
}
void MolDisplayWin::KeyHandler(wxKeyEvent & event) {
	event.Skip();
}
void MolDisplayWin::menuWindowBonds(wxCommandEvent &event) {
	if (bondsWindow) { //need to bring it to the front...
	} else {
		bondsWindow = new BondsDlg(this);
		bondsWindow->Show();
	}
}

void MolDisplayWin::BondsChanged(void) {
#warning Need to pass event to bonds list here
//	if (BondsDlog) BondsDlog->ResetList();
	FrameChanged();
};

//scan the new frame changing any mode bits for consistancy
void MolDisplayWin::FrameChanged(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * temp = MainData->cFrame->SurfaceList;
	while (temp) {
		temp->RotateEvent(MainData);
		temp = temp->GetNextSurface();
	}
	UpdateModelDisplay();
}
void MolDisplayWin::UpdateModelDisplay(void) {
#warning Activate the lines below when the functions are added
	//	DrawFrame();
	UpdateGLModel();
	glCanvas->draw();
}
void MolDisplayWin::ResetView(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
	glCanvas->UpdateGLView();
	glCanvas->draw();
}
void MolDisplayWin::ResetModel(bool Center) {
	if (Center) {
		MainData->CenterModelWindow();
		MainData->WindowSize = 2.0*(MainData->MaxSize+1.0);
	}
	MainData->ResetRotation();
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
	UpdateGLModel();
	// Reset the frame scroll bar
//	::SetControlMaximum(FrameScroll, MainData->NumFrames);
//	::SetControlValue(FrameScroll, MainData->CurrentFrame);
	glCanvas->draw();
	AdjustMenus();
}
void MolDisplayWin::ResetAllWindows(void) {
	glCanvas->UpdateGLView();
	ResetModel(false);
	//update the frame info.
	
	//force updates for all the child windows
	
}
void MolDisplayWin::BeginOperation(void) {
	ProgressInd = new Progress;
	if (!ProgressInd) throw MemoryError();
	OperationInProgress = true;
}
void MolDisplayWin::FinishOperation(void) {
	if (ProgressInd) {
		delete ProgressInd;
		ProgressInd = NULL;
	}
	OperationInProgress = false;
}
void MolDisplayWin::AbortOpen(const char * msg) {
	//On non-Mac systems we don't close the last window, instead
	//reinitialize the window data and leave open
#ifndef __WXMAC__
	MpApp & app = wxGetApp();
	if (app.WindowCount() <= 1) {	
		//This is the last window! Clear it out, but leave it open
		delete MainData;
		MainData = new MoleculeData;
		delete Prefs;
		Prefs = new WinPrefs;
		*Prefs = *gPreferences;
		Dirty = false;
		SetTitle(wxT("Untitled"));
		SetName(wxT("Untitled"));
	} else
#endif
	{
		if (this) Close(true);
	}
	
	if (msg != NULL) MessageAlert(msg);
} /* AbortOpen */

long MolDisplayWin::OpenFile(wxString fileName) {
	//This is modeled on OpenTextFile in the Mac version
	long				test=0;
	
	FILE * myfile = fopen(fileName.mb_str(wxConvUTF8), "r");
	if (myfile == NULL) {
		AbortOpen("Unable to open the requested file.");
		return 0;
	}
	BufferFile * Buffer = NULL;
	try {
		Buffer = new BufferFile(myfile, false);
		//this is temporary, they are supposed to come from the open file dialog
		long flip = 0;
		float offset = 0.0;
//		Window->SetSkipPoints(nSkip);
		
		// Attempt to identify the file type by looking for key words
		TextFileType type = Buffer->GetFileType((const char *) fileName.mb_str(wxConvUTF8));
		BeginOperation();
		switch (type) {
			case kMolType:
				test = OpenMolPltFile(Buffer);
				break;
			case kGAMESSlogType:
				test = OpenGAMESSlog(Buffer, false, flip, offset);
				break;
			case kGAMESSIRCType:
				test = OpenGAMESSIRC(Buffer, false,flip,offset);
				break;
			case kGAMESSDRCType:
				test = OpenGAMESSDRC(Buffer, false, false,flip,offset);
				break;
			case kGAMESSInputType:
				test = OpenGAMESSInput(Buffer);
				break;
			case kXYZType:
				test = OpenXYZFile(Buffer);
				break;
			case kPDBType:
				test = OpenPDBFile(Buffer);
				break;
			case kMDLMolFile:
				test = OpenMDLMolFile(Buffer);
				break;
			case CMLFile:
			{
				test = OpenCMLFile(Buffer);
				if (test == 0) AbortOpen(NULL);
			}
				break;
			default:	//Should only get here for unknown file types.
				AbortOpen("Unable to determine the file type.");
		}
	}
	catch (std::bad_alloc) {//Out of memory error
		AbortOpen("Not enough memory to open the file. Aborted!");
	}
	catch (MemoryError) {
		AbortOpen("Not enough memory to open the file. Aborted!");
	}
	catch (UserCancel) {
		AbortOpen("File open canceled by user");
	}
//	catch (DataError Error) {//Error parsing the file data
//		if (!Error.ErrorSet())  Window->AbortOpen(21);
//		else {
//			Error.WriteError();
//			delete Window; Window = NULL;
//		}
//	}
	//Some kind of File system related error
	catch (FileError Error) { Error.WriteError(); AbortOpen(NULL);}
	catch (...) { AbortOpen("Unknown error reading the selected file. File open aborted.");}
	if (Buffer) delete Buffer;		//Done reading so free up the buffer
	if (test) {//Note test is left 0 if any exception occurs(which causes Window to be deleted)
//		if (gPreferences->ChangeFileType()) {
//			// Looks like this is a good file so set the creator type for the neat icon
//			FInfo	myFInfo;
//			HGetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
//			if (myFInfo.fdCreator != (OSType) 'BMBm') {
//				myFInfo.fdCreator = (OSType) 'BMBm';
//				HSetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
//			}
//		}
		//Text files are not used after opening so close it immediately
		fclose(myfile);
//		Window->CloseFile();	//Hmmm should this happen for CML files?
//		if (!Window->IsSavedFile()) Window->SetFileType(5);
		FinishOperation();	//Close the progress dialog, if opened
//		if (!Window->IsSavedFile() && gPreferences->Default3DOn()) Window->Activate3D();
		//Tell the window its data has changed so that it will be redrawn correctly
//		if (!Window->IsSavedFile()) 
		ResetModel(true);
	}
	return test;
}
long MolDisplayWin::OpenCMLFile(BufferFile * Buffer, bool readPrefs, bool readWindows) {
	if (ProgressInd) ProgressInd->ChangeText("Reading CML file�");
	long test = 0;
	if (readWindows)
		test = MainData->OpenCMLFile(Buffer, Prefs, NULL, readPrefs);
//		test = MainData->OpenCMLFile(Buffer, Prefs, &winData, readPrefs);
	else
		test = MainData->OpenCMLFile(Buffer, Prefs, NULL, readPrefs);
/*	if (test == 0) AbortOpen(0);
	else {
		SetFileType(2);
		Rect tGRect;
		winData.GetMolWinRect(tGRect);
		SetWindowRect(&tGRect);
		CalculateWindowParameters();
		if (winData.is3DModeActive()) {
			winData.is3DModeActive(false);
			Activate3D();
		}
		ResetModel(false);
	}
			*/
	return test;
}

void MolDisplayWin::Rotate(wxMouseEvent& event) {
#ifdef BROKEN
	short		dx, dy, pixelDepth=0;
	int		iatm;
	Matrix4D	rotationMatrix, tempcopyMatrix;
	Surface * lSurface;
	bool	UpdateSurface=false;
	/* For now, the sphere and text stuff has been cut to simplify the
	 * function.  When it's working, they will be added back in.
	 */
	lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		UpdateSurface = UpdateSurface || lSurface->NeedsUpdateDuringRotation();
		lSurface = lSurface->GetNextSurface();
	}

	/* dx,dy are now measured from the origin (click point) to the current
	 * location
	 */
	dx = event.GetX();
	dy = event.GetY();
	if (event.ControlDown()) {	//Control key: translate instead of rotate
#if 0
		CPoint3D offset;
		if (event.ShiftDown()) {	/*The shift key is down so move into/out of the screen*/
			offset.z = dy/(hsize/MainData->WindowSize);
			MainData->TotalRotation[3][2] += offset.z;
		} else {
			offset.x = dx/(hsize/MainData->WindowSize);
			offset.y = dy/(hsize/MainData->WindowSize);
			MainData->TotalRotation[3][0] += offset.x;
			MainData->TotalRotation[3][1] -= offset.y;
		}
		if (!winData.is3DModeActive()) {
			/* Now rotate the coordinates by multiplying by the matrix */
			for (iatm=0; iatm<(MainData->cFrame->NumAtoms); iatm++) 
				Rotate3DPt(MainData->TotalRotation, MainData->cFrame->Atoms[iatm].Position,
						&(MainData->RotCoords[iatm]));	

			SortzBuffer(MainData->RotCoords, MainData->zBuffer, MainData->cFrame->NumAtoms);
		}
#endif
	} else {
		if (event.ShiftDown()) {	/*The shift key is down so zoom instead of rotating*/
			//if (dy == 0) continue;
			if (dy > 99) dy = 99;
			MainData->WindowSize *= 1.0 + 0.01*dy;
		} else {
			CPoint3D	InitialTrans, FinalTrans;
			//First back rotate the translation to get the inital translation
			InitialTrans.x = ((MainData->TotalRotation[3][0])*MainData->TotalRotation[0][0] +
					( MainData->TotalRotation[3][1])*MainData->TotalRotation[0][1] +
					( MainData->TotalRotation[3][2])*MainData->TotalRotation[0][2]);
			InitialTrans.y = ((MainData->TotalRotation[3][0])*MainData->TotalRotation[1][0] +
					( MainData->TotalRotation[3][1])*MainData->TotalRotation[1][1] +
					( MainData->TotalRotation[3][2])*MainData->TotalRotation[1][2]);
			InitialTrans.z = ((MainData->TotalRotation[3][0])*MainData->TotalRotation[2][0] +
					( MainData->TotalRotation[3][1])*MainData->TotalRotation[2][1] +
					( MainData->TotalRotation[3][2])*MainData->TotalRotation[2][2]);
			//Now zero out the translation part of the matrix
			MainData->TotalRotation[3][0] = MainData->TotalRotation[3][1] =
				MainData->TotalRotation[3][2] = 0.0;
			/* Concatenate the new rotation with the current rotation */
			MultiplyMatrix (rotationMatrix, MainData->TotalRotation, tempcopyMatrix);
			CopyMatrix (tempcopyMatrix, MainData->TotalRotation);
			//Now rotate the translation to the new orientation
			Rotate3DPt(MainData->TotalRotation, InitialTrans, &FinalTrans);
			MainData->TotalRotation[3][0] = FinalTrans.x;
			MainData->TotalRotation[3][1] = FinalTrans.y;
			MainData->TotalRotation[3][2] = FinalTrans.z;

#if 0
			if (!winData.is3DModeActive()) {
				/* Now rotate the coordinates by multiplying by the matrix */
				for (iatm=0; iatm<(MainData->cFrame->NumAtoms); iatm++) 
					Rotate3DPt(MainData->TotalRotation, MainData->cFrame->Atoms[iatm].Position,
							&(MainData->RotCoords[iatm]));	

				SortzBuffer(MainData->RotCoords, MainData->zBuffer, MainData->cFrame->NumAtoms);
			}
#endif
		}
	}	//Update any surfaces which depend on the screen plane
	if (UpdateSurface) {
		lSurface = MainData->cFrame->SurfaceList;
		while (lSurface) {
			lSurface->RotateEvent(MainData);
			lSurface = lSurface->GetNextSurface();
		}
	}
	/* Update the window */
#ifdef UseOpenGL
		if (winData.is3DModeActive()) {
			if (UpdateSurface) UpdateGLModel();
			RotateMoleculeGL(ShowAngles);
		} else
#endif
		{
			float scale = ((float) MAX(hsize, vsize))/(MainData->WindowSize);
			BeginDrawingOffscreen(&myGWorld, thisWindow);
			ClipRect(&TotalRect);
			EraseRect(&TotalRect);
			int npasses=1;
			Matrix4D	SavedRotation;
			if (Stereo) npasses=2;
			long hcenter = hoffset;
			for (int ipass=0; ipass<npasses; ipass++) {
				ClipRect(&DisplayRect);
				if (MainData->DrawMode & (1<<5)) {	//Planar drawing mode
					//Scan the SurfaceList, drawing any that can be drawn
					lSurface = MainData->cFrame->SurfaceList;
					while (lSurface) {
						lSurface->Draw2D(MainData, hcenter, sphereCenter.v, scale);
						lSurface = lSurface->GetNextSurface();
					}
					DrawMoleculePlane(scale, hcenter, sphereCenter.v);
				} else {	//otherwise use the normal psuedo-3D drawing code
					DrawMolecule(scale, hcenter, sphereCenter.v);
				}
				if (Stereo) {
					if (ipass == 0) {
						CopyMatrix(MainData->TotalRotation, SavedRotation);
						ApplyRotation(MainData->TotalRotation, 1, Prefs->GetStereoOffset());
						MainData->ResetRotation();
						Rect portRect;
#ifdef CarbonBuild
						GetWindowPortBounds(thisWindow, &portRect);
#else
						portRect = thisWindow->portRect;
#endif
						DisplayRect.left = DisplayRect.right + 1;
						DisplayRect.right = portRect.right;
						hcenter += width;
					} else {
						CopyMatrix(SavedRotation, MainData->TotalRotation);
						MainData->ResetRotation();
						DisplayRect.right = DisplayRect.left - 1;
						DisplayRect.left = TotalRect.left;
					}
				}
			}
			ClipRect(&TotalRect);
			FrameOval(&sphereRect);
			EndDrawingOffscreen(myGWorld, thisWindow);
		}
#if 0
	if (ShowAngles) {
		float psi, phi, theta;
		MatrixToEulerAngles(MainData->TotalRotation, &psi, &phi, &theta);
		sprintf((char *)AngleString, "_%.2f, %.2f, %.2f, Scale:%.2f",
				psi, phi, theta, MainData->WindowSize);
		AngleString[0] = strlen((char *)AngleString) - 1;
		TextFont(systemFont);
		MoveTo(2,12);
		DrawString(AngleString);
	}
#endif

	//"clean up" the rotation matrix make the rotation part orthogonal and magnitude 1
	OrthogonalizeRotationMatrix (MainData->TotalRotation);
	DisplayRect = TotalRect;
	/* Draw once again to get rid of the rotation sphere */
	draw();
	SetCursorToArrow();
#endif
}
