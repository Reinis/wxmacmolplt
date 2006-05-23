/////////////////////////////////////////////////////////////////////////////
// Name:        aboutwxmacmolplt.cpp
// Purpose:     
// Author:      Brett Bode
// Created:     Tue 23 May 13:01:58 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Tue 23 May 13:01:58 2006

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "aboutwxmacmolplt.h"
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
#include "globals.h"
#include "aboutwxmacmolplt.h"

////@begin XPM images
////@end XPM images

/*!
 * AboutwxMacMolPlt type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AboutwxMacMolPlt, wxDialog )

/*!
 * AboutwxMacMolPlt event table definition
 */

BEGIN_EVENT_TABLE( AboutwxMacMolPlt, wxDialog )

////@begin AboutwxMacMolPlt event table entries
////@end AboutwxMacMolPlt event table entries

END_EVENT_TABLE()

/*!
 * AboutwxMacMolPlt constructors
 */

AboutwxMacMolPlt::AboutwxMacMolPlt( )
{
}

AboutwxMacMolPlt::AboutwxMacMolPlt( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * AboutwxMacMolPlt creator
 */

bool AboutwxMacMolPlt::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AboutwxMacMolPlt member initialisation
////@end AboutwxMacMolPlt member initialisation

////@begin AboutwxMacMolPlt creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end AboutwxMacMolPlt creation
    return true;
}

/*!
 * Control creation for AboutwxMacMolPlt
 */

void AboutwxMacMolPlt::CreateControls()
{    
////@begin AboutwxMacMolPlt content construction
    // Generated by DialogBlocks, Tue 23 May 13:15:36 2006 (unregistered)

    AboutwxMacMolPlt* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("wxMacMolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemStaticText3->SetToolTip(_("based on the wxWidgets toolkit"));
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("PostCardWare--Please let me\nknow if you use MacMolPlt. If you\nwish to see further future\ndevelopment then let me know\nthat you use my program!"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Available via the WWW URL:\nhttp://www.scl.ameslab.gov/\n~brett/MacMolPlt/"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Dr. Brett Bode\n329 Wilhelm Hall\nIowa State University\nAmes, IA 50011\nbrett@scl.ameslab.gov"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemStaticText8->SetToolTip(_("Just send an email"));
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("MacMolPlt v6.0\nBy Brett Bode"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Please reference: Bode, B. M. and Gordon, M. S. J. Mol. Graphics and\nModeling, 1998, 16, 133-138."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Thanks to the Gordon Group at Iowa State University, Apple Computer and the National Science Foundation for providing programming support."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText11->Wrap(300);
    if (ShowToolTips())
        itemStaticText11->SetToolTip(_("Thanks even to Kurt..."));
    itemBoxSizer2->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxButton* itemButton12 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end AboutwxMacMolPlt content construction
    itemStaticText9->SetLabel(_("MacMolPlt v"wxMacMolPlt_VERSION"\nBy Brett Bode"));
}

/*!
 * Should we show tooltips?
 */

bool AboutwxMacMolPlt::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AboutwxMacMolPlt::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AboutwxMacMolPlt bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end AboutwxMacMolPlt bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AboutwxMacMolPlt::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AboutwxMacMolPlt icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AboutwxMacMolPlt icon retrieval
}
