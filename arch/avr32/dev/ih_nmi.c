/*!
 * Copyright (C) 2001-2010 by egnite Software GmbH
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
 */


/*
 * $Log: ih_nmi.c,v $
 *
 */

#include <arch/avr32.h>
#include <dev/irqreg.h>
#include <dev/gpio.h>

#include <arch/avr32/gpio.h>
#include <arch/avr32/ihndlr.h>

#include <avr32/io.h>

static int InterruptNMICtl(int cmd, void *param);

IRQ_HANDLER sig_INTERRUPTNMI = {
#ifdef NUT_PERFMON
    0,                          /* Interrupt counter, ir_count. */
#endif
    NULL,                       /* Passed argument, ir_arg. */
    NULL,                       /* Handler subroutine, ir_handler. */
    InterruptNMICtl             /* Interrupt control, ir_ctl. */
};

/*!
 * \brief External interrupt NMI entry.
 */
SIGNAL(InterruptNMIEntry)
{
    /*
     * We can't use our IRQ_ENTRY() macro here, because this is special, it comes
     * directly from the exception handler.
     * There is also no automatic saving of registers. But both GM and EM are
     * set, so we don't need to worry about preempted interrupts.
     */
    __asm__ __volatile__("pushm r0-r12, lr");

#ifdef NUT_PERFMON
    sig_INTERRUPTNMI.ir_count++;
#endif
    if (sig_INTERRUPTNMI.ir_handler) {
        (sig_INTERRUPTNMI.ir_handler) (sig_INTERRUPTNMI.ir_arg);
    }

    /* Clear interrupt */
    AVR32_EIC.icr = AVR32_EIC_ICR_NMI_MASK;
    AVR32_EIC.isr;

    __asm__ __volatile__("popm r0-r12,lr");
}

/*!
 * \brief External interrupt NMI control.
 *
 * \param cmd   Control command.
 *              - NUT_IRQCTL_INIT Initialize and disable interrupt.
 *              - NUT_IRQCTL_STATUS Query interrupt status.
 *              - NUT_IRQCTL_ENABLE Enable interrupt.
 *              - NUT_IRQCTL_DISABLE Disable interrupt.
 *              - NUT_IRQCTL_GETCOUNT Query and clear interrupt counter.
 * \param param Pointer to optional parameter.
 *
 * \return 0 on success, -1 otherwise.
 */
static int InterruptNMICtl(int cmd, void *param)
{
    int rc = 0;
    unsigned int *ival = (unsigned int *) param;
    int_fast8_t enabled = AVR32_EIC.imr & AVR32_EIC_IMR_NMI_MASK;

    /*  Disable interrupt. */
    if (enabled) {
        AVR32_EIC.idr = AVR32_EIC_IDR_NMI_MASK;
        AVR32_EIC.imr;
    }

    switch (cmd) {
    case NUT_IRQCTL_INIT:
        /* Setup Peripheral mux for interrupt line */
        gpio_enable_module_pin(AVR32_EIC_EXTINT_8_PIN, AVR32_EIC_EXTINT_8_FUNCTION);

        /* Initialize to edge triggered with defined priority. */
        AVR32_EIC.mode &= ~AVR32_EIC_MODE_NMI_MASK;

        /* Clear interrupt */
        AVR32_EIC.icr = AVR32_EIC_ICR_NMI_MASK;
        break;
    case NUT_IRQCTL_STATUS:
        if (enabled) {
            *ival |= 1;
        } else {
            *ival &= ~1;
        }
        break;
    case NUT_IRQCTL_ENABLE:
        enabled = 1;
        break;
    case NUT_IRQCTL_DISABLE:
        enabled = 0;
        break;
    case NUT_IRQCTL_GETMODE:
        {
            if (AVR32_EIC.mode & AVR32_EIC_MODE_NMI_MASK) {
                if (AVR32_EIC.level & AVR32_EIC_LEVEL_NMI_MASK)
                    *ival = NUT_IRQMODE_HIGHLEVEL;
                else
                    *ival = NUT_IRQMODE_LOWLEVEL;
            } else {
                if (AVR32_EIC.edge & AVR32_EIC_EDGE_NMI_MASK)
                    *ival = NUT_IRQMODE_RISINGEDGE;
                else
                    *ival = NUT_IRQMODE_FALLINGEDGE;
            }
        }
        break;
    case NUT_IRQCTL_SETMODE:
        if (*ival == NUT_IRQMODE_LOWLEVEL) {
            AVR32_EIC.mode |= AVR32_EIC_MODE_NMI_MASK;
            AVR32_EIC.level &= ~AVR32_EIC_LEVEL_NMI_MASK;
        } else if (*ival == NUT_IRQMODE_HIGHLEVEL) {
            AVR32_EIC.mode |= AVR32_EIC_MODE_NMI_MASK;
            AVR32_EIC.level |= ~AVR32_EIC_LEVEL_NMI_MASK;
        } else if (*ival == NUT_IRQMODE_FALLINGEDGE) {
            AVR32_EIC.mode &= ~AVR32_EIC_MODE_NMI_MASK;
            AVR32_EIC.edge &= ~AVR32_EIC_EDGE_NMI_MASK;
        } else if (*ival == NUT_IRQMODE_RISINGEDGE) {
            AVR32_EIC.mode &= ~AVR32_EIC_MODE_NMI_MASK;
            AVR32_EIC.edge |= ~AVR32_EIC_EDGE_NMI_MASK;
        } else {
            rc = -1;
        }
        break;
#ifdef NUT_PERFMON
    case NUT_IRQCTL_GETCOUNT:
        *ival = (unsigned int) sig_INTERRUPTNMI.ir_count;
        sig_INTERRUPTNMI.ir_count = 0;
        break;
#endif
    default:
        rc = -1;
        break;
    }

    /* Enable interrupt. */
    if (enabled) {
        AVR32_EIC.ier = AVR32_EIC_IER_NMI_MASK;
        AVR32_EIC.imr;
        AVR32_EIC.en |= AVR32_EIC_EN_NMI_MASK;
    }
    return rc;
}
