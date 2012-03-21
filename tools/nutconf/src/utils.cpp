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
 * $Log: utils.cpp,v $
 * Revision 1.3  2006/10/05 17:04:46  haraldkipp
 * Heavily revised and updated version 1.3
 *
 * Revision 1.2  2005/02/06 20:00:44  haraldkipp
 * Missing errno included.
 *
 * Revision 1.1  2004/06/07 16:15:03  haraldkipp
 * Complete redesign based on eCos' configtool
 *
 */

#include <errno.h>
#include "utils.h"

bool CUtils::StrToItemIntegerType(const wxString & str, long &d)
{
    wxChar *pEnd;

    errno = 0;

    bool bHex = (str.Len() > 2 && str[0] == wxT('0') && (str[1] == wxT('x') || str[1] == wxT('X')));

    d = wxStrtol(str, &pEnd, bHex ? 16 : 10);
    return (errno == 0 && (*pEnd == wxT('\0')));
}

bool CUtils::FindString(const wxString& findIn, const wxString& findWhat, bool wholeWord)
{
    wxString remain(findIn);

    if (wholeWord) {
        size_t s = findWhat.Length();

        while (!remain.IsEmpty()) {
            int i = remain.Find(findWhat);

            if (i == wxNOT_FOUND) {
                break;
            }
            if ((i == 0 || !wxIsalnum(remain[i - 1])) && (i + s == remain.Length() || !wxIsalnum(remain[i + s]))) {
                return true;
            }
            remain = remain.Mid(i + 1);
        }
    }
    else if (remain.Find(findWhat) != wxNOT_FOUND) {
        return true;
    }
    return false;
}
