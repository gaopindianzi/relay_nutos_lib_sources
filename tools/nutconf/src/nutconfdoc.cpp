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
 * $Log: nutconfdoc.cpp,v $
 * Revision 1.28  2009/01/14 16:25:46  haraldkipp
 * Already done by Ole. Anyway, removed the commented lines to make Thiago
 * happy. ;-)
 *
 * Revision 1.27  2009/01/13 21:04:10  olereinhardt
 * 2009-01-13  Ole Reinhardt <ole.reinhardt@thermotemp.de>
 *
 * 	* tools/nutconf/nutconfdoc.cpp: Commented out inline destructor of
 * 	  CAbstractDirCopyTraverser to fix compilation bug on linux
 *
 * Revision 1.26  2009/01/04 04:52:39  thiagocorrea
 * Add .svn to ignore list when copying files and reduce some duplicated code.
 *
 * Revision 1.25  2009/01/04 04:30:49  thiagocorrea
 * Allow nutconf to build with _UNICODE under Win32.
 *
 * Revision 1.24  2008/09/18 09:54:26  haraldkipp
 * Fixed memory holes.
 * Corrected ICCAVR option value retrieval.
 *
 * Revision 1.23  2008/07/29 07:32:25  haraldkipp
 * Removed unsupported Eclipse project copying.
 *
 * Revision 1.22  2008/07/28 08:40:20  haraldkipp
 * Temporarly removed unsupported Eclipse project copying. We are testing a
 * new setup for Eclipse.
 *
 * Revision 1.21  2008/07/24 15:41:41  haraldkipp
 * Dynamic configuration.
 *
 * Revision 1.20  2008/03/17 10:19:06  haraldkipp
 * Added Eclipse project file copying (experimental).
 *
 * Revision 1.19  2007/09/11 13:43:22  haraldkipp
 * Top installation directory will be used for ICCAVR project configuration.
 * Re-building the application tree will no longer override existing project
 * files. Probably no longer in use, but _MCU_enhanced will be replaced by
 * _MCU_extended for ATmega256.
 *
 * Revision 1.18  2007/04/25 16:03:33  haraldkipp
 * Bugfix: Configurator failed to create application directories containing
 * subdirs.
 *
 * Revision 1.17  2007/02/15 19:36:03  haraldkipp
 * Wide character issues fixed.
 * Mkdir no more creates the full path without trailing separator.
 *
 * Revision 1.16  2006/10/21 12:48:18  christianwelzel
 * Added support for multiple configurations / settings
 *
 * Revision 1.15  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.14  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.13  2005/10/07 22:12:28  hwmaier
 * Added bld_dir parameter to CreateSampleDirectory.
 *
 * Revision 1.12  2005/08/14 16:10:18  christianwelzel
 * Avoid compiler warnings under cygwin.
 *
 * Revision 1.11  2005/07/26 16:39:09  haraldkipp
 * Do not store default values.
 *
 * Revision 1.10  2005/04/22 15:19:45  haraldkipp
 * Added support for building ICCAVR applications in the sample tree.
 *
 * Revision 1.9  2004/11/24 15:36:53  haraldkipp
 * Release 1.1.1.
 * Do not store empty options.
 * Remove include files from the build tree, if they are no longer used.
 * Command line parameter 's' allows different settings.
 * Minor compiler warning fixed.
 *
 * Revision 1.8  2004/11/08 10:21:26  drsung
 * While creating the sample directory, CVS files are now not copied.
 *
 * Revision 1.7  2004/09/26 12:04:07  drsung
 * Fixed several hundred memory leaks :-).
 * Relative pathes can now be used for source, build and install directory.
 *
 * Revision 1.6  2004/09/19 15:12:22  haraldkipp
 * Set mod flag on all changes
 *
 * Revision 1.5  2004/09/17 13:02:39  haraldkipp
 * First and last directory added to sample dir
 *
 * Revision 1.4  2004/09/07 19:20:07  haraldkipp
 * Initial/default lib/inc dirs updated
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

#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include "nutconf.h"
#include "treeitemdata.h"
#include "nutconfhint.h"
#include "nutconfdoc.h"

#ifdef __WXMSW__
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

/*
 * The doc/view framework will create instances of this class dynamically.
 */
IMPLEMENT_DYNAMIC_CLASS(CNutConfDoc, wxDocument);

