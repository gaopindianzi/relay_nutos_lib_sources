#ifndef APPOPTDLG_H_
#define APPOPTDLG_H_

/* ----------------------------------------------------------------------------
 * Copyright (C) 2004-2007 by egnite Software GmbH
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
 * $Log$
 * Revision 1.3  2008/03/17 10:22:48  haraldkipp
 * Added more comments.
 *
 * Revision 1.2  2006/10/05 17:04:45  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.1  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 */

#include <wx/wx.h>
#include <wx/config.h>

/*!
 * \brief Settings dialog for application tree options.
 */
class CAppOptionsDialog: public wxPanel
{
DECLARE_CLASS(CAppOptionsDialog)
public:
    CAppOptionsDialog(wxWindow* parent);
    virtual bool TransferDataToWindow();
    virtual bool TransferDataFromWindow();
private:
    /*! \brief Combo box control used to select the programmer.
    */
    wxComboBox *m_cbxProgrammer;

    /*! \brief Text control used to specify the sample directory path.
    */
    wxTextCtrl *m_entAppDir;

    void OnBrowseAppDir(wxCommandEvent& event);
    void OnProgrammerEnter(wxCommandEvent& event);
    void OnAppDirChange(wxCommandEvent& event);
    void PopulateProgrammer();

    DECLARE_EVENT_TABLE()

};

#endif
