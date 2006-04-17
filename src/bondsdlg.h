/////////////////////////////////////////////////////////////////////////////
// Name:        bondsdlg.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 13 Apr 16:02:46 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _BONDSDLG_H_
#define _BONDSDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "bondsdlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/grid.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGrid;
////@end forward declarations
class MolDisplayWin;

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_BONDSDIALOG 10019
#define SYMBOL_BONDSDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_BONDSDLG_TITLE _("BondsDlg")
#define SYMBOL_BONDSDLG_IDNAME ID_BONDSDIALOG
#define SYMBOL_BONDSDLG_SIZE wxSize(400, 300)
#define SYMBOL_BONDSDLG_POSITION wxDefaultPosition
#define ID_CHOICE 10022
#define ID_BONDGRID 10020
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * BondsDlg class declaration
 */

class BondsDlg: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( BondsDlg )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    BondsDlg( );
    BondsDlg( MolDisplayWin* parent, wxWindowID id = SYMBOL_BONDSDLG_IDNAME, const wxString& caption = SYMBOL_BONDSDLG_TITLE, const wxPoint& pos = SYMBOL_BONDSDLG_POSITION, const wxSize& size = SYMBOL_BONDSDLG_SIZE, long style = SYMBOL_BONDSDLG_STYLE );

    /// Creation
    bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_BONDSDLG_IDNAME, const wxString& caption = SYMBOL_BONDSDLG_TITLE, const wxPoint& pos = SYMBOL_BONDSDLG_POSITION, const wxSize& size = SYMBOL_BONDSDLG_SIZE, long style = SYMBOL_BONDSDLG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin BondsDlg event handler declarations

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
    void OnAddClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
    void OnDeleteClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
    void OnChoiceSelected( wxCommandEvent& event );

    /// wxEVT_GRID_SELECT_CELL event handler for ID_BONDGRID
    void OnSelectCell( wxGridEvent& event );

    /// wxEVT_GRID_EDITOR_HIDDEN event handler for ID_BONDGRID
    void OnEditorHidden( wxGridEvent& event );

    /// wxEVT_GRID_RANGE_SELECT event handler for ID_BONDGRID
    void OnRangeSelect( wxGridRangeSelectEvent& event );

////@end BondsDlg event handler declarations

////@begin BondsDlg member function declarations

    MolDisplayWin * GetParent() const { return Parent ; }
    void SetParent(MolDisplayWin * value) { Parent = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end BondsDlg member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
	
	// Clear off and rebuild the list of bonds
	void ResetList(void);

////@begin BondsDlg member variables
    wxButton* AddBtn;
    wxButton* DeleteBtn;
    wxChoice* BondOrderCtl;
    wxGrid* bondGrid;
    MolDisplayWin * Parent;
////@end BondsDlg member variables
};

#endif
    // _BONDSDLG_H_