/*!
 * \brief Default constructor.
 */
CNutConfDoc::CNutConfDoc()
{
    m_root = NULL;
}

/*!
 * \brief Destructor.
 */
CNutConfDoc::~CNutConfDoc()
{
    if (m_repository) {
        CloseRepository(m_repository);
    }
    ReleaseRepository();
    DeleteItems();
}

/*!
 * \brief Create a document from a specified file.
 *
 * \param path Pathname of the configuration file.
 */
bool CNutConfDoc::OnCreate(const wxString & path, long flags)
{
    bool rc = false;
    wxString normPath(path);
    CSettings *cfg = wxGetApp().GetSettings();

    normPath.Replace(wxT("\\"), wxT("/"));
    cfg->m_configname = normPath;

    wxGetApp().m_currentDoc = this;

    /*
     * The repository may refer to certain configuration values. Thus, 
     * it must be loaded before reading the repository.
     */
    cfg->Load(cfg->m_configname);
    if ((rc = ReadRepository(cfg->m_repositoryname, normPath)) == true) {

        Modify(false);
        SetDocumentSaved(false);

        rc = wxDocument::OnCreate(path, flags);
        if (rc) {
            if (flags & wxDOC_NEW) {
                wxBusyCursor wait;

                CNutConfHint hint(NULL, nutSelChanged);
                UpdateAllViews(NULL, &hint);

                SetFilename(GetFilename(), true);
            }
        }
    }

    if(!rc) {
        wxGetApp().m_currentDoc = NULL;
    }
    return rc;
}

/*!
 * \brief Create an empty document.
 *
 * \todo Doesn't do anything right now.
 */
bool CNutConfDoc::OnNewDocument()
{
    return true;
}

/*!
 * \brief Open an existing document.
 *
 */
bool CNutConfDoc::OnOpenDocument(const wxString & filename)
{
    /*
     * Check if the current document has been modified.
     */
    if (!OnSaveModified())
        return false;

    SetFilename(filename, true);
    Modify(false);
    UpdateAllViews();

    return true;
}

void CNutConfDoc::SaveComponentOptions(FILE *fp, NUTCOMPONENT * compo)
{
    while (compo) {
        NUTCOMPONENTOPTION *opts = compo->nc_opts;
        while (opts) {
            if(opts->nco_enabled && opts->nco_active) {
                char *value;
                if (opts->nco_value) {
                    /* Save edited value. */
                    value = strdup(opts->nco_value);
                } else {
                    /* Save configured value. */
                    value = GetConfigValue(m_repository, opts->nco_name);
                }
                /* Do not save empty values, unless they are boolean. */
                if (value && *value == '\0') {
                    char *flavor = GetOptionFlavour(m_repository, opts->nco_compo, opts->nco_name);
                    if (flavor == NULL || strncasecmp(flavor, "bool", 4)) {
                        free(value);
                        value = NULL;
                    }
                }
                if (value) {
                    wxString escapedValue = wxString::FromAscii( value );
                    escapedValue.Replace(wxT("\""), wxT("\\\"")); // escape (") to (\");
                    //fprintf(fp, "%s = \"%s\"\n", opts->nco_name, escapedValue.c_str());
                    fprintf(fp, "%s = \"%s\"\n", opts->nco_name, (const char*) escapedValue.mb_str(wxConvUTF8));

                    free(value);
                }
            }
            opts = opts->nco_nxt;
        }
        SaveComponentOptions(fp, compo->nc_child);
        compo = compo->nc_nxt;
    }
}

/*!
 * \brief Save the config file.
 *
 * \param filename Name of the config file including path.
 *
 * \return true on success, otherwise false is returned.
 */
bool CNutConfDoc::OnSaveDocument(const wxString& filename)
{
    if (filename.IsEmpty()) {
        return false;
    }

    FILE *fp = fopen(filename.mb_str(), "w");
    if (fp) {
        SaveComponentOptions(fp, m_root->nc_child);
        fclose(fp);
        Modify(false);
        SetFilename(filename);
        return true;
    }
    return false;
}

/*!
 * \brief Close the document.
 *
 * \return true on success, otherwise false is returned.
 */
bool CNutConfDoc::OnCloseDocument()
{
    if (wxDocument::OnCloseDocument()) {
        DeleteItems();
        return true;
    }
    return false;
}


