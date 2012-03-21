

#include <cfg/os.h>
#include <sys/device.h>
#include <sys/heap.h>
#include <io.h>
#include <stdio.h>
#include <dev/gpio.h>
#include <dev/relaycontrol.h>
#include <cfg/platform_def.h>
#include <dev/reset_avr.h>


typedef struct _RelayDataBlock
{
	unsigned char input[1];
	unsigned char output[1];
} RelayDataBlock;

RelayDataBlock relay_dcb          __attribute__ ((section (".noinit")));


int relay_init(NUTDEVICE * dev)
{
	return 0;
}


NUTFILE *RealyOpen(NUTDEVICE * dev, CONST char *name, int mode, int acc)
{
    NUTFILE *fp = NutHeapAlloc(sizeof(NUTFILE));

    if (fp == 0)
        return NUTFILE_EOF;

    fp->nf_next = 0;
    fp->nf_dev = dev;
    fp->nf_fcb = 0;

    return fp;
}
int RealyClose(NUTFILE * fp)
{
    NutHeapFree(fp);

    return 0;
}


//

int relay_ioctl(NUTDEVICE *dev, int req, void *conf)
{
	int rc = -1;
	unsigned char * pbuf = (unsigned char *)conf;
	RelayDataBlock * dcb = NULL;
    if (dev == 0)
        dev = &devRelayInputOutput;

    dcb = dev->dev_dcb;

	switch(req)
	{
	case IO_OUT_SET:
		{
			dcb->output[0] = pbuf[0];
			rc = 0;
		}
		break;
	case IO_OUT_GET:
		{
			pbuf[0] = dcb->output[0];
			rc = 0;
		}
		break;
	case IO_IN_GET:
		{
			pbuf[0] = dcb->input[0];
			rc = 0;
		}
		break;
	case GET_OUT_NUM:
	case GET_IN_NUM:
		{
			uint32_t * pdat = (uint32_t *)conf;
			*pdat = 8;
			rc = 0;
		}
		break;
	default:
		break;
	}

	return rc;
}

