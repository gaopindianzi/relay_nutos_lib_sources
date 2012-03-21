/* ----------------------------------------------------------------------------
 * Copyright (C) 2004-2005 by egnite Software GmbH
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
 * $Log: valuewindow.cpp,v $
 * Revision 1.9  2008/07/24 15:48:01  haraldkipp
 * Use method to check item validity.
 *
 * Revision 1.8  2008/03/17 10:21:39  haraldkipp
 * Fix OS X scrolling.
 *
 * Revision 1.7  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.6  2005/04/26 12:49:36  haraldkipp
 * Minor wxWidgets runtime warning avoided.
 *
 * Revision 1.5  2005/04/22 15:31:09  haraldkipp
 * Avoid compiler warnings.
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.4  2004/11/24 15:36:53  haraldkipp
 * Release 1.1.1.
 * Do not store empty options.
 * Remove include files from the build tree, if they are no longer used.
 * Command line parameter 's' allows different settings.
 * Minor compiler warning fixed.
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:15:03  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "nutconf.h"
#include "treeitemdata.h"
#include "configtree.h"
#include "enumeditctrl.h"
#include "valuewindow.h"
#include "texteditctrl.h"
#include "inteditctrl.h"
#include "utils.h"

IMPLEMENT_CLASS(CValueWindow, CTreeCompWindow)

BEGIN_EVENT_TABLE(CValueWindow, CTreeCompWindow)
    EVT_PAINT(CValueWindow::OnPaint)
    EVT_MOUSE_EVENTS(CValueWindow::OnMouseEvent)
    EVT_SCROLLWIN(CValueWindow::OnScroll)
    EVT_TREE_ITEM_EXPANDED(-1, CValueWindow::OnExpand)
    EVT_TREE_ITEM_COLLAPSED(-1, CValueWindow::OnExpand)
END_EVENT_TABLE();

CValueWindow::CValueWindow(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & sz, long style)
:  CTreeCompWindow(parent, id, pos, sz, style)
{
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));

    SetFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));

    m_editWindow = NULL;
    m_configItem = NULL;
}

void CValueWindow::DrawItem(wxDC & dc, wxTreeItemId id, const wxRect & rect)
{
    if (m_treeCtrl) {
        CConfigItem *item = ((CTreeItemData *) m_treeCtrl->GetItemData(id))->GetConfigItem();
        if (!item)
            return;
        wxString text = item->GetDisplayValue();

        if (text.IsEmpty())
            return;

        static wxColour normalColour = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
        static wxColour disabledColour = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
        dc.SetTextForeground((item->IsActive() && (item->GetConfigType() != nutModule)) ? normalColour : disabledColour);

        int textW, textH;
        dc.GetTextExtent(text, &textW, &textH);

        int x = 2;
        int y = rect.GetY() + wxMax(0, (rect.GetHeight() - textH) / 2);

        dc.DrawText(text, x, y);
    }
}

void CValueWindow::OnPaint(wxPaintEvent & WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (!m_treeCtrl)
        return;

    wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT), 1, wxSOLID);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    wxFont font(GetFont());
    dc.SetFont(font);
    dc.SetBackgroundMode(wxTRANSPARENT);

    wxSize clientSize = GetClientSize();
    wxRect itemRect;
    int cy = 0;
    wxTreeItemId h, lastH;
    for (h = m_treeCtrl->GetFirstVisibleItem(); h.IsOk(); h = m_treeCtrl->GetNextVisible(h)) {
        if (m_treeCtrl->GetBoundingRect(h, itemRect)) {
            cy = itemRect.GetTop();
            wxRect drawItemRect(0, cy, clientSize.x, itemRect.GetHeight());

            lastH = h;

            // Draw the actual item
            DrawItem(dc, h, drawItemRect);
            dc.DrawLine(0, cy, clientSize.x, cy);
        }
        if (!m_treeCtrl->IsVisible(h)) {
            break;
        }
    }
    if (lastH && lastH.IsOk() && m_treeCtrl->GetBoundingRect(lastH, itemRect)) {
        cy = itemRect.GetBottom();
        dc.DrawLine(0, cy, clientSize.x, cy);
    }
}

void CValueWindow::OnScroll(wxScrollWinEvent & event)
{
#if 1
    CTreeCompWindow::Refresh();
    (void)event;
#else
    wxScrolledWindow::OnScroll(event);
#endif
    PositionEditWindow();
}

void CValueWindow::OnExpand(wxTreeEvent & event)
{
    wxLogVerbose(wxT("CValueWindow::OnExpand"));
    CTreeCompWindow::OnExpand(event);

    EndEditing();
}

void CValueWindow::PositionEditWindow()
{
    if (!m_configItem || !m_editWindow)
        return;

    // Position the control
    wxSize clientSize = GetClientSize();
    wxRect itemRect;
    m_treeCtrl->GetBoundingRect(m_configItem->GetTreeItem(), itemRect);

    wxSize sz = m_editWindow->GetSize();

    m_editWindow->SetSize(0, itemRect.y, clientSize.x, sz.y);
}

/*!
 * \brief Stop editing.
 *
 * Retrieve the current value from the edit control and destroy
 * the control.
 */
