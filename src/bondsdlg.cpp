/////////////////////////////////////////////////////////////////////////////
// Name:        bondsdlg.cpp
// Purpose:     Provide a dialog to allow the user to inspect and change the bond list
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 13 Apr 16:02:45 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "bondsdlg.h"
#endif
#include "Globals.h"

#include "Globals.h"
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "MolDisplayWin.h"
#include "Frame.h"

#include "bondsdlg.h"

////@begin XPM images
////@end XPM images

/*!
 * BondsDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BondsDlg, wxDialog )

/*!
 * BondsDlg event table definition
 */

BEGIN_EVENT_TABLE( BondsDlg, wxDialog )

////@begin BondsDlg event table entries
    EVT_CLOSE( BondsDlg::OnCloseWindow )

    EVT_BUTTON( wxID_ADD, BondsDlg::OnAddClick )

    EVT_BUTTON( wxID_DELETE, BondsDlg::OnDeleteClick )

    EVT_CHOICE( ID_CHOICE, BondsDlg::OnChoiceSelected )

    EVT_GRID_CELL_CHANGE( BondsDlg::OnCellChange )
    EVT_GRID_SELECT_CELL( BondsDlg::OnSelectCell )
    EVT_GRID_RANGE_SELECT( BondsDlg::OnRangeSelect )
    EVT_SIZE( BondsDlg::OnSize )

////@end BondsDlg event table entries

END_EVENT_TABLE()

/*!
 * BondsDlg constructors
 */

BondsDlg::BondsDlg( )
{
}

BondsDlg::BondsDlg( MolDisplayWin * parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * BondsDlg creator
 */

bool BondsDlg::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin BondsDlg member initialisation
    AddBtn = NULL;
    DeleteBtn = NULL;
    bondText = NULL;
    BondOrderCtl = NULL;
    bondGrid = NULL;
////@end BondsDlg member initialisation
    Parent = parent;

////@begin BondsDlg creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end BondsDlg creation
    return true;
}

/*!
 * Control creation for BondsDlg
 */

