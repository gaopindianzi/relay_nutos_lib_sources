#ifndef _NUTCONF_H_
#define _NUTCONF_H_

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
 * $Log$
 * Revision 1.26  2009/03/08 17:44:33  haraldkipp
 * Fixed vertical scrollbar bug.
 *
 * Revision 1.25  2009/01/09 18:50:50  haraldkipp
 * Re-introduced support for Lua 5.0.
 *
 * Revision 1.24  2008/09/18 09:53:51  haraldkipp
 * GUI version 2.0.6.
 *
 * Revision 1.23  2008/08/29 15:03:04  haraldkipp
 * Fixed Configurator bug #2082123. Options now correctly enabled.
 *
 * Revision 1.22  2008/08/28 11:09:29  haraldkipp
 * Added Lua extension to query specific provisions.
 *
 * Revision 1.21  2008/08/20 07:00:34  haraldkipp
 * Incremented build number because of bad package. No code change.
 *
 * Revision 1.20  2008/07/29 07:39:37  haraldkipp
 * Fixed missing 'make install'. Bug was introduced in 2.0.1.
 *
 * Revision 1.19  2008/07/28 08:41:32  haraldkipp
 * Configurator accepts empty install path for using the default.
 *
 * Revision 1.18  2008/07/24 15:39:59  haraldkipp
 * Configurator version 2.0.
 *
 * Revision 1.17  2008/03/17 10:16:18  haraldkipp
 * Tool version 1.5.0. Starts working on OS X, though, still
 * problems with scrolling the split tree.
 *
 * Revision 1.16  2007/09/11 14:23:13  haraldkipp
 * Version 1.4.3: Mainly ICCAVR fixes and enhancements.
 *
 * Revision 1.15  2007/04/25 15:59:17  haraldkipp
 * Version 1.4.2 had been tested with wxWidgets 2.8.3.
 *
 * Revision 1.14  2007/02/15 19:33:45  haraldkipp
 * Version 1.4.1 works with wxWidgets 2.8.0.
 * Several wide character issues fixed.
 *
 * Revision 1.13  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.12  2005/07/26 15:57:18  haraldkipp
 * New keyword "default" to specify default option values.
 *
 * Revision 1.11  2005/07/20 09:22:39  haraldkipp
 * Release 1.2.2
 *
 * Revision 1.10  2005/04/22 15:17:50  haraldkipp
 * Version 1.2.1 can now run without GUI or with wxWidgets 2.5.5.
 *
 * Revision 1.9  2005/02/06 16:39:52  haraldkipp
 * GBA linker script entry in NutConf.mk fixed
 *
 * Revision 1.8  2004/11/24 15:36:53  haraldkipp
 * Release 1.1.1.
 * Do not store empty options.
 * Remove include files from the build tree, if they are no longer used.
 * Command line parameter 's' allows different settings.
 * Minor compiler warning fixed.
 *
 * Revision 1.7  2004/09/19 15:13:09  haraldkipp
 * Only one target per OBJx entry
 *
 * Revision 1.6  2004/09/17 13:03:48  haraldkipp
 * New settings page for tool options
 *
 * Revision 1.5  2004/09/07 19:19:12  haraldkipp
 * Tested with EB40A on Win32
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

#include "settings.h"
#include "nutconfdoc.h"
#include "mainframe.h"

#define VERSION "2.0.9"

class NutConfApp:public wxApp {
    friend class CMainFrame;
  public:
     virtual bool OnInit();
    virtual int OnExit();

    CNutConfDoc *GetNutConfDoc() const;
    CMainFrame *GetMainFrame() const;
    wxDocManager *GetDocManager() const;

    wxDocManager *m_docManager;
    CNutConfDoc *m_currentDoc;
    CMainFrame *m_mainFrame;

    void SetStatusText(const wxString & text);
    bool Launch(const wxString & strFileName, const wxString & strViewer);

    CSettings* GetSettings();
    bool Build(const wxString &target = wxT("all"));

  protected:
    CSettings* m_settings;
    wxString m_initialPath;
};


DECLARE_APP(NutConfApp);

#endif
