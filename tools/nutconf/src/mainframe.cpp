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
 * $Log: mainframe.cpp,v $
 * Revision 1.16  2009/01/04 04:30:49  thiagocorrea
 * Allow nutconf to build with _UNICODE under Win32.
 *
 * Revision 1.15  2008/07/29 07:39:37  haraldkipp
 * Fixed missing 'make install'. Bug was introduced in 2.0.1.
 *
 * Revision 1.14  2008/07/24 15:44:22  haraldkipp
 * Fixed initial window creation on Linux.
 *
 * Revision 1.13  2008/03/17 10:22:48  haraldkipp
 * Added more comments.
 *
 * Revision 1.12  2007/04/25 16:02:27  haraldkipp
 * Adding 3D style to sash in component tree fixes repaint problems.
 * Further added live update and sash gravity attributes for easier resizing.
 *
 * Revision 1.11  2007/02/15 19:33:45  haraldkipp
 * Version 1.4.1 works with wxWidgets 2.8.0.
 * Several wide character issues fixed.
 *
 * Revision 1.10  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.9  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.8  2005/09/21 10:44:15  christianwelzel
 * Replaced deprecated commands with newer ones.
 *
 * Revision 1.7  2005/07/22 18:46:25  haraldkipp
 * Added selectable toolbar button sizes, toolbar menu, more toolbar buttons
 * and online help.
 *
 * Revision 1.6  2005/07/20 11:16:00  haraldkipp
 * Linux application icon added
 *
 * Revision 1.5  2005/07/20 09:20:25  haraldkipp
 * Set application icon
 *
 * Revision 1.4  2005/04/22 15:14:43  haraldkipp
 * Avoid compiler warnings.
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

#include <wx/config.h>

#include "ids.h"
#include "nutconf.h"
#include "settings.h"
#include "settingsdlg.h"
#include "mainframe.h"

#if !defined(__WXMSW__)
#include "bitmaps/application.xpm"
#include "bitmaps/buildlibrary.xpm"
#include "bitmaps/buildlibrary_large.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/help_large.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/open_large.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/save_large.xpm"
#include "bitmaps/search.xpm"
#include "bitmaps/search_large.xpm"
#endif

BEGIN_EVENT_TABLE(CMainFrame, wxDocParentFrame)
    EVT_MENU(wxID_EXIT, CMainFrame::OnQuit)
    EVT_MENU(ID_BUILD_LIBRARY, CMainFrame::OnBuildLibraries)
    EVT_MENU(ID_GENERATE_BUILD_TREE, CMainFrame::OnGenerateBuildTree)
    EVT_MENU(ID_BUILD_NUTOS, CMainFrame::OnBuildNutOS)
    EVT_MENU(ID_CREATE_SAMPLE_APPS, CMainFrame::OnCreateSampleDir)
    EVT_SIZE(CMainFrame::OnSize)
    EVT_SASH_DRAGGED_RANGE(ID_CONFIG_SASH_WINDOW, ID_OUTPUT_SASH_WINDOW, CMainFrame::OnSashDrag)
    EVT_MENU(ID_SETTINGS, CMainFrame::OnSettings)
    EVT_MENU(ID_TOOLBARS, CMainFrame::OnToggleToolbar)
    EVT_MENU(ID_TOOLBARSIZE, CMainFrame::OnToggleToolbarSize)
    EVT_MENU(ID_NUTOS_HELP, CMainFrame::OnHelp)
    EVT_MENU(wxID_FIND, CMainFrame::OnFind)
    EVT_MENU(ID_HELP_ABOUT, CMainFrame::OnAbout)
END_EVENT_TABLE();