bool CValueWindow::EndEditing()
{
    if (m_configItem) {
        if (m_editWindow)
            m_configItem->TransferDataFromWindow(m_editWindow);
        m_configItem = NULL;
    }

    if (m_editWindow) {
        delete m_editWindow;
        m_editWindow = NULL;
    }

    return true;
}

void CValueWindow::OnMouseEvent(wxMouseEvent & event)
{
    if (event.LeftDown() && (event.GetX() > 2)) {
        // Find if this corresponds to a tree item
        int flags = 0;
        wxTreeItemId item = m_treeCtrl->HitTest(wxPoint(4, event.GetY()), flags);
        if (item.IsOk()) {
            m_treeCtrl->SelectItem(item);
            CConfigItem *configItem = ((CTreeItemData *) m_treeCtrl->GetItemData(item))->GetConfigItem();
            if (configItem->CanEdit()) {
                BeginEditing(configItem);
            }
        }
    } else if (event.RightDown()) {
    }
}

/*
 * Creates an edit control and starts editing.
 */
bool CValueWindow::BeginEditing(CConfigItem * item)
{
    if (m_configItem)
        EndEditing();

    m_configItem = item;

    m_editWindow = item->CreateEditWindow(this);
    if (m_editWindow) {
        m_editWindow->Show(false);

        item->TransferDataToWindow(m_editWindow);

        // Position the control
        PositionEditWindow();

        m_editWindow->Show(true);
        m_editWindow->SetFocus();

        return true;
    } else {
        m_configItem = NULL;
        return false;
    }
}


wxRect CValueWindow::GetItemRect(CConfigItem * item)
{
    if (!m_treeCtrl)
        return wxRect(0, 0, 0, 0);

    wxSize clientSize = GetClientSize();
    wxRect itemRect;
    int cy = 0;
    wxTreeItemId h;
    for (h = m_treeCtrl->GetFirstVisibleItem(); h.IsOk(); h = m_treeCtrl->GetNextVisible(h)) {
        CTreeItemData *data = (CTreeItemData *) m_treeCtrl->GetItemData(h);
        if (data->GetConfigItem() == item) {
            if (m_treeCtrl->GetBoundingRect(h, itemRect)) {
                cy = itemRect.GetTop();
                wxRect drawItemRect(0, cy, clientSize.x, itemRect.GetHeight());
                return drawItemRect;
            } else
                return wxRect(0, 0, 0, 0);
        }
    }
    return wxRect(0, 0, 0, 0);
}

wxWindow *CValueWindow::GetEditWindow() const
{
    return m_editWindow;
}
