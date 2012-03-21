/*
 * Copyright (C) 2003-2005 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*
 * $Log: nutconf.cpp,v $
 * Revision 1.20  2009/01/15 17:59:04  haraldkipp
 * Fixed a path separator problem for Linux. The editor is now launched
 * again when double clicking the configured header file in the property list.
 *
 * Revision 1.19  2009/01/14 16:24:38  haraldkipp
 * Applied the patch contributed by Joerg Hermann. This finally
 * seems to fix the seg fault crash on KDE4 and Gnome 2.24.
 *
 * Revision 1.18  2008/09/18 09:53:22  haraldkipp
 * Ignore case when comparing the installation directory.
 *
 * Revision 1.17  2008/07/24 15:45:59  haraldkipp
 * More reliable splash screen appearance on Linux.
 *
 * Revision 1.16  2008/03/17 10:18:27  haraldkipp
 * Splash screen must be destroyed, not deleted.
 *
 * Revision 1.15  2007/09/11 13:43:22  haraldkipp
 * Top installation directory will be used for ICCAVR project configuration.
 * Re-building the application tree will no longer override existing project
 * files. Probably no longer in use, but _MCU_enhanced will be replaced by
 * _MCU_extended for ATmega256.
 *
 * Revision 1.14  2007/02/15 19:33:45  haraldkipp
 * Version 1.4.1 works with wxWidgets 2.8.0.
 * Several wide character issues fixed.
 *
 * Revision 1.13  2006/10/21 12:48:17  christianwelzel
 * Added support for multiple configurations / settings
 *
 * Revision 1.12  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.11  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.10  2005/07/22 18:47:15  haraldkipp
 * Online help added and Copyright year updated.
 *
 * Revision 1.9  2005/07/20 09:22:18  haraldkipp
 * Make mime types work on Linux
 *
 * Revision 1.8  2005/04/22 15:16:57  haraldkipp
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.7  2004/11/24 15:36:53  haraldkipp
 * Release 1.1.1.
 * Do not store empty options.
 * Remove include files from the build tree, if they are no longer used.
 * Command line parameter 's' allows different settings.
 * Minor compiler warning fixed.
 *
 * Revision 1.6  2004/09/26 12:04:07  drsung
 * Fixed several hundred memory leaks :-).
 * Relative pathes can now be used for source, build and install directory.
 *
 * Revision 1.5  2004/09/17 13:09:29  haraldkipp
 * New settings page for tool options
 *
 * Revision 1.4  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.3  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.2  2004/06/07 16:08:07  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/app.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#endif

#include <wx/notebook.h>
#include <wx/config.h>
#include <wx/mimetype.h>
#include <wx/cmdline.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/splash.h>
#include <wx/bitmap.h>
#include <wx/busyinfo.h>

#include <wx/filesys.h>
#include <wx/fs_zip.h>


#include "mainframe.h"
#include "nutconf.h"

#if !defined(__WXMSW__)
#include "bitmaps/nutconf.xpm"
#endif

IMPLEMENT_APP(NutConfApp);

/*!
 * \brief Application main entry.
 */
