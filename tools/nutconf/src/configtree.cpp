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
 * $Log: configtree.cpp,v $
 * Revision 1.7  2008/07/24 15:46:43  haraldkipp
 * Simplyfied mouse event handling.
 * Report dynamic script errors.
 *
 * Revision 1.6  2008/03/17 10:17:18  haraldkipp
 * Removed dispensable scroll handlers.
 *
 * Revision 1.5  2006/10/05 17:04:45  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.4  2005/04/25 08:35:31  haraldkipp
 * Upgrade to wxWidgets 6.0
 *
 * Revision 1.3  2005/04/22 15:09:00  haraldkipp
 * Avoid compiler warnings.
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "nutconf.h"
#include "utils.h"
#include "treeitemdata.h"
#include "nutconfhint.h"
#include "configitem.h"
#include "configtree.h"

#if !defined(__WXMSW__)
#include "bitmaps/checked.xpm"
#include "bitmaps/checked_dis.xpm"
#include "bitmaps/closedfolder.xpm"
#include "bitmaps/closedfolder_dis.xpm"
#include "bitmaps/enumerated.xpm"
#include "bitmaps/enumerated_dis.xpm"
#include "bitmaps/integer.xpm"
#include "bitmaps/integer_dis.xpm"
#include "bitmaps/radiooff.xpm"
#include "bitmaps/radiooff_dis.xpm"
#include "bitmaps/radioon.xpm"
#include "bitmaps/radioon_dis.xpm"
#include "bitmaps/text.xpm"
#include "bitmaps/text_dis.xpm"
#include "bitmaps/unchecked.xpm"
#include "bitmaps/unchecked_dis.xpm"
#include "bitmaps/library.xpm"
#include "bitmaps/library_dis.xpm"
#include "bitmaps/module.xpm"
#include "bitmaps/module_dis.xpm"
#endif

IMPLEMENT_CLASS(CConfigTree, CScrolledTreeCtrl)

BEGIN_EVENT_TABLE(CConfigTree, CScrolledTreeCtrl)
    EVT_MOUSE_EVENTS(CConfigTree::OnMouseEvent)
    EVT_TREE_SEL_CHANGED(-1, CConfigTree::OnSelChanged)
END_EVENT_TABLE();

CConfigTree::CConfigTree(wxWindow * parent, wxWindowID id, const wxPoint & pt, const wxSize & sz, long style)
:  CScrolledTreeCtrl(parent, id, pt, sz, style)
{
    m_imageList = new wxImageList(16, 16, true);
    m_iconDB.SetImageList(m_imageList);
    SetImageList(m_imageList);

    m_iconDB.AddInfo(wxT("Folder"), wxICON(closedfolder), 0, true);
    m_iconDB.AddInfo(wxT("Folder"), wxICON(closedfolder_dis), 0, false);

    m_iconDB.AddInfo(wxT("Library"), wxICON(library), 0, true);
    m_iconDB.AddInfo(wxT("Library"), wxICON(library_dis), 0, false);

    m_iconDB.AddInfo(wxT("Module"), wxICON(module), 0, true);
    m_iconDB.AddInfo(wxT("Module"), wxICON(module_dis), 0, false);

    m_iconDB.AddInfo(wxT("Checkbox"), wxICON(checked), 0, true);
    m_iconDB.AddInfo(wxT("Checkbox"), wxICON(checked_dis), 0, false);
    m_iconDB.AddInfo(wxT("Checkbox"), wxICON(unchecked), 1, true);
    m_iconDB.AddInfo(wxT("Checkbox"), wxICON(unchecked_dis), 1, false);

    m_iconDB.AddInfo(wxT("Radiobox"), wxICON(radioon), 0, true);
    m_iconDB.AddInfo(wxT("Radiobox"), wxICON(radioon_dis), 0, false);
    m_iconDB.AddInfo(wxT("Radiobox"), wxICON(radiooff), 1, true);
    m_iconDB.AddInfo(wxT("Radiobox"), wxICON(radiooff_dis), 1, false);

    m_iconDB.AddInfo(wxT("Text"), wxICON(text), 0, true);
    m_iconDB.AddInfo(wxT("Text"), wxICON(text_dis), 0, false);

    m_iconDB.AddInfo(wxT("Enumerated"), wxICON(enumerated), 0, true);
    m_iconDB.AddInfo(wxT("Enumerated"), wxICON(enumerated_dis), 0, false);

    m_iconDB.AddInfo(wxT("Integer"), wxICON(integer), 0, true);
    m_iconDB.AddInfo(wxT("Integer"), wxICON(integer_dis), 0, false);
}