void CNutConfDoc::ReleaseRepository()
{
	if (m_root)
		ReleaseComponents(m_root);
}

/*
 * Delete all items.
 */
void CNutConfDoc::DeleteItems()
{
    wxNode *node = m_items.GetFirst();
    while (node) {
        wxNode *next = node->GetNext();
        node = next;
    }
}

/*!
 * \brief Read the Configurator's repository.
 *
 * \param repositoryname Pathname of the repository file.
 * \param configname     Pathname of the configuration file.
 *
 * \return true on success, otherwise false is returned.
 */
bool CNutConfDoc::ReadRepository(const wxString & repositoryname, const wxString & configname)
{
    wxBusyCursor wait;
    wxString str;

    str = wxT("Loading ") + repositoryname;
    wxGetApp().SetStatusText(str);
    wxLogMessage(wxT("%s"), str.c_str());

    m_repository = OpenRepository(repositoryname.mb_str());
    if(m_repository) {
        m_root = LoadComponents(m_repository);
        if(m_root) {
            str = wxT("Loading ") + configname;
            wxGetApp().SetStatusText(str);
            wxLogMessage(wxT("%s"), str.c_str());
            if(ConfigureComponents(m_repository, m_root, configname.mb_str())) {
                wxLogMessage(wxT("%s"), GetScriptErrorString());
            }
            else {
                CSettings *cfg = wxGetApp().GetSettings();
                /* Store settings in the Lua registry. */
                RegisterSourcePath(m_repository, cfg->m_source_dir.mb_str());
                RegisterBuildPath(m_repository, cfg->m_buildpath.mb_str());
                RegisterLibPath(m_repository, cfg->m_lib_dir.mb_str());
                RegisterSamplePath(m_repository, cfg->m_app_dir.mb_str());
                RegisterCompilerPlatform(m_repository, cfg->m_platform.mb_str());
                if (RefreshComponents(m_repository, m_root)) {
                    wxLogError(wxT("Conflicting configuration"));
                } else {
                    wxLogMessage(wxT("OK"));
                }
            }
        }
        else {
            wxLogMessage(wxT("%s"), GetScriptErrorString());
        }
//        CloseRepository(m_repository);
    }
    else {
        wxLogError(wxT("Failed to open repository"));
    }

    wxDocument::OnNewDocument();
    AddAllItems();
    wxGetApp().SetStatusText(wxEmptyString);

    return true;
}

/*!
 * \brief Return the repository info.
 */
NUTREPOSITORY * CNutConfDoc::GetRepository()
{
    return m_repository;
}

/*!
 * \brief Return the component root.
 */
NUTCOMPONENT * CNutConfDoc::GetRootComponent()
{
    return m_root;
}

/*!
 *
 */
void CNutConfDoc::AddChildItems(NUTCOMPONENT * compo, wxTreeItemId parent)
{
    CConfigTree *treeCtrl = wxGetApp().GetMainFrame()->GetTreeCtrl();

    if(compo) {
        compo = compo->nc_child;
        while (compo) {
            CConfigItem *item = new CConfigItem(NULL, compo);

            wxTreeItemId childId = treeCtrl->AppendItem(parent, wxT(""), -1, -1, new CTreeItemData(item));
            item->SetTreeItem(childId);
            item->UpdateTreeItem(*treeCtrl);
            m_items.Append(item);


            NUTCOMPONENTOPTION *opts = compo->nc_opts;
            while (opts) {
                item = new CConfigItem(item, opts);
                wxTreeItemId optId = treeCtrl->AppendItem(childId, wxT(""), -1, -1, new CTreeItemData(item));
                item->SetTreeItem(optId);
                item->UpdateTreeItem(*treeCtrl);
                m_items.Append(item);
                opts = opts->nco_nxt;
            }

            AddChildItems(compo, childId);
            compo = compo->nc_nxt;
        }
    }
}

/*!
 * \brief Create all configuration items.
 */
void CNutConfDoc::AddAllItems()
{
    CConfigTree *treeCtrl = wxGetApp().GetMainFrame()->GetTreeCtrl();

    wxGetApp().GetMainFrame()->GetPropertyListWindow()->Fill(NULL);

    treeCtrl->DeleteAllItems();

    CConfigItem *item = new CConfigItem();
    wxTreeItemId rootId = treeCtrl->AddRoot(wxT(""), -1, -1, new CTreeItemData(item));

    item->SetTreeItem(rootId);
    item->UpdateTreeItem(*treeCtrl);
    m_items.Append(item);

    AddChildItems(m_root, rootId);

    UpdateAllViews();

    if (GetItems().GetCount() > 0) {
        wxGetApp().GetMainFrame()->GetTreeCtrl()->Expand(rootId);
    }
    wxGetApp().GetMainFrame()->GetTreeCtrl()->SetFocus();
}

