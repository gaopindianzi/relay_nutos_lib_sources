#ifndef __RESET_H__
#define __RESET_H__

#include <cfg/os.h>
#include <sys/device.h>
#include <sys/heap.h>
#include <io.h>
#include <dev/gpio.h>
#include <dev/relaycontrol.h>
#include <cfg/platform_def.h>
#include <sys/timer.h>
#include <sys/thread.h>


#define AVR_JTAG_RESET         (1<<4)
#define AVR_WDT_RESET          (1<<3)
#define AVR_POWER_DOWN_RESET   (1<<2)
#define AVR_EXT_RESET          (1<<1)
#define AVR_POWER_UP_RESET     (1<<0)


#define  GET_RESET_TYPE        0
#define  SET_RESET             1


extern NUTDEVICE devAvrResetCtl;

#endif
