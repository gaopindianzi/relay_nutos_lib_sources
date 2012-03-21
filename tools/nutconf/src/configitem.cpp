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
 * ----------------------------------------------------------------------------
 */

/*
 * $Log: configitem.cpp,v $
 * Revision 1.11  2008/08/01 19:41:18  olereinhardt
 * Added cast to char* in line 698 to avoid compiling bug on linux
 *
 * Revision 1.10  2008/07/29 07:29:36  haraldkipp
 * Honor editing of boolean settings.
 *
 * Revision 1.9  2008/07/24 15:41:41  haraldkipp
 * Dynamic configuration.
 *
 * Revision 1.8  2008/03/17 10:22:48  haraldkipp
 * Added more comments.
 *
 * Revision 1.7  2006/10/05 17:04:45  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.6  2005/11/24 09:44:30  haraldkipp
 * wxWidget failed to built with unicode support, which results in a number
 * of compile errors. Fixed by Torben Mikael Hansen.
 *
 * Revision 1.5  2005/04/22 15:08:10  haraldkipp
 * Avoid compiler warnings.
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.4  2004/09/07 19:16:12  haraldkipp
 * Activate boolean items only
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:24  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */


#include "ids.h"
#include "utils.h"
#include "nutconf.h"
#include "configtree.h"
#include "configitem.h"
#include "enumeditctrl.h"
#include "inteditctrl.h"
#include "texteditctrl.h"

#ifdef __WXMSW__
#define strcasecmp stricmp
#endif

IMPLEMENT_CLASS(CConfigItem, wxObject);

/*!
 * \brief Default constructor.
 */
CConfigItem::CConfigItem()
{
    m_compo = NULL;
    m_option = NULL;
    m_name = wxEmptyString;
    m_configType = nutConfigTypeNone;
    m_optionType = nutOptionTypeNone;
    m_parent = NULL;
    m_hint = nutHintNone;
    m_itemId = wxTreeItemId();
}

/*!
 * \brief Creates a CConfigItem from a given NUTCOMPONENT.
 */
CConfigItem::CConfigItem(CConfigItem * parent, NUTCOMPONENT * compo)
{
    m_compo = compo;
    m_option = NULL;

    /* Convert the name from the one in the C structure. */
    m_name = wxString(compo->nc_name, wxConvLocal);

    /* Libraries contain modules. */
    if (compo->nc_child) {
        m_configType = nutLibrary;
    } else {
        m_configType = nutModule;
    }
    m_optionType = nutString;
    m_parent = parent;
    m_hint = nutHintNone;
    m_itemId = wxTreeItemId();
    m_value = wxEmptyString;
}

/*!
 * \brief Creates a CConfigItem from a given NUTCOMPONENTOPTION.
 */
CConfigItem::CConfigItem(CConfigItem * parent, NUTCOMPONENTOPTION * option)
{
    m_compo = NULL;
    m_option = option;

    /* Convert the name from the one in the C structure. */
    m_name = wxString(option->nco_name, wxConvLocal);
    m_configType = nutOption;
    m_optionType = nutString;
    m_parent = parent;

    if (m_option && m_option->nco_exclusivity) {
        m_hint = nutHintRadio;
    }
    else {
        m_hint = nutHintNone;
    }

    m_itemId = wxTreeItemId();

    switch (m_optionType) {
    case nutString:
    case nutEnumerated:
        m_value = wxEmptyString;
        break;
    case nutInteger:
        m_value = (long) 0;
        break;
    case nutBool:
        m_value = false;
        break;
    }
}

/*!
 * \brief Returns ID of this item.
 */
wxTreeItemId CConfigItem::GetTreeItem() const
{
    return m_itemId;
}

/*!
 * \brief Sets ID of this item.
 */
void CConfigItem::SetTreeItem(wxTreeItemId id)
{
    m_itemId = id;
}

/*!
 * \brief Returns type of this item.
 *
 * \return Either \ref nutConfigTypeNone, \ref nutFolder, \ref nutLibrary, 
 *         \ref nutModule or \ref nutOption.
 */
nutComponentType CConfigItem::GetConfigType() const
{
    return m_configType;
}

