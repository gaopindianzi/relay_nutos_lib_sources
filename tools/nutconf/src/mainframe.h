#ifndef MAINFRAME_H_
#define MAINFRAME_H_

/* ----------------------------------------------------------------------------
 * Copyright (C) 2004 by egnite Software GmbH
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
 * $Log$
 * Revision 1.5  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.4  2005/07/22 18:46:25  haraldkipp
 * Added selectable toolbar button sizes, toolbar menu, more toolbar buttons
 * and online help.
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/wx.h>
#include <wx/laywin.h>
#include <wx/splitter.h>
#include <wx/docview.h>
//#include <wx/docmdi.h>
#include <wx/treectrl.h>
#include <wx/help.h>

#include "configtree.h"
//#include "conflictlist.h"
#include "valuewindow.h"
#include "splitscroll.h"
#include "infowindow.h"
#include "propertylist.h"
#include "finddlg.h"

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

/*
 * \brief Main frame acting as a document parent frame.
 */
class CMainFrame:public wxDocParentFrame {
  public:
    CMainFrame(wxDocManager * manager, const wxString & title);
    virtual ~CMainFrame();

    void OnSize(wxSizeEvent & event);
    void OnSashDrag(wxSashEvent & event);

    wxTextCtrl *GetOutputWindow() const;
    CConfigTree *GetTreeCtrl() const;
    CValueWindow *GetValueWindow() const;
    CInfoWindow *GetInfoWindow() const;
    CPropertyList *GetPropertyListWindow() const;

    wxHelpController& GetHelpController() { return m_help; }

    CFindDialog *GetFindDialog() const;
    void SetFindDialog(CFindDialog * dlg);

    /* ---------- Menu Event Handlers ---------- */
    void OnQuit(wxCommandEvent & event);
    void OnGenerateBuildTree(wxCommandEvent & event);
    void OnBuildNutOS(wxCommandEvent & event);
    void OnBuildLibraries(wxCommandEvent & event);
    void OnCreateSampleDir(wxCommandEvent & event);
    void OnSettings(wxCommandEvent& event);
    void OnToggleToolbar(wxCommandEvent& event);
    void OnToggleToolbarSize(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnFind(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

  protected:
    void CreateNutMenuBar();
    void CreateNutToolBar();
    void CreateNutStatusBar();
    void CreateNutWindows();

  protected:
    wxSashLayoutWindow *m_outputSashWindow;
    wxSashLayoutWindow *m_configSashWindow;
    wxSashLayoutWindow *m_propertiesSashWindow;
    wxSashLayoutWindow *m_infoSashWindow;

    CSplitScroll *m_scrolledWindow;
    wxSplitterWindow *m_splitter;
    CConfigTree *m_treeWindow;
    CValueWindow *m_valueWindow;

    CPropertyList *m_propertyListWindow;
    CInfoWindow *m_infoWindow;

    wxTextCtrl *m_outputWindow;

    bool m_smallToolbar;

  private:
    wxHelpController m_help;
    CFindDialog* m_findDlg;

    DECLARE_EVENT_TABLE()

};

#endif
