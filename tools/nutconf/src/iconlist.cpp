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
 * $Log: iconlist.cpp,v $
 * Revision 1.3  2005/09/21 10:44:15  christianwelzel
 * Replaced deprecated commands with newer ones.
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "iconlist.h"


/*
 * List of icon states.
 */
CIconList::CIconList(wxImageList * imageList)
{
    m_imageList = imageList;
    DeleteContents(true);
}

bool CIconList::AddInfo(const wxString & name, const wxIcon & icon, int state, bool enabled)
{
    wxASSERT(m_imageList != NULL);

    CIconInfo *info = FindInfo(name);
    if (!info) {
        info = new CIconInfo(name);
        Append(info);
    }
    info->SetIconId(state, enabled, m_imageList->Add(icon));
    return true;
}

CIconInfo *CIconList::FindInfo(const wxString & name) const
{
    wxNode *node = GetFirst();
    while (node) {
        CIconInfo *info = (CIconInfo *) node->GetData();
        if (info->GetName() == name)
            return info;
        node = node->GetNext();
    }
    return NULL;
}

int CIconList::GetIconId(const wxString & name, int state, bool enabled) const
{
    CIconInfo *info = FindInfo(name);
    if (!info)
        return -1;
    return info->GetIconId(state, enabled);
}

bool CIconList::SetIconId(const wxString & name, int state, bool enabled, int iconId)
{
    CIconInfo *info = FindInfo(name);
    if (!info)
        return false;
    info->SetIconId(state, enabled, iconId);
    return true;
}

void CIconList::SetImageList(wxImageList * imageList)
{
    m_imageList = imageList;
}

wxImageList *CIconList::GetImageList() const
{
    return m_imageList;
}
