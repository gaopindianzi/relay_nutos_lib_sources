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
 *
 * ----------------------------------------------------------------------------
 *
 * Mainly taken from wxWindows/contribs/gizmos, copyright by 
 *
 * Julian Smart, wxWindows licence.
 *
 * ----------------------------------------------------------------------------
 */

/*
 * $Log: scrolledtree.cpp,v $
 * Revision 1.8  2009/03/08 17:44:34  haraldkipp
 * Fixed vertical scrollbar bug.
 *
 * Revision 1.7  2008/07/24 15:43:43  haraldkipp
 * Fixed component tree on Linux.
 *
 * Revision 1.6  2008/03/17 10:19:47  haraldkipp
 * Get it run on OS X.
 *
 * Revision 1.5  2005/04/26 12:49:36  haraldkipp
 * Minor wxWidgets runtime warning avoided.
 *
 * Revision 1.4  2005/04/22 15:26:21  haraldkipp
 * Upgraded to wxWidgets 2.5.5.
 *
 * Revision 1.3  2004/08/18 13:34:20  haraldkipp
 * Now working on Linux
 *
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:13:15  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <math.h>

#include "scrolledtree.h"

#ifdef __WXMSW__
#include <wx/msw/private.h>
#include <wx/window.h>
#endif
#ifdef __WXGTK__
#include <wx/generic/treectlg.h>
#include <wx/scrolbar.h>
#endif

#ifdef __WXGTK__
IMPLEMENT_CLASS(CScrolledTreeCtrl, wxGenericTreeCtrl)
#else
IMPLEMENT_CLASS(CScrolledTreeCtrl, wxTreeCtrl)
#endif

#ifdef __WXGTK__
BEGIN_EVENT_TABLE(CScrolledTreeCtrl, wxGenericTreeCtrl)
#else
BEGIN_EVENT_TABLE(CScrolledTreeCtrl, wxTreeCtrl)
#endif
    EVT_SIZE(CScrolledTreeCtrl::OnSize)
    EVT_TREE_ITEM_EXPANDED(-1, CScrolledTreeCtrl::OnExpand)
    EVT_TREE_ITEM_COLLAPSED(-1, CScrolledTreeCtrl::OnExpand)
    EVT_SCROLLWIN(CScrolledTreeCtrl::OnScroll)
    EVT_PAINT(CScrolledTreeCtrl::OnPaint)
END_EVENT_TABLE();

CScrolledTreeCtrl::CScrolledTreeCtrl(wxWindow * parent, wxWindowID id, const wxPoint & pt,
     const wxSize & sz, long style)
#ifdef __WXGTK__
     :wxGenericTreeCtrl(parent, id, pt, sz, style & ~wxTR_ROW_LINES)
#else
     :wxTreeCtrl(parent, id, pt, sz, style & ~wxTR_ROW_LINES)
#endif
{
    m_companionWindow = NULL;
    m_drawRowLines = (style & wxTR_ROW_LINES) != 0;
}

CScrolledTreeCtrl::~CScrolledTreeCtrl()
{
}

void CScrolledTreeCtrl::HideVScrollbar()
{
#ifdef __WXMSW__
    ::ShowScrollBar((HWND) GetHWND(), SB_VERT, false);
#else
    /* No idea how to remove the vertical scrollbar. */
#endif
}

void CScrolledTreeCtrl::DoCalcScrolledPosition(int x, int y, int *xx, int *yy) const
{
#if USE_GENERIC_TREECTRL || !defined(__WXMSW__)
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;
        * yy = 0;
        int yyy;
        win->wxGenericTreeCtrl::DoCalcScrolledPosition(x, y, xx, & yyy);

        wxScrolledWindow* scrolledWindow = GetScrolledWindow();
        if (scrolledWindow)
        {
            int xxx;
            scrolledWindow->DoCalcScrolledPosition(x, y, & xxx, yy);
        }
    }
#endif
}

void CScrolledTreeCtrl::SetScrollbar(int orient,
                               int pos,
                               int thumbVisible,
                               int range,
                               bool update)
{
#ifndef __WXMSW__
    if (orient == wxVERTICAL)
        range = 0;
#endif
    wxWindow::SetScrollbar(orient, pos, thumbVisible, range, update);
}

void CScrolledTreeCtrl::SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                                      int noUnitsX, int noUnitsY, 
                                      int xPos, int yPos, bool noRefresh)
{
#ifdef __WXGTK__
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;
        /*
         * In the original code y-units and y-pos had been set
         * to zero to remove the vertical scrollbar. However,
         * since wxScrollHelper had been implemented, this
         * doesn't work anymore. See wxWidgets ticket #4482.
         */
        win->wxGenericTreeCtrl::SetScrollbars(pixelsPerUnitX, pixelsPerUnitY, noUnitsX, noUnitsY, xPos, yPos, true);

        wxScrolledWindow *scrolledWindow = GetScrolledWindow();
        if (scrolledWindow) {
            scrolledWindow->SetScrollbars(0, pixelsPerUnitY, 0, noUnitsY, 0, yPos, noRefresh);
        }
    }