CMainFrame::CMainFrame(wxDocManager * manager, const wxString & title)
:wxDocParentFrame(manager, (wxFrame *) NULL, ID_MAIN_FRAME, title)
, m_smallToolbar(true)
, m_findDlg(NULL)
{
    SetIcon(wxICON(application));

    CreateNutMenuBar();
    CreateNutToolBar();
    CreateNutStatusBar();

    CreateNutWindows();

    /*
     * Restore frame position and size.
     */
    wxConfigBase *pConfig = wxConfigBase::Get();
    Move(pConfig->Read(wxT("/MainFrame/x"), 50), pConfig->Read(wxT("/MainFrame/y"), 50));
    SetClientSize(pConfig->Read(wxT("/MainFrame/w"), 550), pConfig->Read(wxT("/MainFrame/h"), 350));

}

/*!
 * \brief Destructor.
 */
CMainFrame::~CMainFrame()
{
    // Save frame size and position.
    wxConfigBase *pConfig = wxConfigBase::Get();
    if (pConfig) {
        wxString lastPath = pConfig->GetPath();
        pConfig->SetPath(wxT("/MainFrame"));

        /*
         * Frame window position and client window size.
         */
        int x, y;
        int w, h;
        GetPosition(&x, &y);
        pConfig->Write(wxT("x"), (long) x);
        pConfig->Write(wxT("y"), (long) y);
        GetClientSize(&w, &h);
        pConfig->Write(wxT("w"), (long) w);
        pConfig->Write(wxT("h"), (long) h);

        wxSize sz;
        sz = m_configSashWindow->GetSize();
        pConfig->Write(wxT("tree_w"), (long) sz.x);
        pConfig->Write(wxT("tree_h"), (long) sz.y);

        sz = m_propertiesSashWindow->GetSize();
        pConfig->Write(wxT("prop_w"), (long) sz.x);
        pConfig->Write(wxT("prop_h"), (long) sz.y);

        sz = m_infoSashWindow->GetSize();
        pConfig->Write(wxT("desc_w"), (long) sz.x);
        pConfig->Write(wxT("desc_h"), (long) sz.y);

        sz = m_outputSashWindow->GetSize();
        pConfig->Write(wxT("outp_w"), (long) sz.x);
        pConfig->Write(wxT("outp_h"), (long) sz.y);

        x = m_splitter->GetSashPosition();
        if(x > 0) {
            pConfig->Write(wxT("split_x"), (long) x);
        }
        pConfig->SetPath(lastPath);
    }
}

