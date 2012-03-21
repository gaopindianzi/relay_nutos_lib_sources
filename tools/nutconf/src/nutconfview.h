#ifndef NUTCONFVIEW_H_
#define NUTCONFVIEW_H_

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
 * Revision 1.2  2004/08/03 15:03:25  haraldkipp
 * Another change of everything
 *
 * Revision 1.1  2004/06/07 16:11:22  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/docview.h>

class CConfigItem;

/*!
 * \brief Configuration view class.
 */
class CNutConfView:public wxView {
    DECLARE_DYNAMIC_CLASS(CNutConfView);
  public:
    CNutConfView();
    ~CNutConfView() {
    };

    bool OnCreate(wxDocument * doc, long flags);
    void OnDraw(wxDC * dc);
    void OnUpdate(wxView * sender, wxObject * hint = (wxObject *) NULL);
    bool OnClose(bool deleteWindow = true);
    void OnChangeFilename();

    void Refresh(const wxString & macroName);
    void Refresh(wxTreeItemId h);

    DECLARE_EVENT_TABLE();

  protected:
    wxTreeItemId m_expandedForFind;
};


#endif