#else
    (void)pixelsPerUnitX; 
    (void)pixelsPerUnitY;
    (void)noUnitsX;
    (void)noUnitsY;
    (void)xPos; 
    (void)yPos;
    (void)noRefresh;
#endif
}

int CScrolledTreeCtrl::GetScrollPos(int orient) const
{
#ifdef __WXGTK__
    wxScrolledWindow *scrolledWindow = GetScrolledWindow();
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;

        if (orient == wxHORIZONTAL) {
            return win->wxGenericTreeCtrl::GetScrollPos(orient);
        } else if (scrolledWindow) {
            return scrolledWindow->GetScrollPos(orient);
        }
    }
#else
    (void)orient;
#endif
    return 0;
}

void CScrolledTreeCtrl::GetViewStart(int *x, int *y) const
{
    wxScrolledWindow *scrolledWindow = GetScrolledWindow();

#ifdef __WXGTK__
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl)))
    {
        wxGenericTreeCtrl* win = (wxGenericTreeCtrl*) this;
        int x1, y1, x2, y2;
        win->wxGenericTreeCtrl::GetViewStart(& x1, & y1);
        *x = x1; 
        *y = y1;
        if (!scrolledWindow) {
            return;
        }
        scrolledWindow->GetViewStart(&x2, &y2);
        *y = y2;
    }
    else
#endif
    {
        // x is wrong since the horizontal scrollbar is controlled by the
        // tree control, but we probably don't need it.
        scrolledWindow->GetViewStart(x, y);
    }
}

void CScrolledTreeCtrl::PrepareDC(wxDC & dc)
{
#ifdef __WXGTK__
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        wxScrolledWindow *scrolledWindow = GetScrolledWindow();

        wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;

        int startX, startY;
        GetViewStart(&startX, &startY);

        int xppu1, yppu1, xppu2, yppu2;
        win->wxGenericTreeCtrl::GetScrollPixelsPerUnit(&xppu1, &yppu1);
        scrolledWindow->GetScrollPixelsPerUnit(&xppu2, &yppu2);

        dc.SetDeviceOrigin(-startX * xppu1, -startY * yppu2);
    }
#else
    (void)dc;
#endif
}

/*!
 * \brief Scroll to a given line.
 *
 * \param posHoriz Unused.
 * \param posVert  The line number to scroll to.
 *
 * \todo posHoriz not used, should be removed.
 */
void CScrolledTreeCtrl::ScrollToLine(int posHoriz, int posVert)
{
    wxLogVerbose(wxT("CScrolledTreeCtrl::ScrollToLine(%d,%d)"), posHoriz, posVert);
#ifdef __WXMSW__
    {
        UINT sbCode = SB_THUMBPOSITION;
        HWND vertScrollBar = 0;
        MSWDefWindowProc((WXUINT) WM_VSCROLL, MAKELONG(sbCode, posVert), (WXLPARAM) vertScrollBar);
    }
#endif
#ifdef __WXGTK__
    wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;
    win->Refresh();
#endif
}

void CScrolledTreeCtrl::OnSize(wxSizeEvent & event)
{
    wxLogVerbose(wxT("CScrolledTreeCtrl::OnSize"));
    AdjustRemoteScrollbars();
    HideVScrollbar();
    event.Skip();
}

void CScrolledTreeCtrl::OnExpand(wxTreeEvent & event)
{
    AdjustRemoteScrollbars();
    HideVScrollbar();
    Refresh();

    // Pass on the event
    event.Skip();
    if (m_companionWindow)
        m_companionWindow->GetEventHandler()->ProcessEvent(event);
}

/*
 * Paint the row lines.
 */
void CScrolledTreeCtrl::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

#ifdef __WXGTK__
    wxGenericTreeCtrl::OnPaint(event);
#else
    wxTreeCtrl::OnPaint(event);
#endif

#ifndef __WXMAC__
    if (! m_drawRowLines)
        return;

    // Reset the device origin since it may have been set
    dc.SetDeviceOrigin(0, 0);

    wxPen pen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT), 1, wxSOLID);
    dc.SetPen(pen);
    dc.SetBrush(* wxTRANSPARENT_BRUSH);

    wxSize clientSize = GetClientSize();
    wxRect itemRect;
    int cy=0;
    wxTreeItemId h, lastH;
    for(h = GetFirstVisibleItem(); h.IsOk(); h = GetNextVisible(h)) {
        if (GetBoundingRect(h, itemRect)) {
            cy = itemRect.GetTop();
            dc.DrawLine(0, cy, clientSize.x, cy);
            lastH = h;
        }
        if (!IsVisible(h)) {
            break;
        }
    }
    if (lastH.IsOk() && GetBoundingRect(lastH, itemRect)) {
        cy = itemRect.GetBottom();
        dc.DrawLine(0, cy, clientSize.x, cy);
    }
#endif
}