bool NutConfApp::OnInit()
{
    m_docManager = NULL;
    m_currentDoc = NULL;
    wxString settings_name(wxT("NutConf"));
    wxString settings_ext;

    static const wxCmdLineEntryDesc cmdLineDesc[] =
    {
        { wxCMD_LINE_SWITCH, wxT("v"), _T("verbose"), _T("be verbose"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
        { wxCMD_LINE_SWITCH, wxT("h"), _T("help"), _T("show usage"), wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
        { wxCMD_LINE_OPTION, wxT("s"), _T("settings"), _T("use alternate settings"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_NEEDS_SEPARATOR },
        { wxCMD_LINE_NONE }
    };

    wxCmdLineParser parser(cmdLineDesc, argc, argv);
    parser.SetLogo(_T("Nut/OS Configurator\n") _T(VERSION)
                      _T("Copyright (c) 2004-2007 by egnite Software GmbH\n")
                      _T("Copyright (C) 1998, 1999, 2000 Red Hat, Inc."));
    if(parser.Parse()) {
        return false;
    }
    if(parser.Found(wxT("v"))) {
        wxLog::GetActiveTarget()->SetVerbose();
    }
    else {
        wxLog::GetActiveTarget()->SetVerbose(false);
    }

    /*
     * Splash display.
     */
    wxBitmap bmp(wxBITMAP(SSB_NUTCONF));
    wxSplashScreen* splash = new wxSplashScreen(bmp,
          wxSPLASH_CENTRE_ON_SCREEN,
          0, NULL, -1, wxDefaultPosition, wxDefaultSize,
          wxSIMPLE_BORDER|wxSTAY_ON_TOP);
    wxYield();

    wxImage::AddHandler(new wxGIFHandler);
    wxImage::AddHandler(new wxPNGHandler);
    wxFileSystem::AddHandler(new wxZipFSHandler);

    /*
     * Load settings early.
     */
    SetVendorName(wxT("egnite"));
    if(parser.Found(wxT("s"), &settings_ext)) {
        settings_name += settings_ext;
    }
    SetAppName(settings_name);
    m_settings = new CSettings();
    m_settings->m_topdir = ::wxGetCwd();

    /*
     * Create a wxConfig object early.
     */
    argc = 1;
    if (!wxApp::OnInit())
        return false;

    wxGetEnv(wxT("PATH"), &m_initialPath);

    /*
     * The document manager will handle non application specific menu commands.
     */
    m_docManager = new wxDocManager;

    /*
     * The document template defines the relationship between document and view.
     */
    new wxDocTemplate(m_docManager, wxT("Nut/OS Configuration"), wxT("*.conf"), wxT(""), wxT("conf"), wxT("NutconfDoc"), wxT("NutconfView"),
                                             CLASSINFO(CNutConfDoc), CLASSINFO(CNutConfView));
    m_docManager->SetMaxDocsOpen(1);

    m_mainFrame = new CMainFrame(m_docManager, wxT("Nut/OS Configurator"));

    SetTopWindow(m_mainFrame);
    m_mainFrame->Show();

    if (!m_mainFrame->GetHelpController().Initialize(wxT("nutoshelp"))) {
        wxLogMessage(wxT("Failed to load help file"));
    }

    /* 
     * Detect directory change (version upgrade). 
     */
    wxString initWork = m_settings->FindAbsoluteDir(wxT("conf/repository.nut"));
    if (!initWork.IsEmpty()) {
        m_docManager->SetLastDirectory(initWork + wxT("/conf"));
        if (::wxIsAbsolutePath(m_settings->m_configname)) {
            if (!m_settings->m_configname.Lower().StartsWith(initWork.Lower())) {
                if (wxMessageBox(wxT("Configuration path has changed.\n\nOld path was ") +
                    wxPathOnly(m_settings->m_configname) + wxT("\nNew path is ") + initWork + wxT("/conf\n\nSwitch to new path?"), 
                    wxT("Path Change"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) {
                        m_settings->m_configname = m_settings->m_relsrcpath + wxString(wxT("/conf/")) + m_settings->m_configname.AfterLast('/');
                        m_settings->m_repositoryname = m_settings->m_repositoryname_default;
                        m_settings->Save(m_settings->m_configname);
                }
            }
        }
        if (::wxIsAbsolutePath(m_settings->m_repositoryname)) {
            if (!m_settings->m_repositoryname.StartsWith(initWork)) {
                if (wxMessageBox(wxT("Repository path has changed. Do you want to use relative paths?"), 
                    wxT("Path Change"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION) == wxYES) {
                        m_settings->m_repositoryname = m_settings->m_repositoryname_default;
                        m_settings->Save(m_settings->m_configname);
                }
            }
        }
    }

    /*
     * Create the document. 
     */
    m_docManager->CreateDocument(m_settings->m_configname, 0);

    if(splash) {
        splash->Destroy();
    }
    return true;
}

int NutConfApp::OnExit()
{
    delete wxConfigBase::Set((wxConfigBase *) NULL);
    if(m_docManager) {
        delete m_docManager;
    }
    if(m_settings) {
        delete m_settings;
    }
    return 0;
}

CNutConfDoc *NutConfApp::GetNutConfDoc() const
{
    if (m_currentDoc)
        return m_currentDoc;

    if (!m_docManager)
        return NULL;

    return wxDynamicCast(m_docManager->GetCurrentDocument(), CNutConfDoc);
}

void NutConfApp::SetStatusText(const wxString & text)
{
    CMainFrame *mainFrame = GetMainFrame();
    if (mainFrame) {
        mainFrame->GetStatusBar()->SetStatusText(text, 0);
#ifdef __WXMSW__
        ::UpdateWindow((HWND) mainFrame->GetHWND());
#endif
    }
}

CMainFrame *NutConfApp::GetMainFrame() const
{
    return m_mainFrame;
}

wxDocManager *NutConfApp::GetDocManager() const
{
    return m_docManager;
}

CSettings* NutConfApp::GetSettings() 
{ 
    return m_settings; 
}

bool NutConfApp::Launch(const wxString & strFileName, const wxString & strViewer)
{
    bool ok = false;
    wxString cmd;
    wxString filePath(strFileName);

    filePath.Replace(wxT("/"), wxString(wxFileName::GetPathSeparator()));

    if (!strViewer.IsEmpty()) {
        cmd = strViewer + wxString(wxT(" ")) + filePath;
    } else {
        wxString path, filename, ext;
        wxSplitPath(filePath, &path, &filename, &ext);

        wxFileType *ft = wxTheMimeTypesManager->GetFileTypeFromExtension(ext);
        if (ft == NULL) {
            ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT(".txt"));
            if (ft == NULL) {
                return false;
            }
        }

        ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(filePath, wxT("")));
        delete ft;

        if (!ok) {
            ft = wxTheMimeTypesManager->GetFileTypeFromExtension(wxT(".txt"));
            ok = ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(filePath, wxT("")));
            delete ft;
        }
    }

    if (ok) {
        ok = (wxExecute(cmd, false) != 0);
    }
    return ok;
}

bool NutConfApp::Build(const wxString &target)
{
    CNutConfDoc *doc = GetNutConfDoc();
    if(doc == NULL) {
        return false;
    }

    /*
     * Change working directory to the top build directory.
     */
    wxString initialCwd = wxFileName::GetCwd();
    if (::wxIsAbsolutePath(m_settings->m_buildpath)) {
        wxFileName::SetCwd(m_settings->m_buildpath);
    }
    else {
        wxFileName::SetCwd(m_settings->m_relsrcpath + wxString(wxT("/../")) + m_settings->m_buildpath);
    }

    /* Add tool directories to the PATH. */
#ifdef _WIN32
    wxString newPath(m_settings->m_toolpath);
    newPath.Replace(wxT("/"), wxT("\\"));
    newPath += wxT(";");
    newPath += m_initialPath;
    wxSetEnv(wxT("PATH"), newPath);
#endif

    /*
     * Assemble the command.
     */
    wxString cmd = wxT("make ") + target;

    /*
     * Start execution. stdout and stderr output is collected
     * in string arrays. Not very nice here, but works for now.
     */
    wxLogMessage(wxT("----- Running '%s' -----"), cmd.c_str());

#ifdef _WIN32
    wxPathList exePaths;
    wxString exePathName;

    exePaths.AddEnvList(wxT("PATH"));
    exePathName = exePaths.FindAbsoluteValidPath(_T("make.exe"));
    if (exePathName.IsEmpty()) {
        wxLogMessage(wxT("Make tool not found in ") + newPath);
    }
    wxLogMessage(wxT("Located ") + exePathName);
#endif

    wxBusyInfo wait(wxT("Please wait, running '") + cmd + wxT("'..."));
    wxArrayString output, errors;
    int code = wxExecute(cmd, output, errors);

    /*
     * Display collected output in the output window.
     */
    if(code != -1) {
        wxTextCtrl *outwin = wxGetApp().GetMainFrame()->GetOutputWindow();
        size_t i;
        size_t count;

        if(wxLog::GetVerbose()) {
            count = output.GetCount();
            for (i = 0; i < count; i++) {
                outwin->AppendText(output[i]);
                outwin->AppendText(wxT("\n"));
                outwin->ShowPosition(outwin->GetLastPosition());
            }
        }
        count = errors.GetCount();
        for (i = 0; i < count; i++) {
            outwin->AppendText(errors[i]);
            outwin->AppendText(wxT("\n"));
            outwin->ShowPosition(outwin->GetLastPosition());
        }
    }
    if(code) {
        wxLogMessage(wxT("----- '%s' failed with error %d -----"), cmd.c_str(), code);
    }
    else {
        wxLogMessage(wxT("----- '%s' terminated successfully -----"), cmd.c_str(), code);
    }

    /* 
     * Restore initial environment. 
     */
    wxSetEnv(wxT("PATH"), m_initialPath);
    wxFileName::SetCwd(initialCwd);

    return (code == 0);
}
