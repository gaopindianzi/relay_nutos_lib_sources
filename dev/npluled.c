/*
 * Copyright (C) 2005 by egnite Software GmbH. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY EGNITE SOFTWARE GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL EGNITE
 * SOFTWARE GMBH OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 */

/*!
 * \brief Low Level User LED (Green) Access.
 *
 * Low level User LED (Green) hardware routines for the programmable logic provided
 * on the Ethernut 3 reference design.
 **
 * \verbatim
 **
 * Revision 1.0  2006/01/22 10:01:02  alex (k.vassliev)
 * First check-in.
 *
 *
 * \endverbatim
 */

#include <dev/npl.h>
#include <dev/npluled.h>

#if 0
/* Use for local debugging. */
#define NUTDEBUG
#include <stdio.h>
#endif

/*!
 * \brief User LED control routine.
 *
 * \param status ULED_ON of ULED_OFF
 *
 */

void NplUledCntl(int status)
{
    if (status == ULED_ON) {
        /*! \brief Activate negated chip select. */
        outb(NPL_XER, inb(NPL_XER) & ~NPL_USRLED);
    } else {
        if (status == ULED_OFF) {
            /*! \brief Deactivate negated chip select. */
            outb(NPL_XER, inb(NPL_XER) | NPL_USRLED);
        }
    }
}

/*!
 * \brief User LED status routine.
 *
 * \return The ULED Status (ULED_ON or ULED_OFF).
*
 */

int NplUledStatus(void)
{
    int rc = (inb(NPL_XER) & NPL_USRLED) == NPL_USRLED;

    return rc;
}
