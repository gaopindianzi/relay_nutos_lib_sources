#ifndef CONFIGITEM_H_
#define CONFIGITEM_H_

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
 * $Log$
 * Revision 1.5  2008/03/17 10:22:48  haraldkipp
 * Added more comments.
 *
 * Revision 1.4  2006/10/05 17:04:45  haraldkipp
 * Heavily revised and updated version 1.3
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

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/variant.h>

#include "nutcomponent.h"

/*!
 * \brief Component types.
 *
 * The Nut/OS configuration is organized in a tree structure.
 */
enum nutComponentType {
    /*! \brief No specific type or type not yet assigned. */
    nutConfigTypeNone,

    /*! \brief Folder component. 
     *
     * A folder may contain any number of libraries. 
     * Seems to be currently unused.
     */
    nutFolder,

    /*! \brief Library component.
     *
     * A library may contain any number of modules or options.
     */
    nutLibrary,

    /*! \brief Module component. 
     * 
     * A module may contain any number of options. Modules are
     * typically related to source code files.
     */
    nutModule,

    /*! \brief Component option. 
     *
     * Each component option specifies a configuration value.
     */
    nutOption
};

/*!
 * \brief Option flavors.
 */
enum nutOptionFlavor {
    /*! \brief No specific flavor. 
     *
     */
    nutFlavorNone,

    /*! \brief Boolean flavor.
     *
     * Option may be explicitely enabled or disabled, either by a
     * check box or a radio button control.
     */
    nutFlavorBool,

    /*! \brief Boolean data flavor.
     *
     * Option may be explicitely enabled or disabled, either by a
     * check box or a radio button control. In addition this option
     * contains a data value, which is empty if the option is
     * disabled.
     */
    nutFlavorBoolData,

    /*! \brief Data flavor.
     *
     * This option contains a data value.
     */
    nutFlavorData
};

/*!
 * \brief Option types.
 */
enum nutOptionType {
    /*! \brief No specific option. 
     *
     */
    nutOptionTypeNone,

    /*! \brief Integer value. 
     *
     * Option is used to configure an integer value.
     */
    nutInteger,

    /*! \brief Boolean value. 
     *
     * Option is used to configure a boolean value.
     */
    nutBool,

    /*! \brief String value. 
     *
     * Option is used to configure a textual value.
     */
    nutString,

    /*! \brief Enumerated value. 
     *
     * Used to select a configuration option from a predefined set.
     */
    nutEnumerated
};

enum nutUIHint {
    nutHintNone,
    nutHintCheck,
    nutHintRadio
};

class CTreeItemData;
class CConfigTree;

/*!
 * \brief Configuration tree component.
 */
class CConfigItem:public wxObject {
    DECLARE_CLASS(CConfigItem)
    friend class CNutConfDoc;
  public:
     CConfigItem();
     CConfigItem(CConfigItem * parent, NUTCOMPONENTOPTION * option);
     CConfigItem(CConfigItem * parent, NUTCOMPONENT * compo);

    wxTreeItemId GetTreeItem() const;
    void SetTreeItem(wxTreeItemId id);

    nutComponentType GetConfigType() const;

    void SetUIHint(nutUIHint hint);
    nutUIHint GetUIHint() const;

    bool IsEnabled() const;

    void SetValue(const wxVariant & value);
     wxVariant & GetValue();

    wxString GetName() const;
    wxString GetBriefDescription() const;
    wxString GetDescription() const;
    nutOptionFlavor GetOptionFlavor() const;
    nutOptionType GetOptionType() const;
    int GetEnumStrings(wxArrayString & arEnumStrings) const;
    wxString GetRequirementList() const;
    wxString GetProvisionList() const;
    wxString GetExclusivityList() const;

    void SetActive(bool ena);
    bool IsActive() const;

    bool UpdateTreeItem(CConfigTree & treeCtrl);

    bool CanEdit() const;
    wxWindow *CreateEditWindow(wxWindow * parent);

    void OnIconLeftDown(CConfigTree & treeCtrl);

    wxString GetDisplayValue() const;
    wxString StringValue() const;
    wxString GetFilename() const;
    wxString GetMacro() const;

    bool TransferDataToWindow(wxWindow * window);
    bool TransferDataFromWindow(wxWindow * window);

    bool HasBool() const;

    bool ViewHeaderFile();


  protected:
    /*! 
     * \brief Parent component.
     */
    CConfigItem * m_parent;

    /*! 
     * \brief Unique component ID.
     */
    wxTreeItemId m_itemId;

    /*! 
     * \brief Pointer to the component data structure.
     */
    NUTCOMPONENT *m_compo;

    /*! 
     * \brief Pointer to the option data structure.
     */
    NUTCOMPONENTOPTION *m_option;

    /*! 
     * \brief Component name.
     */
    wxString m_name;

    /*! 
     * \brief Component type of this item.
     *
     * Either library, module or option.
     */
    nutComponentType m_configType;

    /*!
     * \brief Option type of this item.
     *
     * If this item is an option, then it is either a string,
     * an integer, a boolean or an enumerated value.
     */
    nutOptionType m_optionType;

    nutUIHint m_hint;

    /*!
     * \brief Value of this item.
     */
    wxVariant m_value;
};

#endif