// Adjust the containing wxScrolledWindow's scrollbars appropriately
void CScrolledTreeCtrl::AdjustRemoteScrollbars()
{
#ifdef __WXGTK__
    if (IsKindOf(CLASSINFO(wxGenericTreeCtrl))) {
        // This is for the generic tree control.
        // It calls SetScrollbars which has been overridden
        // to adjust the parent scrolled window vertical
        // scrollbar.
        ((wxGenericTreeCtrl *) this)->AdjustMyScrollbars();
        return;
    } else
#endif
    {
        // This is for the wxMSW tree control
        wxScrolledWindow *scrolledWindow = GetScrolledWindow();
        if (scrolledWindow) {
            wxRect itemRect;
            if (GetBoundingRect(GetRootItem(), itemRect)) {
                // Actually, the real height seems to be 1 less than reported
                // (e.g. 16 instead of 16)
                int itemHeight = itemRect.GetHeight() - 1;

                int w, h;
                GetClientSize(&w, &h);

                wxRect rect(0, 0, 0, 0);
                CalcTreeSize(rect);

                double f = ((double) (rect.GetHeight()) / (double) itemHeight);
                int treeViewHeight = (int) ceil(f);

                int scrollPixelsPerLine = itemHeight;
                int scrollPos = -(itemRect.y / itemHeight);

                scrolledWindow->SetScrollbars(0, scrollPixelsPerLine, 0, treeViewHeight, 0, scrollPos);

                // Ensure that when a scrollbar becomes hidden or visible,
                // the contained window sizes are right.
                // Problem: this is called too early (?)
                wxSizeEvent event(scrolledWindow->GetSize(), scrolledWindow->GetId());
                scrolledWindow->GetEventHandler()->ProcessEvent(event);
            }
        }
    }
}

// Calculate the area that contains both rectangles
static wxRect CombineRectangles(const wxRect & rect1, const wxRect & rect2)
{
    wxRect rect;

    int right1 = rect1.GetRight();
    int bottom1 = rect1.GetBottom();
    int right2 = rect2.GetRight();
    int bottom2 = rect2.GetBottom();

    wxPoint topLeft = wxPoint(wxMin(rect1.x, rect2.x), wxMin(rect1.y, rect2.y));
    wxPoint bottomRight = wxPoint(wxMax(right1, right2), wxMax(bottom1, bottom2));

    rect.x = topLeft.x;
    rect.y = topLeft.y;
    rect.SetRight(bottomRight.x);
    rect.SetBottom(bottomRight.y);

    return rect;
}


void CScrolledTreeCtrl::CalcTreeSize(wxRect & rect)
{
    CalcTreeSize(GetRootItem(), rect);
}

void CScrolledTreeCtrl::CalcTreeSize(const wxTreeItemId & id, wxRect & rect)
{
    // More efficient implementation would be to find the last item (but how?)
    // Q: is the bounding rect relative to the top of the virtual tree workspace
    // or the top of the window? How would we convert?
    wxRect itemSize;
    if (GetBoundingRect(id, itemSize)) {
        rect = CombineRectangles(rect, itemSize);
    }

    wxTreeItemIdValue cookie;
    wxTreeItemId childId = GetFirstChild(id, cookie);
    while (childId) {
        CalcTreeSize(childId, rect);
        childId = GetNextChild(childId, cookie);
    }
}


wxScrolledWindow *CScrolledTreeCtrl::GetScrolledWindow() const
{
    //wxLogVerbose(wxT("CScrolledTreeCtrl::GetScrolledWindow"));
    wxWindow *parent = wxWindow::GetParent();
    while (parent) {
        if (parent->IsKindOf(CLASSINFO(wxScrolledWindow))) {
            return (wxScrolledWindow *) parent;
        }
        parent = parent->GetParent();
    }
    wxLogVerbose(wxT("CScrolledTreeCtrl::GetScrolledWindow(!!NONE!!)"));
    return NULL;
}

/*!
 * \brief Process scroll event.
 *
 * \param event Event information.
 */
void CScrolledTreeCtrl::OnScroll(wxScrollWinEvent & event)
{
    /*
     * Horizontal scrolling events should be processed by the next
     * event handler in the chain.
     */
    int orient = event.GetOrientation();
    if (orient == wxHORIZONTAL) {
        event.Skip();
        return;
    }

#ifdef __WXGTK__
    int nScrollInc = CalcScrollInc(event);
    m_yScrollPosition += nScrollInc;
    SetScrollPos(wxVERTICAL, m_yScrollPosition, true);
    wxGenericTreeCtrl *win = (wxGenericTreeCtrl *) this;
    win->Refresh();
#else
    wxScrolledWindow *scrollWin = GetScrolledWindow();
    if (scrollWin) {
        int x, y;
        scrollWin->GetViewStart(&x, &y);
        ScrollToLine(-1, y);
    }
#endif
}

void CScrolledTreeCtrl::SetCompanionWindow(wxWindow * companion)
{
    m_companionWindow = companion;
}

wxWindow *CScrolledTreeCtrl::GetCompanionWindow() const
{
    return m_companionWindow;
}
