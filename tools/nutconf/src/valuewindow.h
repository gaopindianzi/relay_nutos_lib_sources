#ifndef VALUEWINDOW_H_
#define VALUEWINDOW_H_

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
 * $Log$
 * Revision 1.1  2004/06/07 16:15:03  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include "treecomp.h"
#include "configitem.h"

class CValueWindow:public CTreeCompWindow {
  public:
    CValueWindow(wxWindow * parent, wxWindowID id = -1, const wxPoint & pos = wxDefaultPosition,
                 const wxSize & sz = wxDefaultSize, long style = 0);

    virtual void DrawItem(wxDC & dc, wxTreeItemId id, const wxRect & rect);

    void OnPaint(wxPaintEvent & event);
    void OnMouseEvent(wxMouseEvent & event);
    void OnScroll(wxScrollWinEvent & event);
    void OnExpand(wxTreeEvent & event);

    wxWindow *GetEditWindow() const;

    bool BeginEditing(CConfigItem * item);
    bool EndEditing();
    void PositionEditWindow();
    wxRect GetItemRect(CConfigItem * item);

  protected:
    wxWindow * m_editWindow;
    CConfigItem *m_configItem;

    DECLARE_EVENT_TABLE();
    DECLARE_CLASS(CValueWindow);
};

#endif