void BondsDlg::CreateControls()
{    
////@begin BondsDlg content construction
    BondsDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    AddBtn = new wxButton( itemDialog1, wxID_ADD, _("&Add"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        AddBtn->SetToolTip(_("Add a new bond"));
    itemBoxSizer3->Add(AddBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DeleteBtn = new wxButton( itemDialog1, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        DeleteBtn->SetToolTip(_("deletes the selected bond(s)"));
    DeleteBtn->Enable(false);
    itemBoxSizer3->Add(DeleteBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    bondText = new wxStaticText( itemDialog1, wxID_STATIC, _("Bond Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    bondText->Enable(false);
    itemBoxSizer3->Add(bondText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString BondOrderCtlStrings[] = {
        _("Hydrogen Bond"),
        _("Single Bond"),
        _("Double Bond"),
        _("Triple Bond")
    };
    BondOrderCtl = new wxChoice( itemDialog1, ID_CHOICE, wxDefaultPosition, wxDefaultSize, 4, BondOrderCtlStrings, 0 );
    BondOrderCtl->SetStringSelection(_("Single Bond"));
    if (ShowToolTips())
        BondOrderCtl->SetToolTip(_("Bond Order"));
    BondOrderCtl->Enable(false);
    itemBoxSizer3->Add(BondOrderCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 5, wxGROW, 5);

    bondGrid = new wxGrid( itemDialog1, ID_BONDGRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL );
    bondGrid->SetDefaultColSize(100);
    bondGrid->SetDefaultRowSize(25);
    bondGrid->SetColLabelSize(25);
    bondGrid->SetRowLabelSize(0);
    bondGrid->CreateGrid(5, 4, wxGrid::wxGridSelectRows);
    itemBoxSizer8->Add(bondGrid, 1, wxGROW|wxALL, 0);

////@end BondsDlg content construction
	 //Setup the columns to store integers and floats
	bondGrid->SetColFormatNumber(0);
	bondGrid->SetColFormatNumber(1);
	bondGrid->SetColFormatFloat(2, -1, 6);
	bondGrid->SetColLabelValue(0, _T("Atom 1"));
	bondGrid->SetColLabelValue(1, _T("Atom 2"));
	bondGrid->SetColLabelValue(2, _T("Length"));
	bondGrid->SetColLabelValue(3, _T("Type"));
	wxSize s(50, 150);
	bondGrid->SetMinSize(s);
	ResetList();
}

void BondsDlg::ResetList(void) {
		//Clear off the old rows
	bondGrid->DeleteRows(0, bondGrid->GetNumberRows(), true);
	//loop through the bonds and add each with SetItem(row, col, string)
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	if (nbonds > 0) {
		bool temp = lFrame->GetBondSelectState(0);
		//Add back the new ones
		bondGrid->InsertRows(0, nbonds, true);
		bondGrid->HideCellEditControl();
		bondGrid->ClearSelection();
		lFrame->SetBondSelectState(0, temp);
		
		wxString buf;
		for (long i=0; i<nbonds; i++) {
			Bond * b = lFrame->GetBondLoc(i);
			buf.Printf(wxT("%d"), (b->Atom1 + 1));
			bondGrid->SetCellValue(i, 0, buf);
			buf.Printf(wxT("%d"), (b->Atom2 + 1));
			bondGrid->SetCellValue(i, 1, buf);
			buf.Printf(wxT("%f"), lFrame->GetBondLength(i));
			bondGrid->SetCellValue(i, 2, buf);
			bondGrid->SetReadOnly(i, 2, true);
			switch (lFrame->GetBondOrder(i)) {
				case kHydrogenBond:
					buf.Printf(wxT("%s"), _T("Hydrogen"));
					break;
				case kSingleBond:
					buf.Printf(wxT("%s"), _T("Single"));
					break;
				case kDoubleBond:
					buf.Printf(wxT("%s"), _T("Double"));
					break;
				case kTripleBond:
					buf.Printf(wxT("%s"), _T("Triple"));
					break;
			}
			bondGrid->SetCellValue(i, 3, buf);
			bondGrid->SetReadOnly(i, 3, true);
			bondGrid->SetCellAlignment(wxALIGN_CENTRE, i, 3);
			if (b->GetSelectState()) bondGrid->SelectRow(i, true);
		}
	}
	UpdateControls();
}

void BondsDlg::UpdateControls(void) {
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	bool selectionActive = false;
	BondOrder selectedOrder=kUnknownBond;
	for (long i=0; i<nbonds; i++) {
		Bond * b = lFrame->GetBondLoc(i);
		if (b->GetSelectState()) {
			if (!selectionActive) {
				selectionActive = true;
				selectedOrder = lFrame->GetBondOrder(i);
			} else {
				if (selectedOrder != lFrame->GetBondOrder(i))
					selectedOrder = kMixedBonds;
			}
		}
	}
	DeleteBtn->Enable(selectionActive);
	if (selectedOrder < kMixedBonds) BondOrderCtl->SetSelection(selectedOrder);
	BondOrderCtl->Enable(selectionActive);
	bondText->Enable(selectionActive);
}

/*!
 * Should we show tooltips?
 */

bool BondsDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BondsDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin BondsDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BondsDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BondsDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BondsDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BondsDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
 */

void BondsDlg::OnAddClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	if (lFrame->AddBond(0,1)) {
		wxString buf;
		bondGrid->AppendRows(1);
		buf.Printf(wxT("%d"), 1);
		bondGrid->SetCellValue(nbonds, 0, buf);
		buf.Printf(wxT("%d"), 2);
		bondGrid->SetCellValue(nbonds, 1, buf);
		buf.Printf(wxT("%f"), lFrame->GetBondLength(nbonds));
		bondGrid->SetCellValue(nbonds, 2, buf);
		bondGrid->SetReadOnly(nbonds, 2, true);
		switch (lFrame->GetBondOrder(nbonds)) {
			case kHydrogenBond:
				buf.Printf(wxT("%s"), _T("Hydrogen"));
				break;
			case kSingleBond:
				buf.Printf(wxT("%s"), _T("Single"));
				break;
			case kDoubleBond:
				buf.Printf(wxT("%s"), _T("Double"));
				break;
			case kTripleBond:
				buf.Printf(wxT("%s"), _T("Triple"));
				break;
		}
		bondGrid->SetCellValue(nbonds, 3, buf);
		bondGrid->SetReadOnly(nbonds, 3, true);
		bondGrid->SetCellAlignment(wxALIGN_CENTRE, nbonds, 3);
		lFrame->SetBondSelectState(nbonds, true);
		bondGrid->SelectRow(nbonds, true);
		bondGrid->SetGridCursor(nbonds, 0);
		bondGrid->MakeCellVisible(nbonds, 0);
		bondGrid->ShowCellEditControl();
	}
	UpdateControls();
	Parent->ResetModel(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void BondsDlg::OnDeleteClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	for (int i=(nbonds-1); i>=0; i--) {
		Bond * b = lFrame->GetBondLoc(i);
		if (b->GetSelectState()) {
			lFrame->DeleteBond(i);
			bondGrid->DeleteRows(i, 1, true);
		}
	}
	bondGrid->ClearSelection();
	UpdateControls();
	Parent->ResetModel(false);
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void BondsDlg::OnChoiceSelected( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	int choice = BondOrderCtl->GetSelection();

	for (int i=0; i<nbonds; i++) {
		if (lFrame->GetBondSelectState(i)) {
			wxString order;
			switch (choice) {
				case kHydrogenBond:
					lFrame->SetBondOrder(i, kHydrogenBond);
					order.Printf(wxT("%s"), _T("Hydrogen"));
					break;
				case kSingleBond:
					lFrame->SetBondOrder(i, kSingleBond);
					order.Printf(wxT("%s"), _T("Single"));
					break;
				case kDoubleBond:
					lFrame->SetBondOrder(i, kDoubleBond);
					order.Printf(wxT("%s"), _T("Double"));
					break;
				case kTripleBond:
					lFrame->SetBondOrder(i, kTripleBond);
					order.Printf(wxT("%s"), _T("Triple"));
					break;
			}
			bondGrid->SetCellValue(i, 3, order);
		}
	}
	Parent->ResetModel(false);
}

/*!
 * wxEVT_GRID_SELECT_CELL event handler for ID_BONDGRID
 */

void BondsDlg::OnSelectCell( wxGridEvent& event )
{
	int row = event.GetRow();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	for (int i=0; i<nbonds; i++) {
		Bond * b = lFrame->GetBondLoc(i);
		b->SetSelectState(false);
	}
	if ((row>=0)&&(row<nbonds)) {
		Bond * b = lFrame->GetBondLoc(row);
		b->SetSelectState(event.Selecting());
	}
	UpdateControls();
	event.Skip();
}



/*!
 * wxEVT_GRID_RANGE_SELECT event handler for ID_BONDGRID
 */

void BondsDlg::OnRangeSelect( wxGridRangeSelectEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	//for (int i=0; i<nbonds; i++) lFrame->SetBondSelectState(i, false);
	if(event.Selecting()) {
	  for (int i=0; i<nbonds; i++) lFrame->SetBondSelectState(i, false);
	  for (int i=event.GetTopRow(); i<=event.GetBottomRow(); i++) {
	    lFrame->SetBondSelectState(i, true);
	  }
	}
	
	UpdateControls();
	event.Skip();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_BONDSDIALOG
 */

void BondsDlg::OnCloseWindow( wxCloseEvent& event )
{
	Parent->CloseBondsWindow();
}


/*!
 * wxEVT_GRID_CELL_CHANGE event handler for ID_BONDGRID
 */

void BondsDlg::OnCellChange( wxGridEvent& event )
{
	int row = event.GetRow();
	int col = event.GetCol();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	wxString val = bondGrid->GetCellValue(row, col);
	long newval;
	if (val.ToLong(&newval)) {
		newval --;
		if ((newval>=0)&&(newval<lFrame->GetNumAtoms())&&
			(newval!=lFrame->GetBondAtom(row, 2-col))) {
			lFrame->ChangeBond(row, col+1, newval);
		}
	}
	val.Printf(wxT("%d"), (lFrame->GetBondAtom(row, col+1) + 1));
	bondGrid->SetCellValue(row, col, val);
	val.Printf(wxT("%f"), lFrame->GetBondLength(row));
	bondGrid->SetCellValue(row, 2, val);
	for (int i=0; i<nbonds; i++) lFrame->SetBondSelectState(i, false);
	Parent->ResetModel(false);
	event.Skip();
}

/*!
 * wxEVT_SIZE event handler for ID_BONDGRID
 */

void BondsDlg::OnSize( wxSizeEvent& event )
{
	wxSize s = event.GetSize();
	int width = (s.GetWidth()/4) - 4;
	if (width < 40) width = 40;
	bondGrid->SetColSize(0, width);
	bondGrid->SetColSize(1, width);
	bondGrid->SetColSize(2, width);
	bondGrid->SetColSize(3, width);
    event.Skip();
}

void BondsDlg::UpdateSelection(bool mode)
{
  MoleculeData * MainData = Parent->GetData();
  Frame * lFrame = MainData->GetCurrentFramePtr();
  long nbonds = lFrame->GetNumBonds();

  if (nbonds != bondGrid->GetNumberRows())
    {
      ResetList();
      bondGrid->MakeCellVisible(nbonds-1, 0);
    }

  if (mode)
    bondGrid->ClearSelection();

  for (long i=0; i<nbonds; i++) 
    if (lFrame->GetBondSelectState(i))
      bondGrid->SelectRow(i, true);

  Layout();
}