/*!
 * \brief Return the list of items.
 */
wxList & CNutConfDoc::GetItems()
{
    return m_items;
}

/*!
 * \brief Return a specified item.
 */
CConfigItem *CNutConfDoc::GetItem(size_t i)
{
    return (CConfigItem *) m_items[i];
}


/*!
 * \brief Find NUTCOMPONENTOPTION by name.
 */
NUTCOMPONENTOPTION *CNutConfDoc::FindOptionByName(NUTCOMPONENT * compo, char *name)
{
    if (compo == NULL && m_root) {
        compo = m_root->nc_child;
    }
    while (compo) {
        NUTCOMPONENTOPTION *opts = compo->nc_opts;
        while (opts) {
            if (strcmp(opts->nco_name, name) == 0) {
                return opts;
            }
            opts = opts->nco_nxt;
        }
        if (compo->nc_child && (opts = FindOptionByName(compo->nc_child, name)) != NULL) {
            return opts;
        }
        compo = compo->nc_nxt;
    }
    return NULL;
}

/*!
 * \brief Return the activation state of an item with a specified name.
 */
bool CNutConfDoc::IsOptionActive(char *name)
{
    NUTCOMPONENTOPTION *opt = FindOptionByName(m_root->nc_child, name);

    if (opt && opt->nco_active) {
        return true;
    }
    return false;
}

bool CNutConfDoc::IsRequirementProvided(NUTCOMPONENT *compo, char *requirement)
{
    int i;

    while (compo) {
        if(compo->nc_enabled) {
            char **cprovides = GetComponentProvisions(m_repository, compo);
            if (cprovides) {
                for (i = 0; cprovides[i]; i++) {
                    if(strcmp(cprovides[i], requirement) == 0) {
                        break;
                    }
                    free(cprovides[i]);
                }
                if (cprovides[i]) {
                    for (; cprovides[i]; i++) {
                        free(cprovides[i]);
                    }
                    free(cprovides);
                    return true;
                }
                free(cprovides);
            }

            NUTCOMPONENTOPTION *opts = compo->nc_opts;
            while (opts) {
                char **provides = GetOptionProvisions(m_repository, opts->nco_compo, opts->nco_name);
                if (provides) {
                    for (i = 0; provides[i]; i++) {
                        if(strcmp(provides[i], requirement) == 0) {
                            if(opts->nco_active) {
                                break;
                            }
                        }
                        free(provides[i]);
                    }
                    if (provides[i]) {
                        for (; provides[i]; i++) {
                            free(provides[i]);
                        }
                        free(provides);
                        return true;
                    }
                    free(provides);
                }
                opts = opts->nco_nxt;
            }
            if (IsRequirementProvided(compo->nc_child, requirement)) {
                return true;
            }
        }
        compo = compo->nc_nxt;
    }
    return false;
}

bool CNutConfDoc::IsRequirementProvided(char *requirement)
{
    return IsRequirementProvided(m_root, requirement);
}

void CNutConfDoc::DeactivateOptionList(NUTCOMPONENT *compo, char **exlist)
{
    if (exlist) {
        if (compo == NULL) {
            compo = m_root->nc_child;
        }
        for (int i = 0; exlist[i]; i++) {
            NUTCOMPONENTOPTION *opt = FindOptionByName(compo, exlist[i]);
            if (opt) {
                opt->nco_active = 0;
            }
        }
    }
}

/*!
 * \brief Set value of an integer item.
 */
bool CNutConfDoc::SetValue(CConfigItem & item, long nValue)
{
    wxString str;
    str.Printf(wxT("%ld"), nValue);
    return SetValue(item, str);
}

/*!
 * \brief Set value of a string item.
 */
