/* ----------------------------------------------------------------------------
 * Copyright (C) 2004-2007 by egnite Software GmbH
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ----------------------------------------------------------------------------
 * Parts are
 *
 * Copyright (C) 1998, 1999, 2000 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * ----------------------------------------------------------------------------
 */

/*
 * $Log: tooloptdlg.cpp,v $
 * Revision 1.2  2007/04/25 16:01:42  haraldkipp
 * Path entry validator added.
 * Transfer functions return actual result.
 *
 * Revision 1.1  2004/09/17 13:03:48  haraldkipp
 * New settings page for tool options
 *
 */

#include <wx/valgen.h>
#include <wx/filename.h>

#include "ids.h"
#include "nutconf.h"
#include "pathvalidator.h"
#include "tooloptdlg.h"

IMPLEMENT_CLASS(CToolOptionsDialog, wxPanel)

CToolOptionsDialog::CToolOptionsDialog(wxWindow* parent)
: wxPanel(parent, ID_SETTINGS_REPOSITORY)
{
    CSettings *opts = wxGetApp().GetSettings();
    CPathValidator toolPathValid(VALIDPATH_LIST | VALIDPATH_IS_DIRECTORY | VALIDPATH_EXISTS | VALIDPATH_SHOW_NATIVE, &opts->m_toolpath);

    wxStaticBox *groupPath = new wxStaticBox(this, -1, wxT("Tool Paths"));
    m_entryPath = new wxTextCtrl(this, ID_PATH_ENTRY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, toolPathValid);

    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    wxSizer *sizerGroup = new wxStaticBoxSizer(groupPath, wxHORIZONTAL);

    sizerGroup->Add(m_entryPath, 1, wxALIGN_LEFT | wxGROW | wxALL, 5);
    sizerTop->Add(sizerGroup, 0, wxGROW | wxALIGN_CENTRE | wxALL, 5);

    SetAutoLayout(true);
    SetSizer(sizerTop);
}

bool CToolOptionsDialog::TransferDataToWindow()
{
    return wxPanel::TransferDataToWindow();
}

bool CToolOptionsDialog::TransferDataFromWindow()
{
    return wxPanel::TransferDataFromWindow();
}

