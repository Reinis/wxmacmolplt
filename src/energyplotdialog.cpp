/////////////////////////////////////////////////////////////////////////////
// Name:        energyplotdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue 23 May 2006 01:34:03 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "energyplotdialog.h"
#endif

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

#include "energyplotdialog.h"

////@begin XPM images

////@end XPM images

/*!
 * EnergyPlotDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( EnergyPlotDialog, wxDialog )

/*!
 * EnergyPlotDialog event table definition
 */

BEGIN_EVENT_TABLE( EnergyPlotDialog, wxDialog )

////@begin EnergyPlotDialog event table entries
////@end EnergyPlotDialog event table entries

END_EVENT_TABLE()

/*!
 * EnergyPlotDialog constructors
 */

EnergyPlotDialog::EnergyPlotDialog( )
{
}

EnergyPlotDialog::EnergyPlotDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * EnergyPlotDialog creator
 */

bool EnergyPlotDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin EnergyPlotDialog member initialisation
    epGraph = NULL;
////@end EnergyPlotDialog member initialisation

////@begin EnergyPlotDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end EnergyPlotDialog creation
    return true;
}

/*!
 * Control creation for EnergyPlotDialog
 */

void EnergyPlotDialog::CreateControls()
{    
////@begin EnergyPlotDialog content construction
    EnergyPlotDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    epGraph = new wxMolGraph( itemDialog1, ID_EPGRAPH, wxDefaultPosition, wxSize(350, 200), wxSUNKEN_BORDER );
    itemBoxSizer2->Add(epGraph, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end EnergyPlotDialog content construction
}

/*!
 * Should we show tooltips?
 */

bool EnergyPlotDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap EnergyPlotDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin EnergyPlotDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end EnergyPlotDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon EnergyPlotDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin EnergyPlotDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end EnergyPlotDialog icon retrieval
}