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
 * $Log: enumeditctrl.cpp,v $
 * Revision 1.4  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.3  2005/04/26 12:48:28  haraldkipp
 * Workaround for wxChoice focus bug on GTK+.
 *
 * Revision 1.2  2005/04/22 15:13:04  haraldkipp
 * Avoid compiler warnings.
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "valuewindow.h"
#include "enumeditctrl.h"


BEGIN_EVENT_TABLE(CEnumEditCtrl, wxChoice)
    EVT_CHAR(CEnumEditCtrl::OnChar)
    EVT_KILL_FOCUS(CEnumEditCtrl::OnKillFocus)
    EVT_ENTER_WINDOW(CEnumEditCtrl::OnMouseEnter)
    EVT_LEAVE_WINDOW(CEnumEditCtrl::OnMouseLeave)
END_EVENT_TABLE();

IMPLEMENT_CLASS(CEnumEditCtrl, wxChoice);

CEnumEditCtrl::CEnumEditCtrl(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style)
    :wxChoice(parent, id, pos, size, 0, 0, style)
    ,m_mouseIsOver(true)
{
}

void CEnumEditCtrl::OnChar(wxKeyEvent & event)
{
    if (event.GetKeyCode() == WXK_RETURN) {
        CValueWindow *parent = (CValueWindow *) GetParent();
        parent->EndEditing();
    } else {
        event.Skip();
    }
}

/*!
 * \brief Edit control loses focus.
 */
void CEnumEditCtrl::OnKillFocus(wxFocusEvent &event)
{
    if (!m_mouseIsOver) {
        CValueWindow *parent = (CValueWindow *) GetParent();
        parent->EndEditing();
    } else {
        m_mouseIsOver = false;
        event.Skip();
    }
}

/*!
 * \brief Called when the mouse leaves the control.
 *
 * Used to fix a bug in the GTK version of wxWidgets 2.6.0.
 */
void CEnumEditCtrl::OnMouseEnter(wxMouseEvent& event)
{
    m_mouseIsOver = true;
    event.Skip();
}

/*!
 * \brief Called when the mouse leaves the control.
 *
 * Used to fix a bug in the GTK version of wxWidgets 2.6.0.
 */
void CEnumEditCtrl::OnMouseLeave(wxMouseEvent& event)
{
    m_mouseIsOver = false;
    event.Skip();
}