bool CNutConfDoc::SetValue(CConfigItem & item, const wxString & strValue)
{
    if (item.m_option) {
        char *newval = strdup(strValue.mb_str());

        /* Check if edited value changed. */
        if (item.m_option->nco_value == NULL || strcmp(item.m_option->nco_value, newval)) {
            /* Remove any previously edited value. */
            if (item.m_option->nco_value) {
                free(item.m_option->nco_value);
                item.m_option->nco_value = NULL;
            }
            /* Check if new value differs from configured value. */
            char *cfgval = GetConfigValue(m_repository, item.m_option->nco_name);
            if ((cfgval == NULL && *newval) || (cfgval && strcmp(cfgval, newval))) {
                item.m_option->nco_value = newval;
                item.m_option->nco_active = 1;
                Modify(true);
            } else {
                free(newval);
            }
            if (cfgval) {
                free(cfgval);
            }
            CNutConfHint hint(&item, nutValueChanged);
            UpdateAllViews(NULL, &hint);
        } else {
            free(newval);
        }
    }
    return true;
}

/*!
 * \brief Set value of a boolean item.
 */
bool CNutConfDoc::SetActive(CConfigItem & item, bool bEnabled)
{
    item.SetActive(bEnabled);

    /* Store settings in the Lua registry. */
    CSettings *cfg = wxGetApp().GetSettings();
    RegisterSourcePath(m_repository, cfg->m_source_dir.mb_str());
    RegisterBuildPath(m_repository, cfg->m_buildpath.mb_str());
    RegisterLibPath(m_repository, cfg->m_lib_dir.mb_str());
    RegisterSamplePath(m_repository, cfg->m_app_dir.mb_str());
    RegisterCompilerPlatform(m_repository, cfg->m_platform.mb_str());

    RefreshComponents(m_repository, m_root);
    Modify(true);
    CNutConfHint hint(&item, nutExternallyChanged);
    UpdateAllViews(NULL, &hint);

    return true;
}

/*!
 * \brief Return build tree path.
 */
wxString CNutConfDoc::GetBuildTree()
{
    return wxGetApp().GetSettings()->m_buildpath;
}

/*!
 * \brief Return build tree path.
 */
wxString CNutConfDoc::GetInstallDir()
{
    CSettings *cfg = wxGetApp().GetSettings();

    wxString ins_dir(cfg->m_lib_dir);
    if(ins_dir.IsEmpty()) {
        ins_dir = cfg->m_buildpath + wxT("/lib");
    }
    return ins_dir;
}

/*!
 * \brief Create all header amd make files.
 *
 * \return true on success, otherwise false.
 */
bool CNutConfDoc::GenerateBuildTree()
{
    CSettings *cfg = wxGetApp().GetSettings();

    wxBusyCursor wait;

    wxLogMessage(wxT("Creating Makefiles for %s in %s"), cfg->m_platform.c_str(), cfg->m_buildpath.c_str());
    if(CreateMakeFiles(m_repository, m_root, cfg->m_buildpath.mb_str(), cfg->m_source_dir.mb_str(),
                       cfg->m_platform.mb_str(), cfg->m_firstidir.mb_str(), cfg->m_lastidir.mb_str(),
                       GetInstallDir().mb_str())) {
        return false;
    }

    wxLogMessage(wxT("Creating header files in %s"), cfg->m_buildpath.c_str());
    if(CreateHeaderFiles(m_repository, m_root, cfg->m_buildpath.mb_str())) {
        return false;
    }
    wxLogMessage(wxT("OK"));
    return true;
}

class CAbstractDirCopyTraverser : public wxDirTraverser
{
public:
	CAbstractDirCopyTraverser() {}

	virtual bool isSCMdir(const wxString& dirname) const
	{
		if(dirname.EndsWith(wxString(wxT("CVS")) + wxFileName::GetPathSeparator()) || 
		   dirname.EndsWith(wxString(wxT(".svn")) + wxFileName::GetPathSeparator()) ) {
			return true;
		}
		return false;
	}
};

class CDirCopyTraverser : public CAbstractDirCopyTraverser
{
public:
    CDirCopyTraverser(wxString source, wxString target)
    : m_source(source)
    , m_target(target)
    {
    }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        wxString sub = filename.Mid(m_source.Length());
        wxFileName name(m_target + sub);