void CConfigTree::OnMouseEvent(wxMouseEvent & event)
{
    int flags = 0;
    wxTreeItemId item;

    if (event.LeftDown()) {
        item = HitTest(wxPoint(event.GetX(), event.GetY()), flags);
        if (item.IsOk()) {
            if (flags & wxTREE_HITTEST_ONITEMICON) {
                CTreeItemData *data = (CTreeItemData *) GetItemData(item);
                data->GetConfigItem()->OnIconLeftDown(*this);
            }
        }
        event.Skip();
    } else if (event.RightDown()) {
        item = HitTest(wxPoint(event.GetX(), event.GetY()), flags);
        if (item.IsOk()) {
            if ((flags & wxTREE_HITTEST_ONITEMBUTTON) ||
                (flags & wxTREE_HITTEST_ONITEMICON) || 
                (flags & wxTREE_HITTEST_ONITEMINDENT) || 
                (flags & wxTREE_HITTEST_ONITEMLABEL)) {
                SelectItem(item);
            }
        }
    }
    else {
        event.Skip();
    }
}

void CConfigTree::OnSelChanged(wxTreeEvent & WXUNUSED(event))
{
    CNutConfDoc *doc = wxGetApp().GetNutConfDoc();
    if (doc) {
        CNutConfHint hint(NULL, nutSelChanged);
        doc->UpdateAllViews(NULL, &hint);
        if (GetScriptStatus()) {
            wxLogMessage(wxT("%s"), GetScriptErrorString());
        }
    }
}

CIconList & CConfigTree::GetIconDB()
{
    return m_iconDB;
}

wxTreeItemId CConfigTree::FindNextItemId(wxTreeItemId treeItemId, const wxString& text,
    bool checkFirst, bool matchCase, bool matchWord)
{
    wxString ctext(text);
    if (!matchCase) {
        ctext.MakeLower();
    }

    wxTreeItemId found;
    wxTreeItemId id;
    wxTreeItemId currentId = treeItemId;
    CConfigItem *item = NULL;
    wxTreeItemIdValue dummy;

    while (currentId.IsOk()) {
        if (checkFirst) {
            CTreeItemData *data = (CTreeItemData *)GetItemData(currentId);
            if (data) {
                if ((item = data->GetConfigItem()) != NULL) {
                    wxString name = item->GetName();
                    wxString brief = item->GetBriefDescription();
                    wxString desc = item->GetDescription();

                    if (!matchCase) {
                        name.MakeLower();
                        brief.MakeLower();
                        desc.MakeLower();
                    }
                    if (CUtils::FindString(name, ctext, matchWord) ||
                        CUtils::FindString(brief, ctext, matchWord) ||
                        CUtils::FindString(desc, ctext, matchWord)) {
                        found = currentId;
                        break;
                    }
                }
            }
        }
        checkFirst = true;

        id = GetFirstChild(currentId, dummy);
        if (!id.IsOk()) {
            id = GetNextSibling(currentId);
            if (!id.IsOk()) {
                wxTreeItemId parentId = currentId;
                do {
                    parentId = GetItemParent(parentId);
                    if (!parentId.IsOk()) {
                        break;
                    }
                    id = GetNextSibling(parentId);
                } while (!id.IsOk());
            }
        }
        currentId = id;
    }
    return found;
}
