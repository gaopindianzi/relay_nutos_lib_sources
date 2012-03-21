#include <cfg/os.h>
#include <sys/device.h>
#include <sys/heap.h>
#include <io.h>
#include <dev/gpio.h>
#include <dev/reset_avr.h>
#include <sys/timer.h>
#include <sys/thread.h>
#include <sys/file.h>
#include <dev/watchdog.h>

#define AVR_JTAG_RESET         (1<<4)
#define AVR_WDT_RESET          (1<<3)
#define AVR_POWER_DOWN_RESET   (1<<2)
#define AVR_EXT_RESET          (1<<1)
#define AVR_POWER_UP_RESET     (1<<0)

static unsigned char reset_type;
static unsigned char device_will_reset;

unsigned char AvrResetType(void)
{
	unsigned char reg = inb(MCUCSR);
	outb(MCUCSR,0);
	return reg;
}


THREAD(wdt_control_thread, arg)
{
	NutThreadSetPriority(50);
	NutSleep(10000);
	NutWatchDogStart(5000, 0);
	while(1) {
		if(!device_will_reset) {
			NutWatchDogRestart();
			NutSleep(800);
		} else {
			NutSleep(10000);
		}
	}
}

static void StartWdtControlSrever(void)
{
    NutThreadCreate("wdt_control_thread",  wdt_control_thread, 0, 256);
}


int reset_init(NUTDEVICE * dev)
{
	reset_type = AvrResetType();
	device_will_reset = 0;
	StartWdtControlSrever();
	return 0;
}


NUTFILE *ResetOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *fp = NutHeapAlloc(sizeof(NUTFILE));

    if (fp == 0)
        return NUTFILE_EOF;

    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;
    return fp;
}

int ResetClose(NUTFILE * fp)
{
    NutHeapFree(fp);
    return 0;
}

int reset_ioctl(NUTDEVICE *dev, int req, void *conf)
{
	int rc = -1;
	unsigned char * type = (unsigned char *)conf;
    if (dev == 0)
        dev = &devAvrResetCtl;

	switch(req)
	{
	case GET_RESET_TYPE:
		{
			*type = reset_type;
			rc = 0;
		}
		break;
	case SET_RESET:
		{
			device_will_reset = 1;
			rc = 0;
		}
		break;
	default:
		break;
	}

	return rc;
}


NUTDEVICE devAvrResetCtl = {
    0,                          /*!< Pointer to next device. */
    {'r', 'e', 's', 'e', 't', 'c', 't', 'l', 0,},      /*!< Unique device name. */
    IFTYP_STREAM,               /*!< Type of device. */
    0,                          /*!< Base address. */
    0,                          /*!< First interrupt number. */
    NULL,                 /*!< Interface control block. */
    NULL,                 /*!< Driver control block. */
    reset_init,                /*!< Driver initialization routine. */
    reset_ioctl,               /*!< Driver specific control function. */
    NULL,                /*!< Read from device. */
    NULL,               /*!< Write to device. */
    NULL,             /*!< Write to device from program space. */
    ResetOpen,                /*!< Open a device or file. */
    ResetClose,               /*!< Close a device or file. */
    NULL                 /*!< Request file size. */
};