void CConfigItem::SetUIHint(nutUIHint hint)
{
    m_hint = hint;
}

nutUIHint CConfigItem::GetUIHint() const
{
    return m_hint;
}

void CConfigItem::SetValue(const wxVariant & value)
{
    m_value = value;
}

wxVariant & CConfigItem::GetValue()
{
    return m_value;
}

/*!
 * \brief Refresh a CConfigItem entry in a specified CConfigTree.
 */
bool CConfigItem::UpdateTreeItem(CConfigTree & treeCtrl)
{
    treeCtrl.SetItemText(m_itemId, GetBriefDescription());

    static wxColour normalColour = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    static wxColour disabledColour = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);

    treeCtrl.SetItemTextColour(m_itemId, normalColour);

    int iconState = 0;
    wxString iconName;

    switch (GetConfigType()) {
    case nutFolder:
        iconName = wxT("Folder");
        break;
    case nutLibrary:
        iconName = wxT("Library");
        break;
    case nutModule:
        iconName = wxT("Module");
        break;
    case nutOption:
        if (GetOptionFlavor() == nutFlavorData) {
            switch (GetOptionType()) {
            case nutInteger:
                iconName = wxT("Integer");
                break;
            case nutEnumerated:
                iconName = wxT("Enumerated");
                break;
            case nutString:
                iconName = wxT("Text");
                break;
            case nutBool:
                if (GetUIHint() == nutHintRadio)
                    iconName = wxT("Radiobox");
                else
                    iconName = wxT("Checkbox");
                iconState = (m_value.GetBool()? 0 : 1);
                break;
            }
        }
        if (GetOptionFlavor() == nutFlavorBoolData || GetOptionFlavor() == nutFlavorBool) {
            if (GetUIHint() == nutHintRadio)
                iconName = wxT("Radiobox");
            else
                iconName = wxT("Checkbox");
            iconState = (IsActive()? 0 : 1);
        }
        break;
    }

    if (!iconName.IsEmpty()) {
        int iconId = treeCtrl.GetIconDB().GetIconId(iconName, iconState, IsEnabled());
        treeCtrl.SetItemImage(m_itemId, iconId, wxTreeItemIcon_Normal);
        treeCtrl.SetItemImage(m_itemId, iconId, wxTreeItemIcon_Selected);
    }
    return true;
}

/*!
 * \brief Create a control to edit our value.
 *
 * This routine is called when the user clicks on an editable
 * item in the value window.
 */
