#ifndef __RELAY_CONTROL_DRIVER_H__
#define __RELAY_CONTROL_DRIVER_H__

#include <sys/device.h>

#define  IO_OUT_SET        0
#define  IO_OUT_GET        1
#define  IO_IN_GET         2
#define  GET_OUT_NUM       3
#define  GET_IN_NUM        4
#define  IO_SET_ONEBIT     5
#define  IO_CLR_ONEBIT     6
#define  IO_GET_ONEBIT     14
#define  IO_SET_BITMAP     7
#define  IO_CLR_BITMAP     8
#define  IO_SIG_BITMAP     9
#define  IO_SIG_ONEBIT     10
#define  IO_SET_DELAY_ENABLE   11
#define  IO_SET_DELAY_ONOFF    12
#define  IO_SET_DELAY_TIME     13


extern NUTDEVICE devRelayInputOutput;

#endif