        /* Do not copy Makefiles for source tree builds. */
        if(!name.GetName().IsSameAs(wxT("Makedefs"), false) &&
            !name.GetName().IsSameAs(wxT("Makerules"), false) &&
            !name.GetName().IsSameAs(wxT("Makeburn"), false) &&
            !name.FileExists()) { /* Do not overwrite any existing source file. */
            ::wxCopyFile(filename, name.GetFullPath());
        }
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        wxString sub = dirname.Mid(m_source.Length());
        wxFileName name(m_target + sub + wxFileName::GetPathSeparator(), wxEmptyString);
        if(!isSCMdir(name.GetFullPath())) {
            name.Mkdir(0777, wxPATH_MKDIR_FULL);
            return wxDIR_CONTINUE;
        }
        return wxDIR_IGNORE;
    }
private:
    wxString m_source;
    wxString m_target;
};

#ifdef _WIN32
class CDirIccAvrProjectTraverser : public CAbstractDirCopyTraverser
{
public:
    CDirIccAvrProjectTraverser(wxString source, wxString target)
    : m_source(source)
    , m_target(target)
    {
        m_doc = wxGetApp().GetNutConfDoc();
    }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        wxString sub = filename.Mid(m_source.Length());
        wxFileName name(m_target + sub);
        /* Create project source list, but do not override exiting list. */
        if(name.GetExt().IsSameAs(wxT("src"), false) && !name.FileExists()) {
            TransferSourcesFile(filename, name.GetFullPath());
        }
        if(name.GetExt().IsSameAs(wxT("prj"), false) && !name.FileExists()) {
            TransferProjectFile(filename, name.GetFullPath());
        }
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        wxString sub = dirname.Mid(m_source.Length());
        wxFileName name(m_target + sub + wxFileName::GetPathSeparator(), wxEmptyString);
		if(!isSCMdir(name.GetFullPath())) {
            name.Mkdir(0777, wxPATH_MKDIR_FULL);
            return wxDIR_CONTINUE;
        }
        return wxDIR_IGNORE;
    }
