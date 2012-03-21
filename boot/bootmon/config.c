/*
 * Copyright (C) 2005-2007 by egnite Software GmbH
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * For additional information see http://www.ethernut.de/
 *
 */

/*
 * $Id: config.c 2935 2010-04-01 12:14:17Z haraldkipp $
 */

#include "config.h"
#include "utils.h"
#include "dialog.h"
#include "nvmem.h"

/*!
 * \brief Nut/OS network configuration.
 */
CONFNET confnet = {
    0,
    {'E', 'T', 'H', 'E', 'R', 'N', 'U', 'T', 0}
    ,
    {0x00, 0x06, 0x98, 0x30, 0x00, 0x00}
    ,
    0, 0, 0, 0
};

/*
 * \brief Boot info structure.
 */
CONFBOOT confboot;

#ifdef EE_CONFNET

static unsigned char ConfigGet(unsigned int loc)
{
    unsigned char b;

    if (NvMemRead(loc, &b, 1)) {
        return 0;
    }
    return b;
}

#endif

/*!
 * \brief Read IP and boot configuration from non-volatile memory.
 */
int BootConfigRead(void)
{
    int rc;

#ifdef EE_CONFNET
    rc = -1;
    NvMemInit();
    DEBUG("CONFNET ");
    if (ConfigGet(EE_CONFNET) == sizeof(CONFNET)) {
        DEBUG("OK\n");
        rc = NvMemRead(EE_CONFNET, &confnet, sizeof(CONFNET));
#ifdef EE_CONFBOOT
        DEBUG("CONFBOOT ");
        if (rc == 0 && ConfigGet(EE_CONFBOOT) == sizeof(CONFBOOT)) {
            DEBUG(" OK\n");
            rc = NvMemRead(EE_CONFBOOT, &confboot, sizeof(CONFBOOT));
        } else {
            DEBUG("No\n");
        }
#endif
    } else {
        DEBUG("No\n");
    }
#else
    rc = 0;
#endif
    return rc;
}

/*!
 * \brief Store IP and boot configuration in non-volatile memory.
 */
void BootConfigWrite(void)
{
#ifdef EE_CONFNET
    NvMemInit();

    confnet.cd_size = sizeof(CONFNET);
    NvMemWrite(EE_CONFNET, &confnet, sizeof(CONFNET));
#ifdef EE_CONFBOOT
    confboot.cb_size = sizeof(CONFBOOT);
    NvMemWrite(EE_CONFBOOT, &confboot, sizeof(CONFBOOT));
#endif                          /* EE_CONFBOOT */
#endif                          /* EE_CONFNET */
}
