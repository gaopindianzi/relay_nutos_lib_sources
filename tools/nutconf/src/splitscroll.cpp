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
 * $Log: splitscroll.cpp,v $
 * Revision 1.6  2008/03/17 10:21:12  haraldkipp
 * OS X uses a different window hierarchy, so we need to search for the
 * scrollbar instead of simply taking the first one.
 *
 * Revision 1.5  2005/05/22 14:58:35  haraldkipp
 * Compile failed on OS X. MacUpdateImmediately() seems to be vanished.
 *
 * Revision 1.4  2005/04/22 15:27:19  haraldkipp
 * Avoid compiler warnings.
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

#include "splitscroll.h"

IMPLEMENT_CLASS(CSplitScroll, wxScrolledWindow)

BEGIN_EVENT_TABLE(CSplitScroll, wxScrolledWindow)
    EVT_SCROLLWIN(CSplitScroll::OnScroll)
    EVT_SIZE(CSplitScroll::OnSize)
END_EVENT_TABLE()

/* 
 * \brief Constructs a splitted scroll window with two children.
 */
CSplitScroll::CSplitScroll(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & sz, long style)
:  wxScrolledWindow(parent, id, pos, sz, style)
{
}

void CSplitScroll::OnSize(wxSizeEvent & WXUNUSED(event))
{

    wxSize sz = GetClientSize();

    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node) {
        wxWindow* child = node->GetData();
        if (!child->IsKindOf(CLASSINFO(wxScrollBar))) {
            child->SetSize(0, 0, sz.x, sz.y);
            break;
        }
        node = node->GetNext();
    }
}

void CSplitScroll::OnScroll(wxScrollWinEvent & event)
{
    // Ensure that events being propagated back up the window hierarchy
    // don't cause an infinite loop
    static bool scrolling = false;
    if (scrolling) {
        event.Skip();
        return;
    }

    /*
     * Horizontal scrolling will be done by the next handler in the chain.
     */
    int orient = event.GetOrientation();
    if (orient == wxHORIZONTAL) {
        event.Skip();
        return;
    }

    int nScrollInc = CalcScrollInc(event);
    if (nScrollInc == 0) {
        return;
    }


    scrolling = true;

    int newPos = m_yScrollPosition + nScrollInc;
    SetScrollPos(wxVERTICAL, newPos, true);

    m_yScrollPosition += nScrollInc;

    // Find targets in splitter window and send the event to them
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node) {
        wxWindow* child = (wxWindow*) node->GetData();
        if (child->IsKindOf(CLASSINFO(wxSplitterWindow))) {
            wxSplitterWindow* splitter = (wxSplitterWindow*) child;
            if (splitter->GetWindow1())
                splitter->GetWindow1()->ProcessEvent(event);
            if (splitter->GetWindow2())
                splitter->GetWindow2()->ProcessEvent(event);
            break;
        }
        node = node->GetNext();
    }

    scrolling = false;
}