private:
    wxString m_source;
    wxString m_target;
    CNutConfDoc *m_doc;

    bool TransferProjectFile(wxString source, wxString target)
    {
        bool in_section = false;

        wxFileInputStream f_source(source);
        if (!f_source.Ok()) {
            wxLogMessage(wxT("Failed to read from %s"), source.c_str());
            return false;
        }
        wxFileOutputStream f_target(target);
        if (!f_target.Ok()) {
            wxLogMessage(wxT("Failed to write to %s"), target.c_str());
            return false;
        }

        CSettings *cfg = wxGetApp().GetSettings();
        wxTextInputStream istream(f_source);
        wxTextOutputStream ostream(f_target);
        for(;;) {
            wxString line = istream.ReadLine();

            if (f_source.Eof() && line.IsEmpty()) {
                break;
            }
            if (in_section) {
                if (line.StartsWith(wxT("Edit1="))) {
                    line.Empty();
                    /* If we have a first include path, add it first. */
                    if(!cfg->m_firstidir.IsEmpty()) {
                        line += cfg->m_firstidir + wxT(";");
                    }
                    /* Add the include path of the build tree. */
                    if (!::wxIsAbsolutePath(cfg->m_buildpath)) {
                        line += cfg->m_topdir + wxT("\\");
                    }
					line += cfg->m_buildpath + wxT("\\include;");
                    /* Add the ICC specific include path of the source tree. */
                    if (!::wxIsAbsolutePath(cfg->m_source_dir)) {
                        line += cfg->m_topdir + wxT("\\");
                    }
                    line += cfg->m_source_dir + wxT("\\include\\crt\\iccavr;");
                    /* Add the include path of the source tree. */
                    if (!::wxIsAbsolutePath(cfg->m_source_dir)) {
                        line += cfg->m_topdir + wxT("\\");
                    }
                    line += cfg->m_source_dir + wxT("\\include");
                    /* If we have a last include path, add it last. */
                    if(!cfg->m_lastidir.IsEmpty()) {
                        line += wxT(";") + cfg->m_lastidir;
                    }
                    line = wxT("Edit1=") + line;
                    line.Replace(wxT("/"), wxT("\\"));
                }
                else if (line.StartsWith(wxT("Edit2="))) {
                    line = wxT("Edit2=");
                    if (!::wxIsAbsolutePath(cfg->m_buildpath)) {
                        line += cfg->m_topdir + wxT("\\");
                    }
                    line += cfg->m_buildpath + wxT("\\lib");
                    line.Replace(wxT("/"), wxT("\\"));
                }
                else if (line.StartsWith(wxT("Edit3="))) {
                    NUTCOMPONENTOPTION *opt = m_doc->FindOptionByName(NULL, "PLATFORM");
                    line = wxT("Edit3=");
                    if (opt && opt->nco_enabled && opt->nco_active) {
                        if (opt->nco_value && opt->nco_value[0]) {
                            line += wxString(opt->nco_value, wxConvLocal) + wxT(" ");
                        } else {
                            char *value = GetConfigValueOrDefault(m_doc->GetRepository(), opt->nco_compo, opt->nco_name);
                            if (value) {
                                line += wxString(value, wxConvLocal) + wxT(" ");
                                free(value);
                            }
                        }
                    }
                    opt = m_doc->FindOptionByName(NULL, "MCU_ATMEGA2561");
                    if (opt == NULL) {
                        opt = m_doc->FindOptionByName(NULL, "MCU_ATMEGA2560");
                    }
                    if (opt && opt->nco_enabled && opt->nco_active) {
                        line += wxT("_MCU_extended");
                    }
                    else {
                        line += wxT("_MCU_enhanced");
                    }
                    line += wxT(" __HARVARD_ARCH__");
                    line += wxT(" ATMEGA");
                    line += wxT(" CONST=\"\"");
                }
                else if (line.StartsWith(wxT("Edit13="))) {
                    char *value = NULL;
                    NUTCOMPONENTOPTION *opt = m_doc->FindOptionByName(NULL, "ICCAVR_STARTUP");
                    line = wxT("Edit13=");

                    if (opt && opt->nco_enabled && opt->nco_active) {
                        if (opt->nco_value) {
                            value = strdup(opt->nco_value);
                        } else {
                            value = GetConfigValueOrDefault(m_doc->GetRepository(), opt->nco_compo, opt->nco_name);
                        }
                    }
                    wxString valstr;
                    if (value) {
                        valstr = wxString(value, wxConvLocal).Trim().Trim(false);
                        free(value);
                    }
                    if (valstr.IsEmpty()) {
                        line += wxT("crtenutram");
                    } else {
                        line += valstr;
                    }
                    line += wxT(".o");
                }
                else if (line.StartsWith(wxT("Edit27="))) {
                    line = wxT("Edit27=");
                    if (!::wxIsAbsolutePath(cfg->m_buildpath)) {
                        line += cfg->m_topdir + wxT("\\");
                    }
                    line += cfg->m_buildpath + wxT("\\lib\\nutinit.o");
                    line.Replace(wxT("/"), wxT("\\"));
                }
            }
            else if (line[0] == '[') {
                in_section = line.IsSameAs(wxT("[Compiler Options]"), false);
            }
            ostream.WriteString(line + wxT("\n"));
        }
        return true;
    }

    bool TransferSourcesFile(wxString source, wxString target)
    {
        wxFileInputStream f_source(source);
        if (!f_source.Ok()) {
            wxLogMessage(wxT("Failed to read from %s"), source.c_str());
            return false;
        }
        wxFileOutputStream f_target(target);
        if (!f_target.Ok()) {
            wxLogMessage(wxT("Failed to write to %s"), target.c_str());
            return false;
        }

        wxTextInputStream istream(f_source);
        wxTextOutputStream ostream(f_target);
        for(;;) {
            wxString line = istream.ReadLine();
            wxString rest;

            if (f_source.Eof() && line.IsEmpty()) {
                break;
            }
            if (line.StartsWith(wxT("..\\..\\app\\"), &rest)) {
                line = rest.AfterFirst('\\');
            }
            ostream.WriteString(line + wxT("\n"));
        }
        return true;
    }

};
#endif

class CDirEclipseProjectTraverser : public CAbstractDirCopyTraverser
{
public:
    CDirEclipseProjectTraverser(wxString source, wxString target)
    : m_source(source)
    , m_target(target)
    {
        m_doc = wxGetApp().GetNutConfDoc();
    }

    virtual wxDirTraverseResult OnFile(const wxString& filename)
    {
        wxString sub = filename.Mid(m_source.Length());
        wxFileName name(m_target + sub);
        /* Create project source list, but do not override exiting list. */
        if(name.GetFullName().IsSameAs(wxT(".cproject"), false) && !name.FileExists()) {
            TransferSourcesFile(filename, name.GetFullPath());
        }
        if(name.GetFullName().IsSameAs(wxT(".project"), false) && !name.FileExists()) {
            TransferProjectFile(filename, name.GetFullPath());
        }
        return wxDIR_CONTINUE;
    }