wxWindow *CConfigItem::CreateEditWindow(wxWindow * parent)
{
    wxWindow *window = NULL;

    switch (GetOptionType()) {
    case nutEnumerated:
        /* Create drop down box for enumerated values. */
        {
            window = new CEnumEditCtrl(parent, ID_ITEM_EDIT_WINDOW, wxDefaultPosition, wxDefaultSize, 0);
            wxArrayString arEnumStrings;
            GetEnumStrings(arEnumStrings);
            for (unsigned int i = 0; i < arEnumStrings.GetCount(); i++) {
                ((CEnumEditCtrl *) window)->Append(arEnumStrings[i]);
            }
        }
        break;
    case nutInteger:
        /* Use a spin control for integer values. */
        window = new CIntEditCtrl(parent, ID_ITEM_EDIT_WINDOW, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS);
        break;
    case nutString:
        /* Normal entry control for string values. */
        window = new CTextEditCtrl(parent, ID_ITEM_EDIT_WINDOW, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        break;
    }

    wxASSERT(window != NULL);

    return window;
}

/*!
 * \brief Process a left mouse button click on our icon.
 *
 * Boolean values will be inverted.
 *
 * \param treeCtrl The tree control that received the click.
 */
void CConfigItem::OnIconLeftDown(CConfigTree & WXUNUSED(treeCtrl))
{
    if (GetConfigType() != nutOption)
        return;

    switch (GetOptionFlavor()) {
    case nutFlavorBool:
    case nutFlavorBoolData:
        if (IsEnabled()) {
            CNutConfDoc *pDoc = wxGetApp().GetNutConfDoc();

            if (m_option && m_option->nco_exclusivity) {
                if(!IsActive()) {
                    pDoc->DeactivateOptionList(m_option->nco_compo, m_option->nco_exclusivity);
                    pDoc->SetActive(*this, true);
                }
            }
            else {
                pDoc->SetActive(*this, !IsActive());
            }
        }
    default:
        break;
    }
}

/*!
 * \brief Launch external file viewer.
 */
bool CConfigItem::ViewHeaderFile()
{
    bool rc = false;
    wxString strFile(GetFilename());

    if (!strFile.IsEmpty()) {
        CNutConfDoc *pDoc = wxGetApp().GetNutConfDoc();
        if (pDoc->GetBuildTree().IsEmpty()) {
            wxMessageBox(wxT("Cannot display header file until configuration is saved"));
        } else {
            strFile.Prepend(wxT("/"));
            strFile.Prepend(pDoc->GetBuildTree());
            //rc = wxGetApp().Launch(strFile, "notepad.exe");
            rc = wxGetApp().Launch(strFile, wxEmptyString);
        }
    }
    return rc;
}

/*! 
 * \brief Return the item's brief description.
 */
wxString CConfigItem::GetBriefDescription() const
{
    wxString str;
    char *brief;

    if (m_compo) {
        brief = GetComponentBrief(wxGetApp().GetNutConfDoc()->GetRepository(), m_compo);
        if (brief) {
            str = wxString(brief, wxConvLocal);
            free(brief);
        } else {
            str = wxString(m_compo->nc_name, wxConvLocal);
        }
    } else if (m_option) {
        brief = GetOptionBrief(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (brief) {
            str = wxString(brief, wxConvLocal);
            free(brief);
        } else {
            str = wxString(m_option->nco_name, wxConvLocal);
        }
    } else {
        str = wxT("Nut/OS Components");
    }
    return str;
}

wxString CConfigItem::GetName() const
{
    wxString str;

    if (m_compo) {
        str = wxString(m_compo->nc_name,wxConvLocal);
    } else if (m_option) {
        str = wxString(m_option->nco_name,wxConvLocal);
    }
    return str;
}

wxString CConfigItem::GetDescription() const
{
    wxString str;
    char *desc = NULL;

    if (m_compo) {
        desc = GetComponentDescription(wxGetApp().GetNutConfDoc()->GetRepository(), m_compo);
    } else if (m_option) {
        desc = GetOptionDescription(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
    }
    if (desc) {
        str = wxString(desc, wxConvLocal);
        free(desc);
    }
    return str;
}

nutOptionFlavor CConfigItem::GetOptionFlavor() const
{
    if (m_option) {
        char *flavor = GetOptionFlavour(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (flavor) {
            if (strcasecmp(flavor, "boolean") == 0) {
                free(flavor);
                return nutFlavorBool;
            }
            if (strcasecmp(flavor, "booldata") == 0) {
                free(flavor);
                return nutFlavorBoolData;
            }
            free(flavor);
        }
        return nutFlavorData;
    }
    return nutFlavorNone;
}

nutOptionType CConfigItem::GetOptionType() const
{
    nutOptionType rc = nutOptionTypeNone;

    if (m_option) {
        rc = nutString;
        char *type = GetOptionTypeString(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (type) {
            if (strcasecmp(type, "integer") == 0) {
                rc = nutInteger;
            }
            else if (strcasecmp(type, "bool") == 0) {
                rc = nutBool;
            }
            else if (strcasecmp(type, "enumerated") == 0) {
                rc = nutEnumerated;
            }
            free(type);
        }
    }
    return rc;
}

wxString CConfigItem::GetDisplayValue() const
{
    wxString str;

    switch (GetOptionType()) {
    case nutEnumerated:
    case nutInteger:
    case nutString:
        str = StringValue();
        break;
    }
    return str;
}

/*!
 * \brief Return configuration item as a string.
 */
wxString CConfigItem::StringValue() const
{
    wxString str;

    if (m_option) {
        /* First try the edited value. */
        if (m_option->nco_value) {
            str = wxString(m_option->nco_value, wxConvLocal);
        } else {
            /* Get either the configured value or its default. */
            char *val = GetConfigValueOrDefault(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
            if (val) {
                str = wxString(val, wxConvLocal);
                free(val);
            }
            /* Return the first enumerated item, if no configured or default value. */
            else if (GetOptionType() == nutEnumerated) {
                char **choices = GetOptionChoices(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
                if (choices) {
                    if (choices[0]) {
                        str = wxString(choices[0], wxConvLocal);
                    }
                    for (int i = 0; choices[i]; i++) {
                        free(choices[i]);
                    }
                    free(choices);
                }
            }
        }
    }
    return str;
}

wxString CConfigItem::GetFilename() const
{
    wxString str;

    if (m_option) {
        char *fname = GetOptionFile(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (fname) {
            str = wxString(fname, wxConvLocal);
            free(fname);
        }
    }
    return str;
}

wxString CConfigItem::GetMacro() const
{
    if (m_option && m_option->nco_name) {
        return wxString(m_option->nco_name,wxConvLocal);
    }
    return wxEmptyString;
}

wxString CConfigItem::GetRequirementList() const
{
    wxString str;

    if (m_option) {
        char **requires = GetOptionRequirements(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (requires) {
            for (int i = 0; requires[i]; i++) {
                if(!str.IsEmpty()) {
                    str += wxT(", ");
                }
                str += wxString(requires[i], wxConvLocal);
                free(requires[i]);
            }
            free(requires);
        }
    }
    /* Do we need component requirements? */
    if (m_compo) {
        char **requires = GetComponentRequirements(wxGetApp().GetNutConfDoc()->GetRepository(), m_compo);
        if (requires) {
            for (int i = 0; requires[i]; i++) {
                if(!str.IsEmpty()) {
                    str += wxT(", ");
                }
                str += wxString(requires[i],wxConvLocal);
                free(requires[i]);
            }
            free(requires);
        }
    }
    return str;
}

wxString CConfigItem::GetProvisionList() const
{
    wxString str;
    char **provides;

    if (m_option) {
        provides = GetOptionProvisions(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (provides) {
            for (int i = 0; provides[i]; i++) {
                if(!str.IsEmpty()) {
                    str += wxT(", ");
                }
                str += wxString(provides[i],wxConvLocal);
                free(provides[i]);
            }
            free(provides);
        }
    }
    if (m_compo) {
        provides = GetComponentProvisions(wxGetApp().GetNutConfDoc()->GetRepository(), m_compo);
        if (provides) {
            for (int i = 0; provides[i]; i++) {
                if(!str.IsEmpty()) {
                    str += wxT(", ");
                }
                str += wxString(provides[i],wxConvLocal);
                free(provides[i]);
            }
            free(provides);
        }
    }
    return str;
}

wxString CConfigItem::GetExclusivityList() const
{
    wxString str;

    if (m_option && m_option->nco_exclusivity) {
        for (int i = 0; m_option->nco_exclusivity[i]; i++) {
            if(!str.IsEmpty()) {
                str += wxT(", ");
            }
            str += wxString(m_option->nco_exclusivity[i], wxConvLocal);
        }
    }
    if (m_compo && m_compo->nc_exclusivity) {
        for (int i = 0; m_compo->nc_exclusivity[i]; i++) {
            if(!str.IsEmpty()) {
                str += wxT(", ");
            }
            str += wxString(m_compo->nc_exclusivity[i], wxConvLocal);
        }
    }
    return str;
}

int CConfigItem::GetEnumStrings(wxArrayString & arEnumStrings) const
{
    if (m_option) {
        char **choices = GetOptionChoices(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (choices) {
            for (int i = 0; choices[i]; i++) {
                arEnumStrings.Add(wxString(choices[i], wxConvLocal));
                free(choices[i]);
            }
            free(choices);
        }
    }
    return arEnumStrings.GetCount();
}


/*!
 * \brief Check if this is a boolean item.
 */
bool CConfigItem::HasBool() const
{
    if (m_option) {
        char *flavor = GetOptionFlavour(wxGetApp().GetNutConfDoc()->GetRepository(), m_option->nco_compo, m_option->nco_name);
        if (flavor && (strcasecmp(flavor, "boolean") == 0 || strcasecmp(flavor, "booldata") == 0)) {
            free(flavor);
            return true;
        }
        if (flavor) {
            free(flavor);
        }
    }
    return false;
}

/*!
 * \brief Check if this item is enabled.
 *
 * An item is disabled, if it requires other items being activated.
 */
bool CConfigItem::IsEnabled() const
{
    if (m_option) {
        return m_option->nco_enabled != 0;
    }
    if (m_compo) {
        return m_compo->nc_enabled != 0;
    }
    return false;
}

/*!
 * \brief Check if this item is active.
 *
 * An option is activated by the user or automatically by active_if.
 * Only options may become incative, components are always active.
 */
bool CConfigItem::IsActive() const
{
    if (m_option) {
        if (HasBool()) {
            if(m_option->nco_active == 0) {
                if(m_option->nco_active_if) {
                    CNutConfDoc *pDoc = wxGetApp().GetNutConfDoc();
                    m_option->nco_active = pDoc->IsOptionActive(m_option->nco_active_if);
                }
            }
            return m_option->nco_active != 0;
        }
        m_option->nco_active = 1;
    }
    return true;
}

/*!
 * \brief Set boolean value of this item.
 */
void CConfigItem::SetActive(bool ena)
{
    if (m_option) {
        m_option->nco_active = ena;
        if (ena && m_option->nco_value == NULL) {
            m_option->nco_value = strdup((char*)wxT(""));
        }
    }
}

bool CConfigItem::CanEdit() const
{
    if (!IsEnabled())
        return false;

    if (GetConfigType() != nutOption)
        return false;

    if (GetOptionFlavor() != nutFlavorData && GetOptionFlavor() != nutFlavorBoolData)
        return false;

    return true;
}

/*!
 * \brief Set value of an edit control.
 *
 * Called when editing the value of an item has started.
 *
 * \param window The edit control.
 */
bool CConfigItem::TransferDataToWindow(wxWindow * window)
{
    /* Set value of a text edit control. */
    if (window->IsKindOf(CLASSINFO(CTextEditCtrl))) {
        CTextEditCtrl *win = (CTextEditCtrl *) window;
        win->SetValue(GetDisplayValue());
    } 

    /* Set value of an enumerated edit control. */
    else if (window->IsKindOf(CLASSINFO(CEnumEditCtrl))) {
        CEnumEditCtrl *win = (CEnumEditCtrl *) window;
        win->SetStringSelection(GetDisplayValue());
    } 

    /* Set value of an integer edit control. */
    else if (window->IsKindOf(CLASSINFO(CIntEditCtrl))) {
        CIntEditCtrl *win = (CIntEditCtrl *) window;
        long i;
        CUtils::StrToItemIntegerType(StringValue(), i);

        wxString val;
        val.Printf(wxT("%ld"), i);

        win->SetValue(val);
    }
    return true;
}

/*!
 * \brief Retrieve value from an edit control.
 *
 * Called when editing the value of an item has stopped.
 *
 * \param window The edit control.
 */
bool CConfigItem::TransferDataFromWindow(wxWindow * window)
{
    CNutConfDoc *doc = wxGetApp().GetNutConfDoc();
    wxASSERT(doc != NULL);

    if (!doc)
        return false;

    /* Retrieve value from text edit control. */
    if (window->IsKindOf(CLASSINFO(CTextEditCtrl))) {
        CTextEditCtrl *win = (CTextEditCtrl *) window;

        wxASSERT(GetOptionType() == nutString);

        // TODO: do checking
        doc->SetValue(*this, win->GetValue().Trim(false));
    } 

    /* Retrieve value from enumerated edit control. */
    else if (window->IsKindOf(CLASSINFO(CEnumEditCtrl))) {
        CEnumEditCtrl *win = (CEnumEditCtrl *) window;

        wxASSERT(GetOptionType() == nutEnumerated);

        // TODO: do checking
        doc->SetValue(*this, win->GetStringSelection().Trim(false));
    } 
    
    /* Retrieve value from integer edit control. */
    else if (window->IsKindOf(CLASSINFO(CIntEditCtrl))) {
        CIntEditCtrl *win = (CIntEditCtrl *) window;

        wxASSERT(GetOptionType() == nutInteger);

        // TODO: do checking
        doc->SetValue(*this, (long) win->GetValue());
    }
    return true;
}