void CMainFrame::CreateNutMenuBar()
{
    wxMenu *fileMenu = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
    //td fileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), wxT("Creates a new document"));
    fileMenu->Append(wxID_OPEN, wxT("&Open\tCtrl+O"), wxT("Opens an existing configuration"));
    fileMenu->Append(wxID_SAVE, wxT("&Save\tCtrl+S"), wxT("Saves the active configuration"));
    fileMenu->Append(wxID_SAVEAS, wxT("Save &As..."), wxT("Saves the active configuration with a new name"));
    //td fileMenu->AppendSeparator();
    //td fileMenu->Append(ID_IMPORT, wxT("&Import..."), wxT("Imports a minimal configuration exported from another configuration"));
    //td fileMenu->Append(ID_EXPORT, wxT("&Export..."), wxT("Exports a minimal configuration for importing into another configuration"));
    //td fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt+X"), wxT("Quits the application"));

    wxMenu *editMenu = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
    editMenu->Append(wxID_FIND, wxT("&Find...\tCtrl+F"), wxT("Finds the specified text"));
    editMenu->Append(ID_SETTINGS, wxT("&Settings...\tCtrl+T"), wxT("Shows the settings dialog"));
    //td editMenu->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"), wxT("Cuts the output pane selection and moves it to the Clipboard"));
    //td editMenu->Append(wxID_COPY, wxT("&Copy\tCtrl+C"), wxT("Copies the output pane selection to the clipboard"));
    //td editMenu->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"), wxT("Inserts Clipboard contents"));
    //td editMenu->Append(wxID_CLEAR, wxT("&Clear"), wxT("Erases everything in the output pane"));
    //td editMenu->AppendSeparator();
    //td editMenu->Append(wxID_SELECTALL, wxT("&Select All\tCtrl+A"), wxT("Selects the entire output pane"));
    //td editMenu->AppendSeparator();
    //td editMenu->Append(ID_FIND_NEXT, wxT("Find &Next\tF3"), wxT("Finds the next item matching the Find text"));
    //td editMenu->AppendSeparator();
    //td editMenu->Append(ID_SAVE_OUTPUT, wxT("Sa&ve Output..."), wxT("Saves the contents of the output pane"));

    wxMenu *viewMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    wxMenu *toolbarMenu = new wxMenu;
    toolbarMenu->AppendCheckItem(ID_TOOLBARS, wxT("&Show"), wxT("Shows or hides the toolbar"));
    toolbarMenu->Check(ID_TOOLBARS, true);
    toolbarMenu->AppendCheckItem(ID_TOOLBARSIZE, wxT("&Large Buttons"), wxT("Toggles toolbar size"));
    viewMenu->Append(-1, wxT("Toolbar"), toolbarMenu);
    //td viewMenu->Append(ID_SETTINGS, wxT("&Settings...\tCtrl+T"), wxT("Shows the application settings dialog"));
    //td viewMenu->AppendSeparator();
    //td viewMenu->Append(ID_TOGGLE_PROPERTIES, wxT("&Properties\tAlt+1"), wxT("Shows or hides the properties window"), true);
    //td viewMenu->Append(ID_TOGGLE_OUTPUT, wxT("&Output\tAlt+2"), wxT("Shows the output window"), true);
    //td viewMenu->Append(ID_TOGGLE_SHORT_DESCR, wxT("&Short Description\tAlt+3"), wxT("Shows or hides the short description window"), true);

    wxMenu *buildMenu = new wxMenu(wxEmptyString, wxMENU_TEAROFF);
    //td buildMenu->Append(ID_BUILD_LIBRARY, wxT("&Library\tF7"), wxT("Builds the library"));
    //td buildMenu->Append(ID_BUILD_TESTS, wxT("&Tests\tShift+F7"), wxT("Builds the tests"));
    //td buildMenu->Append(ID_CLEAN, wxT("&Clean"), wxT("Deletes intermediate and output files"));
    //td buildMenu->Append(ID_STOP_BUILD, wxT("&Stop"), wxT("Stops the build"));
    //td buildMenu->AppendSeparator();
    buildMenu->Append(ID_BUILD_LIBRARY, wxT("Build Nut/OS"), wxT("Builds Nut/OS libraries"));
    // buildMenu->Append(ID_GENERATE_BUILD_TREE, wxT("&Generate Build Tree"), wxT("Explicitly recreates the build tree"));
    // buildMenu->Append(ID_BUILD_NUTOS, wxT("Build Nut/OS"), wxT("Builds Nut/OS libraries"));
    buildMenu->AppendSeparator();
    buildMenu->Append(ID_CREATE_SAMPLE_APPS, wxT("Create Sample Directory"), wxT("Creates a directory with Nut/OS sample applications"));
    //td buildMenu->Append(ID_BUILD_OPTIONS, wxT("&Options..."), wxT("Changes build options"));
    //td buildMenu->Append(ID_BUILD_REPOSITORY, wxT("&Repository..."), wxT("Selects repository"));
    //td buildMenu->Append(ID_BUILD_TEMPLATES, wxT("&Templates..."), wxT("Selects the package templates"));
    //td buildMenu->Append(ID_BUILD_PACKAGES, wxT("&Packages..."), wxT("Selects individual packages"));

    //td wxMenu *toolsMenu = new wxMenu(wxT(""), wxMENU_TEAROFF);
    //td wxMenu *pathMenu = new wxMenu;
    //td pathMenu->Append(ID_PATHS_BUILD_TOOLS, wxT("&Build Tools..."), wxT("Specifies the folder containing the build tools"));
    //td pathMenu->Append(ID_PATHS_USER_TOOLS, wxT("&User Tools..."), wxT("Specifies the folder containing the user tools"));
    //td toolsMenu->Append(ID_PATHS, wxT("&Paths"), pathMenu);
    //td toolsMenu->Append(ID_SHELL, wxT("&Shell..."), wxT("Invokes a command shell"));
    //td toolsMenu->Append(ID_RUN_TESTS, wxT("&Run Tests...\tCtrl+F5"), wxT("Runs the configuration tests"));
    //td toolsMenu->Append(ID_PLATFORMS, wxT("&Platforms..."), wxT("Edits the platforms list"));
    //td toolsMenu->Append(ID_ADMINISTRATION, wxT("&Administration..."), wxT("Performs repository administration tasks"));
    //td toolsMenu->AppendSeparator();
    //td toolsMenu->Append(ID_INDEX_DOCS, wxT("Regenerate Help &Index"), wxT("Regenerates the online help contents"));

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(ID_NUTOS_HELP, wxT("Help &Contents"), wxT("Displays help contents"));
    helpMenu->Append(ID_HELP_ABOUT, wxT("About NutConf..."), wxT("Displays version and copyright information"));
    //td helpMenu->Append(ID_CONFIGTOOL_HELP, wxT("&Configuration Tool Help\tShift+F1"), wxT("Displays help"));
    //td helpMenu->Append(ID_CONTEXT_HELP, wxT("&Help On..."), wxT("Displays help for clicked-on windows"));
    //td helpMenu->AppendSeparator();

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, wxT("&File"));
    menuBar->Append(editMenu, wxT("&Edit"));
    menuBar->Append(viewMenu, wxT("&View"));
    menuBar->Append(buildMenu, wxT("&Build"));
    //td menuBar->Append(toolsMenu, wxT("&Tools"));
    menuBar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(menuBar);
}


