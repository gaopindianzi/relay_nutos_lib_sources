#ifndef SETTINGS_H_
#define SETTINGS_H_

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
 * Revision 1.7  2008/07/29 07:28:54  haraldkipp
 * The default installation path is an empty string.
 *
 * Revision 1.6  2007/09/11 13:43:22  haraldkipp
 * Top installation directory will be used for ICCAVR project configuration.
 * Re-building the application tree will no longer override existing project
 * files. Probably no longer in use, but _MCU_enhanced will be replaced by
 * _MCU_extended for ATmega256.
 *
 * Revision 1.5  2006/10/21 12:48:18  christianwelzel
 * Added support for multiple configurations / settings
 *
 * Revision 1.4  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.3  2004/09/17 13:03:48  haraldkipp
 * New settings page for tool options
 *
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/08/03 15:04:59  haraldkipp
 * Another change of everything
 *
 */

#include <wx/wx.h>
#include <wx/config.h>

class CSettings: public wxObject
{
DECLARE_DYNAMIC_CLASS(CSettings)
public:
    CSettings();
    ~CSettings();

    bool Load(wxString ConfigFileName);
    bool Save(wxString ConfigFileName);

    wxString FindAbsoluteDir(wxString refPathName);
    wxString FindRelativeDir(wxString refPathName);

    /*! \brief Top installation directory. */
    wxString m_topdir;
    /*! \brief Relative path to the top source directory. */
    wxString m_relsrcpath;
    /*! \brief Path to board specific configuration file. */
    wxString m_configname;
    wxString m_configname_default;
    /*! \brief Path to our main repository file. */
    wxString m_repositoryname;
    wxString m_repositoryname_default;
    /*! \brief Flag for multiple configurations. */
    bool m_mulConfig;
    bool m_mulConfig_default;
    /*! \brief First include directory path. */
    wxString m_firstidir;
    wxString m_firstidir_default;
    /*! \brief Last include directory path. */
    wxString m_lastidir;
    wxString m_lastidir_default;
    /*! \brief Path to the Nut/OS build tree. */
    wxString m_buildpath;
    wxString m_buildpath_default;
    /*! \brief Path to the Nut/OS libraries. */
    wxString m_lib_dir;
    /*! \brief Path to the Nut/OS source tree. */
    wxString m_source_dir;
    wxString m_source_dir_default;
    /*! \brief Target platform. */
    wxString m_platform;
    wxString m_platform_default;
    /*! \brief Path to our build tools. */
    wxString m_toolpath;
    wxString m_toolpath_default;
    /*! \brief Path to the Nut/OS application tree. */
    wxString m_app_dir;
    wxString m_app_dir_default;
    /*! \brief Target programming tool. */
    wxString m_programmer;
    wxString m_programmer_default;
};

#endif