    virtual wxDirTraverseResult OnDir(const wxString& dirname)
    {
        wxString sub = dirname.Mid(m_source.Length());
        wxFileName name(m_target + sub + wxFileName::GetPathSeparator(), wxEmptyString);
		if(!isSCMdir(name.GetFullPath())) {
            name.Mkdir(0777, wxPATH_MKDIR_FULL);
            return wxDIR_CONTINUE;
        }
        return wxDIR_IGNORE;
    }
private:
    wxString m_source;
    wxString m_target;
    CNutConfDoc *m_doc;

    bool TransferProjectFile(wxString source, wxString target)
    {
        wxFileInputStream f_source(source);
        if (!f_source.Ok()) {
            wxLogMessage(wxT("Failed to read from %s"), source.c_str());
            return false;
        }
        wxFileOutputStream f_target(target);
        if (!f_target.Ok()) {
            wxLogMessage(wxT("Failed to write to %s"), target.c_str());
            return false;
        }

        wxTextInputStream istream(f_source);
        wxTextOutputStream ostream(f_target);
        for(;;) {
            wxString line = istream.ReadLine();

            if (f_source.Eof() && line.IsEmpty()) {
                break;
            }
            ostream.WriteString(line + wxT("\n"));
        }
        return true;
    }

    bool TransferSourcesFile(wxString source, wxString target)
    {
        wxFileInputStream f_source(source);
        if (!f_source.Ok()) {
            wxLogMessage(wxT("Failed to read from %s"), source.c_str());
            return false;
        }
        wxFileOutputStream f_target(target);
        if (!f_target.Ok()) {
            wxLogMessage(wxT("Failed to write to %s"), target.c_str());
            return false;
        }

        wxTextInputStream istream(f_source);
        wxTextOutputStream ostream(f_target);
        for(;;) {
            wxString line = istream.ReadLine();
            wxString rest;

            if (f_source.Eof() && line.IsEmpty()) {
                break;
            }
            ostream.WriteString(line + wxT("\n"));
        }
        return true;
    }

};


bool CNutConfDoc::GenerateApplicationTree()
{
    CSettings *cfg = wxGetApp().GetSettings();

    wxBusyCursor wait;

    wxString src_dir = cfg->m_source_dir + wxT("/app");
    wxString cfg_inc = cfg->m_firstidir;

    wxLogMessage(wxT("Copying samples from %s to %s"), src_dir.c_str(), cfg->m_app_dir.c_str());
    CDirCopyTraverser traverser(src_dir, cfg->m_app_dir);
    wxDir dir(src_dir);
    dir.Traverse(traverser);

#ifdef _WIN32
    src_dir = cfg->m_source_dir + wxT("/appicc");
    wxLogMessage(wxT("Translating ICCAVR projects from %s to %s"), src_dir.c_str(), cfg->m_app_dir.c_str());
    CDirIccAvrProjectTraverser icc_traverser(src_dir, cfg->m_app_dir);
    wxDir icc_dir(src_dir);
    icc_dir.Traverse(icc_traverser);
#endif
#if 0
    src_dir = cfg->m_source_dir + wxT("/appeclipse");
    wxLogMessage(wxT("Translating Eclipse projects from %s to %s"), src_dir.c_str(), cfg->m_app_dir.c_str());
    CDirEclipseProjectTraverser eclipse_traverser(src_dir, cfg->m_app_dir);
    wxDir eclipse_dir(src_dir);
    eclipse_dir.Traverse(eclipse_traverser);
#endif
    wxLogMessage(wxT("Creating Makefiles for %s in %s"), cfg->m_platform.c_str(), cfg->m_app_dir.c_str());
    if(CreateSampleDirectory(m_repository, m_root, cfg->m_buildpath.mb_str(), cfg->m_app_dir.mb_str(), 
                             cfg->m_source_dir.mb_str(), GetInstallDir().mb_str(), cfg->m_platform.mb_str(), 
                             cfg->m_programmer.mb_str(), cfg->m_firstidir.mb_str(), cfg->m_lastidir.mb_str())) {
        return false;
    }
    wxLogMessage(wxT("OK"));
    return true;
}