void CMainFrame::CreateNutToolBar()
{
    wxToolBarBase *toolBar = CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL | wxTB_DOCKABLE, ID_TOOLBAR);
    toolBar->SetMargins(4, 4);

    /* Load small toolbar buttons. */
    if (m_smallToolbar) {
        toolBar->AddTool(wxID_OPEN, wxBITMAP(TBB_OPEN), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Open configuration"));
        toolBar->AddTool(wxID_SAVE, wxBITMAP(TBB_SAVE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Save configuration"));
        toolBar->AddTool(ID_BUILD_LIBRARY, wxBITMAP(TBB_BUILDLIBRARY), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Build Nut/OS"));
        toolBar->AddTool(wxID_FIND, wxBITMAP(TBB_SEARCH), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Search"));
        toolBar->AddTool(ID_NUTOS_HELP, wxBITMAP(TBB_HELP), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Show help"));
    }
    /* Load large toolbar buttons. */
    else {
        toolBar->AddTool(wxID_OPEN, wxBITMAP(TBB_OPEN_LARGE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Open configuration"));
        toolBar->AddTool(wxID_SAVE, wxBITMAP(TBB_SAVE_LARGE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Save configuration"));
        //toolBar->AddSeparator();
        toolBar->AddTool(ID_BUILD_LIBRARY, wxBITMAP(TBB_BUILDLIBRARY_LARGE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Build Nut/OS"));
        toolBar->AddTool(wxID_FIND, wxBITMAP(TBB_SEARCH_LARGE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Search"));
        toolBar->AddTool(ID_NUTOS_HELP, wxBITMAP(TBB_HELP_LARGE), wxNullBitmap, false, -1, -1, (wxObject *) NULL, wxT("Show help"));
        toolBar->SetToolBitmapSize(wxSize(32, 32));
    }
    toolBar->Realize();

    toolBar->SetHelpText(wxT("The toolbar allows quick access to commonly-used commands."));
}

void CMainFrame::CreateNutStatusBar()
{
    CreateStatusBar(4, wxST_SIZEGRIP);
    int widths[] = { -1, 100, 40, 80 };
    SetStatusWidths(4, widths);
    SetStatusText(wxT("Ready"));
}

void CMainFrame::CreateNutWindows()
{
    int w;
    int h;

    Show();
    /*
     * Create the bottom sash. Its client is a multiline text control,
     * which acts as a log output window. 
     */
    m_outputSashWindow = new wxSashLayoutWindow(this, ID_OUTPUT_SASH_WINDOW);
    m_outputSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_outputSashWindow->SetAlignment(wxLAYOUT_BOTTOM);
    m_outputSashWindow->SetSashVisible(wxSASH_TOP, true);

    m_outputWindow = new wxTextCtrl(m_outputSashWindow, ID_OUTPUT_WINDOW, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, 
        wxTE_MULTILINE | wxCLIP_CHILDREN | wxTE_READONLY | wxTE_RICH);
    wxLog *old_log = wxLog::SetActiveTarget(new wxLogTextCtrl(m_outputWindow));
    delete old_log;

    wxLogMessage(wxT("Nut/OS Configurator Version %s"), wxT(VERSION));
    wxLogMessage(wxT("Linked to %s"), wxVERSION_STRING);
    wxLogMessage(wxT("Running on %s"), wxGetOsDescription().c_str());
    wxLogMessage(wxT("Working in %s"), ::wxGetCwd().c_str());
    m_outputWindow->ShowPosition(m_outputWindow->GetLastPosition());

    /*
     * Create the left sash.
     */
    wxLogVerbose(wxT("Create ConfigSash"));
    m_configSashWindow = new wxSashLayoutWindow(this, ID_CONFIG_SASH_WINDOW);
    m_configSashWindow->SetOrientation(wxLAYOUT_VERTICAL);
    m_configSashWindow->SetAlignment(wxLAYOUT_LEFT);
    m_configSashWindow->SetSashVisible(wxSASH_RIGHT, true);

    /*
     * Create the top right sash. Its client is a property list.
     */
    wxLogVerbose(wxT("Create PropertySash"));
    m_propertiesSashWindow = new wxSashLayoutWindow(this, ID_PROPERTIES_SASH_WINDOW);
    m_propertiesSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_propertiesSashWindow->SetAlignment(wxLAYOUT_TOP);
    m_propertiesSashWindow->SetSashVisible(wxSASH_BOTTOM, true);

    wxLogVerbose(wxT("Create CPropertyList"));
    m_propertyListWindow = new CPropertyList(m_propertiesSashWindow, ID_PROPERTIES_WINDOW,
                                             wxDefaultPosition, wxDefaultSize,
                                             wxLC_REPORT | wxCLIP_CHILDREN | wxLC_VRULES | wxLC_HRULES | wxSUNKEN_BORDER);

    /*
     * Create the bottom right sash. Its client is a multiline
     * text control to display the description of a selected
     * tree item.
     */
    wxLogVerbose(wxT("Create InfoSash"));
    m_infoSashWindow = new wxSashLayoutWindow(this, ID_SHORT_DESCR_SASH_WINDOW);
    m_infoSashWindow->SetOrientation(wxLAYOUT_HORIZONTAL);
    m_infoSashWindow->SetAlignment(wxLAYOUT_TOP);

    wxLogVerbose(wxT("Create CInfoWindow"));
    m_infoWindow =
        new CInfoWindow(m_infoSashWindow, ID_SHORT_DESCR_WINDOW, wxDefaultPosition, wxDefaultSize,
                        wxTE_MULTILINE | wxCLIP_CHILDREN | wxTE_READONLY);

    /*
     * Component tree control.
     *
     * Window relationship:
     *  CSplitScroll - wxScrolledWindow
     *      wxSplitterWindow
     *          CConfigTree - CScrolledTreeCtrl - wxTreeCtrl
     *          CValueWindow - CTreeCompWindow - wxWindow
     */
    wxLogVerbose(wxT("Create CSplitScroll"));
    m_scrolledWindow = new CSplitScroll(m_configSashWindow, ID_SCROLLED_WINDOW, wxDefaultPosition,
                                        wxSize(250, 150), wxNO_BORDER | wxCLIP_CHILDREN | wxVSCROLL);
    wxLogVerbose(wxT("Create wxSplitterWindow"));
    m_splitter = new wxSplitterWindow(m_scrolledWindow, ID_SPLITTER_WINDOW, wxDefaultPosition,
                                      wxSize(240, 130), wxSP_LIVE_UPDATE | wxSP_3D | wxCLIP_CHILDREN);
    wxLogVerbose(wxT("Create CConfigTree"));
    m_treeWindow = new CConfigTree(m_splitter, ID_TREE_CTRL, wxDefaultPosition, wxSize(120, 120), 
                             wxTR_HAS_BUTTONS | /* wxTR_NO_LINES | */ wxTR_ROW_LINES | wxNO_BORDER);
    wxLogVerbose(wxT("Create CValueWindow"));
    m_valueWindow = new CValueWindow(m_splitter, ID_VALUE_WINDOW, wxDefaultPosition, 
                                     wxSize(120, 120), wxNO_BORDER);
    m_splitter->SplitVertically(m_treeWindow, m_valueWindow);
    m_splitter->SetMinimumPaneSize(100);
    m_splitter->SetSashGravity(0.5);

    m_scrolledWindow->EnableScrolling(false, false);

    // Let the two controls know about each other
    m_valueWindow->SetTreeCtrl(m_treeWindow);
    m_treeWindow->SetCompanionWindow(m_valueWindow);

    wxConfigBase *pConfig = wxConfigBase::Get();
    wxString lastPath = pConfig->GetPath();
    pConfig->SetPath(wxT("/MainFrame"));

    wxLogVerbose(wxT("Size ConfigSash"));
    w = pConfig->Read(wxT("tree_w"), 270);
    h = pConfig->Read(wxT("tree_h"), 190);
    m_configSashWindow->SetDefaultSize(wxSize(w > 50 ? w : 50, h > 50 ? h : 50));
    m_splitter->SetSashSize(5);
    int x = pConfig->Read(wxT("split_x"), 10);
    m_splitter->SetSashPosition(x < 10 ? 10 : x);

    wxLogVerbose(wxT("Size PropertySash"));
    w = pConfig->Read(wxT("prop_w"), 300);
    h = pConfig->Read(wxT("prop_h"), 115);
    m_propertiesSashWindow->SetDefaultSize(wxSize(w > 50 ? w : 50, h > 50 ? h : 50));

    wxLogVerbose(wxT("Size InfoSash"));
    w = pConfig->Read(wxT("desc_w"), 300);
    h = pConfig->Read(wxT("desc_h"), 75);
    m_infoSashWindow->SetDefaultSize(wxSize(w > 50 ? w : 50, h > 50 ? h : 50));

    wxLogVerbose(wxT("Size OutputSash"));
    w = pConfig->Read(wxT("outp_w"), 560);
    h = pConfig->Read(wxT("outp_h"), 90);
    m_outputSashWindow->SetDefaultSize(wxSize(w > 50 ? w : 50, h > 50 ? h : 50));

    pConfig->SetPath(lastPath);
    wxLogVerbose(wxT("+++++++++++++++ Windows created +++++++++++++++++"));
}

wxTextCtrl *CMainFrame::GetOutputWindow() const
{
    return m_outputWindow;
}

CConfigTree *CMainFrame::GetTreeCtrl() const
{
    return m_treeWindow;
}

CValueWindow *CMainFrame::GetValueWindow() const
{
    return m_valueWindow;
}

CInfoWindow *CMainFrame::GetInfoWindow() const
{
    return m_infoWindow;
}

CPropertyList *CMainFrame::GetPropertyListWindow() const
{
    return m_propertyListWindow;
}

CFindDialog *CMainFrame::GetFindDialog() const
{ 
    return m_findDlg; 
}

void CMainFrame::SetFindDialog(CFindDialog * dlg)
{ 
    m_findDlg = dlg;
}

void CMainFrame::OnSize(wxSizeEvent & WXUNUSED(event))
{
    wxLayoutAlgorithm layout;
    layout.LayoutFrame(this);
}

/*!
 * \brief Process sash drag event.
 *
 * 
 */
void CMainFrame::OnSashDrag(wxSashEvent & event)
{
    if (event.GetDragStatus() == wxSASH_STATUS_OUT_OF_RANGE)
        return;

    switch (event.GetId()) {
    case ID_CONFIG_SASH_WINDOW:
        /* Vertical sash dragged. */
        m_configSashWindow->SetDefaultSize(wxSize(event.GetDragRect().width, 2000));
        break;
    case ID_PROPERTIES_SASH_WINDOW:
        /* Right horizontal sash dragged. */
        m_propertiesSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
        break;
    case ID_SHORT_DESCR_SASH_WINDOW:
        m_infoSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
        break;
    case ID_OUTPUT_SASH_WINDOW:
        /* Lower horizontal sash dragged. */
        m_outputSashWindow->SetDefaultSize(wxSize(2000, event.GetDragRect().height));
        break;
    }

    wxLayoutAlgorithm layout;
    if (!layout.LayoutFrame(this)) {
        wxNode *node = (wxNode *) GetChildren().GetFirst();
        while (node) {
            wxWindow *win = (wxWindow *) node->GetData();
            if (win->IsKindOf(CLASSINFO(wxSashLayoutWindow))) {
                wxSashLayoutWindow *sashWin = (wxSashLayoutWindow *) win;
                wxSize sz = sashWin->GetSize();
                sashWin->SetDefaultSize(sz);
            }
            node = node->GetNext();
        }
    }
}

/* ------------------------- Menu Events ------------------------- */

void CMainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close();
}

void CMainFrame::OnSettings(wxCommandEvent& WXUNUSED(event))
{
    CSettingsDialog dialog(this);
    dialog.ShowModal();
}

void CMainFrame::OnFind(wxCommandEvent& WXUNUSED(event))
{
    if (m_findDlg) {
        m_findDlg->Raise();
    }
    else {
        int style = wxFR_NOUPDOWN;
        int flags = wxFR_DOWN;
        bool val;

        wxConfigBase *pConfig = wxConfigBase::Get();
        if (pConfig) {
            wxString lastPath = pConfig->GetPath();
            pConfig->SetPath(wxT("/FindDialog"));

            pConfig->Read(wxT("case"), &val, false);
            if (val) {
                flags |= wxFR_MATCHCASE;
            }
            pConfig->Read(wxT("word"), &val, false);
            if (val) {
                flags |= wxFR_WHOLEWORD;
            }

            pConfig->SetPath(lastPath);
        }

#if 0
        ctFindReplaceDialog::sm_findData.SetFlags(flags);
#endif
        m_findDlg = new CFindDialog(this, wxT("Find text"), style);
        m_findDlg->Show(true);
    }
}

void CMainFrame::OnGenerateBuildTree(wxCommandEvent & WXUNUSED(event))
{
    CNutConfDoc *doc = wxGetApp().GetNutConfDoc();
    CSettings *cfg = wxGetApp().GetSettings();
    wxString str;
    
    str += wxT("\nSource directory: ");
    str += cfg->m_source_dir;
    str += wxT("\nBuild directory: ");
    str += cfg->m_buildpath;
    str += wxT("\nTarget platform: ");
    str += cfg->m_platform;
    str += wxT("\n\nDo you like to generate this build tree?\n");
    if(wxMessageBox(str, wxT("Generate Build"), wxOK | wxCANCEL | wxICON_QUESTION, this) == wxOK) {
        if (doc) {
            doc->GenerateBuildTree();
        }
    }
}

/*!
 * \brief Building Nut/OS event.
 */
void CMainFrame::OnBuildNutOS(wxCommandEvent & WXUNUSED(event))
{
    CSettings *cfg = wxGetApp().GetSettings();
    wxString str;

    wxString lib_dir(cfg->m_lib_dir);
    if(lib_dir.IsEmpty()) {
        lib_dir = cfg->m_buildpath + wxT("/lib");
    }
    
    /*
     * Create a message box containing all relevant information and
     * let the user decide if he really wants that.
     */
    str += wxT("\nSource directory: ");
    str += cfg->m_source_dir;
    str += wxT("\nBuild directory: ");
    str += cfg->m_buildpath;
    str += wxT("\nTarget platform: ");
    str += cfg->m_platform;
    str += wxT("\nInstall directory: ");
    str += lib_dir;
    str += wxT("\n\nDo you like to build the Nut/OS libraries?\n");
    if(wxMessageBox(str, wxT("Build Nut/OS"), wxOK | wxCANCEL | wxICON_QUESTION, this) == wxOK) {
        /* Clean up any previous build. */
        if(!wxGetApp().Build(wxT("clean"))) {
            if(wxMessageBox(wxT("Cleaning build tree failed! Continue?"), wxT("Build"), 
                wxYES_NO | wxNO_DEFAULT | wxICON_HAND) != wxYES) {
                return;
            }
        }
        /* Start the new build. */
        if(!wxGetApp().Build()) {
            if(wxMessageBox(wxT("Building Nut/OS failed! Continue?"), wxT("Build"), 
                wxYES_NO | wxNO_DEFAULT | wxICON_HAND) != wxYES) {
                return;
            }
        }
        /* Install new libraries. */
        if(!wxGetApp().Build(wxT("install"))) {
            wxMessageBox(wxT("Installing Nut/OS failed!"), wxT("Build"), wxCANCEL | wxICON_HAND);
        }
    }
}

void CMainFrame::OnBuildLibraries(wxCommandEvent &event)
{
    OnGenerateBuildTree(event);
    OnBuildNutOS(event);
}

void CMainFrame::OnCreateSampleDir(wxCommandEvent & WXUNUSED(event))
{
    CNutConfDoc *doc = wxGetApp().GetNutConfDoc();
    CSettings *cfg = wxGetApp().GetSettings();
    wxString app_source = cfg->m_source_dir + wxT("/app");
    wxString str;
    
    str += wxT("\nSource directory: ");
    str += app_source;
    str += wxT("\nTarget directory: ");
    str += cfg->m_app_dir;
    str += wxT("\nProgrammer ");
    str += cfg->m_programmer;
    str += wxT("\n\nDo you like to create or update the sample directory?\n");
    if(wxMessageBox(str, wxT("Creating samples"), wxOK | wxCANCEL | wxICON_QUESTION, this) == wxOK) {
        if (doc) {
            doc->GenerateApplicationTree();
        }
    }
}

void CMainFrame::OnToggleToolbar(wxCommandEvent& WXUNUSED(event))
{
    wxToolBar *tbar = GetToolBar();

    if (tbar == NULL) {
        CreateNutToolBar();
    }
    else {
        delete tbar;

        SetToolBar(NULL);
    }
}

void CMainFrame::OnToggleToolbarSize(wxCommandEvent& WXUNUSED(event))
{
    wxToolBar *tbar = GetToolBar();

    if (tbar) {
        delete tbar;
        SetToolBar(NULL);
    }
    m_smallToolbar = !m_smallToolbar;
    CreateNutToolBar();
}

void CMainFrame::OnHelp(wxCommandEvent& WXUNUSED(event))
{
    m_help.DisplayContents();
}

void CMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxString message(wxT("Nut/OS Configurator "));
	message += wxT(VERSION);
    wxMessageBox(message, wxT("About"), wxOK | wxICON_INFORMATION, this);
}

