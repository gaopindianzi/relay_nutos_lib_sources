#include <cfg/os.h>
#include <sys/device.h>
#include <sys/heap.h>
#include <io.h>
#include <dev/gpio.h>
#include <dev/relaycontrol.h>
#include <cfg/platform_def.h>
#include <sys/timer.h>
#include <sys/thread.h>


#ifdef RELAY_PLATFORM_8CHIN_8CHOUT
#include "relay_control8in8outv1.c"
#endif

#ifdef RELAY_PLATFORM_16CHOUT
#include "relay_control16outv1.c"
#endif

#ifdef RELAY_PLATFORM_2CHIN_2CHOUT
#include "relay_control2in2out.c"
#endif

#ifdef RELAY_PLATFORM_4CHIN_4CHOUT
#include "relay_control4in4out.c"
#endif

#ifdef ETHERNET_TO_485_MIMIBOARD_V1
#include "relay_control_none.c"
#endif

NUTDEVICE devRelayInputOutput = {
    0,                          /*!< Pointer to next device. */
    {'r', 'e', 'l', 'a', 'y', 'c', 't', 'l', 0,},      /*!< Unique device name. */
    IFTYP_STREAM,               /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    NULL,                 /*!< Interface control block. */
    &relay_dcb,                 /*!< Driver control block. */
    relay_init,                /*!< Driver initialization routine. */
    relay_ioctl,               /*!< Driver specific control function. */
    NULL,                /*!< Read from device. */
    NULL,               /*!< Write to device. */
    NULL,             /*!< Write to device from program space. */
    RealyOpen,                /*!< Open a device or file. */
    RealyClose,               /*!< Close a device or file. */
    NULL                 /*!< Request file size. */
};

