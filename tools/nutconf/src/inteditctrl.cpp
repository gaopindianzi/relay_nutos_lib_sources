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
 * $Log: inteditctrl.cpp,v $
 * Revision 1.3  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.2  2005/04/22 15:13:52  haraldkipp
 * Avoid compiler warnings.
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "valuewindow.h"
#include "inteditctrl.h"

BEGIN_EVENT_TABLE(CIntEditCtrl, wxSpinCtrl)
    EVT_TEXT_ENTER(-1, CIntEditCtrl::OnEnter)
    EVT_KILL_FOCUS(CIntEditCtrl::OnKillFocus)
    END_EVENT_TABLE();

IMPLEMENT_CLASS(CIntEditCtrl, wxSpinCtrl);

CIntEditCtrl::CIntEditCtrl(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style)
: wxSpinCtrl(parent, id, wxEmptyString, pos, size, style, -32000, 32000, 0)
{
}

/*!
 * \brief Enter key pressed in edit control.
 */
void CIntEditCtrl::OnEnter(wxCommandEvent & WXUNUSED(event))
{
    CValueWindow *parent = (CValueWindow *) GetParent();
    parent->EndEditing();
}

/*!
 * \brief Edit control loses focus.
 */
void CIntEditCtrl::OnKillFocus(wxFocusEvent & WXUNUSED(event))
{
    CValueWindow *parent = (CValueWindow *) GetParent();
    parent->EndEditing();
}
