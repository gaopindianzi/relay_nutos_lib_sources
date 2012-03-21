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
 *
 * Mainly taken from wxWindows/contribs/gizmos, copyright by 
 *
 * Julian Smart, wxWindows licence.
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
 * $Log: treecomp.cpp,v $
 * Revision 1.5  2008/07/24 15:43:43  haraldkipp
 * Fixed component tree on Linux.
 *
 * Revision 1.4  2008/03/17 10:17:19  haraldkipp
 * Removed dispensable scroll handlers.
 *
 * Revision 1.3  2005/04/22 15:30:22  haraldkipp
 * Avoid compiler warnings.
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.2  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.1  2004/06/07 16:15:03  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "treecomp.h"

IMPLEMENT_CLASS(CTreeCompWindow, wxWindow)

BEGIN_EVENT_TABLE(CTreeCompWindow, wxWindow)
    EVT_PAINT(CTreeCompWindow::OnPaint)
    EVT_SCROLLWIN(CTreeCompWindow::OnScroll)
    EVT_TREE_ITEM_EXPANDED(-1, CTreeCompWindow::OnExpand)
    EVT_TREE_ITEM_COLLAPSED(-1, CTreeCompWindow::OnExpand)
END_EVENT_TABLE()

    CTreeCompWindow::CTreeCompWindow(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & sz, long style)
:wxWindow(parent, id, pos, sz, style)
{
    m_treeCtrl = NULL;
}

void CTreeCompWindow::DrawItem(wxDC & dc, wxTreeItemId id, const wxRect & rect)
{
    if (!m_treeCtrl)
        return;

    wxString text = m_treeCtrl->GetItemText(id);
    dc.SetTextForeground(*wxBLACK);
    dc.SetBackgroundMode(wxTRANSPARENT);

    int textW, textH;
    dc.GetTextExtent(text, &textW, &textH);
    dc.DrawText(text, 5, rect.GetY() + wxMax(0, (rect.GetHeight() - textH) / 2));
}

void CTreeCompWindow::OnPaint(wxPaintEvent & WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (!m_treeCtrl) {
        return;
    }

    wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT), 1, wxSOLID);
    dc.SetPen(pen);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    wxFont font(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT));
    dc.SetFont(font);

    wxSize clientSize = GetClientSize();
    wxRect itemRect;
    int cy;
    wxTreeItemId h, lastH;
    for (h = m_treeCtrl->GetFirstVisibleItem(); h.IsOk(); h = m_treeCtrl->GetNextVisible(h)) {
        if (m_treeCtrl->GetBoundingRect(h, itemRect)) {
            cy = itemRect.GetTop();
            wxRect drawItemRect(0, cy, clientSize.x, itemRect.GetHeight());
            lastH = h;
            DrawItem(dc, h, drawItemRect);
            dc.DrawLine(0, cy, clientSize.x, cy);
        }
    }
    if (lastH.IsOk() && m_treeCtrl->GetBoundingRect(lastH, itemRect)) {
        cy = itemRect.GetBottom();
        dc.DrawLine(0, cy, clientSize.x, cy);
    }
}

void CTreeCompWindow::OnScroll(wxScrollWinEvent& event)
{
    int orient = event.GetOrientation();
    if (orient == wxHORIZONTAL)
    {
        event.Skip();
        return;
    }
    if (!m_treeCtrl)
        return;

    // TODO: scroll the window physically instead of just refreshing.
    Refresh(true);
}

void CTreeCompWindow::OnExpand(wxTreeEvent & WXUNUSED(event))
{
    Refresh();
}

CScrolledTreeCtrl *CTreeCompWindow::GetTreeCtrl() const
{
    return m_treeCtrl;
}

void CTreeCompWindow::SetTreeCtrl(CScrolledTreeCtrl * treeCtrl)
{
    m_treeCtrl = treeCtrl;
}
