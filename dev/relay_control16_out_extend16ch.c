

#include <cfg/os.h>
#include <sys/device.h>
#include <sys/heap.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <dev/gpio.h>
#include <dev/relaycontrol.h>
#include <cfg/platform_def.h>
#include <dev/reset_avr.h>

#define THISINFO    0


static const unsigned char code_msk[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

unsigned char switch_signal_hold_time[32]    __attribute__ ((section (".noinit")));
unsigned char io_out[32/8]                   __attribute__ ((section (".noinit")));


typedef struct _RelayDataBlock
{
	unsigned char output[2];
	unsigned char delay_trig_enable[2];    //是否有触发模式
	unsigned char delay_on_off[2];         //是延时触发开，还是延时触发关
	unsigned char delay_time[2][8];        //延时，秒为单位
	unsigned char delay_time_set[2][8];    //延时，秒为单位
} RelayDataBlock;

RelayDataBlock    relay_dcb          __attribute__ ((section (".noinit")));


#if 0
#define DEBUG_LED1     4
#define DEBUG_LED2     3
#define DEBUG_LED_PORT AVRPORTE

static void debutset(uint8_t ledmsk)
{
	GpioPinSet(DEBUG_LED_PORT,DEBUG_LED1,(ledmsk&(1<<0))?0:1);
	GpioPinSet(DEBUG_LED_PORT,DEBUG_LED2,(ledmsk&(1<<1))?0:1);
}
#endif

void  relay_lunch_out(unsigned char * reg)
{
	unsigned char out = 0;

	//输出
	out = reg[0];

	GpioPinSet(AVRPORTD,4,(out&(1<<0))?1:0);
	GpioPinSet(AVRPORTD,5,(out&(1<<1))?1:0);
	GpioPinSet(AVRPORTD,6,(out&(1<<2))?1:0);
	GpioPinSet(AVRPORTD,7,(out&(1<<3))?1:0);
	GpioPinSet(AVRPORTB,4,(out&(1<<4))?1:0);
	GpioPinSet(AVRPORTB,5,(out&(1<<5))?1:0);
	GpioPinSet(AVRPORTB,6,(out&(1<<6))?1:0);
	GpioPinSet(AVRPORTB,7,(out&(1<<7))?1:0);

	NutSleep(1);
	GpioPinSetHigh(AVRPORTF, 4);
	NutSleep(1);
	GpioPinSetLow(AVRPORTF, 4);

	out = reg[1];

	GpioPinSet(AVRPORTD,4,(out&(1<<0))?1:0);
	GpioPinSet(AVRPORTD,5,(out&(1<<1))?1:0);
	GpioPinSet(AVRPORTD,6,(out&(1<<2))?1:0);
	GpioPinSet(AVRPORTD,7,(out&(1<<3))?1:0);
	GpioPinSet(AVRPORTB,4,(out&(1<<4))?1:0);
	GpioPinSet(AVRPORTB,5,(out&(1<<5))?1:0);
	GpioPinSet(AVRPORTB,6,(out&(1<<6))?1:0);
	GpioPinSet(AVRPORTB,7,(out&(1<<7))?1:0);

	NutSleep(1);
	GpioPinSetHigh(AVRPORTE, 2);
	NutSleep(1);
	GpioPinSetLow(AVRPORTE, 2);
}

THREAD(relay_control_thread, arg)
{
	//unsigned char led = 0;
	NutThreadSetPriority(99);
	if(THISINFO)printf("Start relay_control_thread\r\n");
	for(;;) {
#if 0
		unsigned char i;
		for(i=0;i<8;i++) {
			if((relay_dcb.delay_trig_enable[0])&code_msk[i]) {
				if((relay_dcb.delay_on_off[0])&code_msk[i]) {
					if(relay_dcb.delay_time[0][i] > 0) {
						if((relay_dcb.delay_time[i] - 1) == 0) {
							//延时开，打开继电器
							relay_dcb.output[0] |= code_msk[i];
						}
					}
				} else {
					if(relay_dcb.delay_time[0][i] > 0) {
						if(relay_dcb.delay_time[0][i] - 1 == 0) {
							//延时关，关闭继电器
							relay_dcb.output[0] &= code_msk[i];
						}
					}
				}
			}
		}
		for(i=0;i<8;i++) {
			if((relay_dcb.delay_trig_enable[1])&code_msk[i]) {
				if((relay_dcb.delay_on_off[1])&code_msk[i]) {
					if(relay_dcb.delay_time[1][i] > 0) {
						if((relay_dcb.delay_time[i] - 1) == 0) {
							//延时开，打开继电器
							relay_dcb.output[1] |= code_msk[i];
						}
					}
				} else {
					if(relay_dcb.delay_time[1][i] > 0) {
						if(relay_dcb.delay_time[1][i] - 1 == 0) {
							//延时关，关闭继电器
							relay_dcb.output[1] &= code_msk[i];
						}
					}
				}
			}
		}
#endif
		relay_lunch_out(io_out);
		//debutset(led++);
		NutSleep(10);
	}
}

static void StartIoOutControlSrever(void)
{
    NutThreadCreate("relay_control_thread",  relay_control_thread, 0, 512);
}

int relay_init(NUTDEVICE * dev)
{
	FILE * file;
	unsigned char type;

	if(THISINFO)printf("Start relay_init\r\n");

	GpioPinSetLow(AVRPORTF, 4);
	GpioPinSetLow(AVRPORTF, 5);
	GpioPinSetLow(AVRPORTE, 2);
	GpioPinConfigSet(AVRPORTF, 4, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTF, 5, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTE, 2, GPIO_CFG_OUTPUT);

	GpioPinConfigSet(AVRPORTD, 4, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTD, 5, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTD, 6, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTD, 7, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTB, 4, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTB, 5, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTB, 6, GPIO_CFG_OUTPUT);
	GpioPinConfigSet(AVRPORTB, 7, GPIO_CFG_OUTPUT);

	NutRegisterDevice(&devAvrResetCtl, 0, 0);
	file = fopen("resetctl", "w+b");
	_ioctl(_fileno(file), GET_RESET_TYPE, &type);
	fclose(file);

	if(type&AVR_EXT_RESET) {
		//memset(&relay_dcb,0,sizeof(relay_dcb));//复位,其他情况，不复位
		//relay_lunch_out(relay_dcb.output);
		memset(switch_signal_hold_time,0,sizeof(switch_signal_hold_time));
		memset(io_out,0,sizeof(io_out));
	} else {
		//char * pmem = &relay_dcb;
		//pmem += 2;
		//memset(pmem,0,sizeof(relay_dcb)-2);  //复位延时参数，继电器不改变
	}
	relay_lunch_out(io_out);

	StartIoOutControlSrever();

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

	//debugled(0x03);

    return fp;
}
int RealyClose(NUTFILE * fp)
{
    NutHeapFree(fp);

	//debugled(0x01);

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
			unsigned char i;
			io_out[0] = pbuf[0];
			io_out[1] = pbuf[1];
			io_out[2] = pbuf[2];
			io_out[3] = pbuf[3];
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[0]&msk) {
					switch_signal_hold_time[i] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[1]&msk) {
					switch_signal_hold_time[i+8] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[2]&msk) {
					switch_signal_hold_time[i+16] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[3]&msk) {
					switch_signal_hold_time[i+24] = 80;
				}
			}
			rc = 0;
		}
		break;
	case IO_OUT_GET:
		{
			pbuf[0] = io_out[0];
			pbuf[1] = io_out[1];
			pbuf[2] = io_out[2];
			pbuf[3] = io_out[3];
			rc = 0;
		}
		break;
	case IO_IN_GET:
		{
			pbuf[0] = 0;
			pbuf[1] = 0;
			rc = 0;
		}
		break;
	case GET_OUT_NUM:
		{
			uint32_t * pdat = (uint32_t *)pbuf;
			*pdat = 32;
			rc = 0;
		}
		break;
	case GET_IN_NUM:
		{
			uint32_t * pdat = (uint32_t *)pbuf;
			*pdat = 0;
			rc = 0;
		}
		break;
	case IO_SET_BITMAP:
		{
			unsigned char i;
			io_out[0] |= pbuf[0];
			io_out[1] |= pbuf[1];
			io_out[2] |= pbuf[2];
			io_out[3] |= pbuf[3];
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[0]&msk) {
					switch_signal_hold_time[i] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[1]&msk) {
					switch_signal_hold_time[i+8] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[2]&msk) {
					switch_signal_hold_time[i+16] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[3]&msk) {
					switch_signal_hold_time[i+24] = 80;
				}
			}
			rc = 0;
		}
		break;
	case IO_CLR_BITMAP:
		{
			unsigned char i;
			io_out[0] &= ~pbuf[0];
			io_out[1] &= ~pbuf[1];
			io_out[2] &= ~pbuf[2];
			io_out[3] &= ~pbuf[3];
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[0]&msk) {
					switch_signal_hold_time[i] = 0;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[1]&msk) {
					switch_signal_hold_time[i+8] = 0;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[2]&msk) {
					switch_signal_hold_time[i+16] = 0;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[3]&msk) {
					switch_signal_hold_time[i+24] = 0;
				}
			}
			rc = 0;
		}
		break;
	case IO_SET_ONEBIT:
		{
			unsigned int index = pbuf[1];
			index <<= 8;
			index  |= pbuf[0];
			if(index < 8) {
				io_out[0] |=  code_msk[index];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			} else if(index < 16) {
				io_out[1] |=  code_msk[index-8];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			} else if(index < 24) {
				io_out[2] |=  code_msk[index-16];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			} else if(index < 32) {
				io_out[3] |=  code_msk[index-24];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			}
		}
		break;
	case IO_GET_ONEBIT:
		{
			unsigned int index = pbuf[1];
			index <<= 8;
			index  |= pbuf[0];
			if(index < 8) {
				if(io_out[0]&code_msk[index]) {
					pbuf[1] = pbuf[0] = 0xFF;
				} else {
					pbuf[1] = pbuf[0] = 0x00;
				}
				rc = 0;
			} else if(index < 16) {
				if(io_out[1]&code_msk[index-8]) {
					pbuf[1] = pbuf[0] = 0xFF;
				} else {
					pbuf[1] = pbuf[0] = 0x00;
				}
				rc = 0;
			} else if(index < 24) {
				if(io_out[2]&code_msk[index-16]) {
					pbuf[1] = pbuf[0] = 0xFF;
				} else {
					pbuf[1] = pbuf[0] = 0x00;
				}
				rc = 0;
			} else if(index < 32) {
				if(io_out[3]&code_msk[index-24]) {
					pbuf[1] = pbuf[0] = 0xFF;
				} else {
					pbuf[1] = pbuf[0] = 0x00;
				}
				rc = 0;
			}
		}
		break;
	case IO_CLR_ONEBIT:
		{
			unsigned int index = pbuf[1];
			index <<= 8;
			index  |= pbuf[0];
			if(index < 8) {
				io_out[0] &= ~code_msk[index];
				switch_signal_hold_time[index] = 0;
				rc = 0;
			} else if(index < 16) {
				io_out[1] &= ~code_msk[index-8];
				switch_signal_hold_time[index] = 0;
				rc = 0;
			} else if(index < 24) {
				io_out[2] &= ~code_msk[index-16];
				switch_signal_hold_time[index] = 0;
				rc = 0;
			} else if(index < 32) {
				io_out[3] &= ~code_msk[index-24];
				switch_signal_hold_time[index] = 0;
				rc = 0;
			}
		}
		break;
	case IO_SIG_BITMAP:
		{
			unsigned char i;
			io_out[0] ^= pbuf[0];
			io_out[1] ^= pbuf[1];
			io_out[2] ^= pbuf[2];
			io_out[3] ^= pbuf[3];
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[0]&msk) {
					switch_signal_hold_time[i] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[1]&msk) {
					switch_signal_hold_time[i+8] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[2]&msk) {
					switch_signal_hold_time[i+16] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[3]&msk) {
					switch_signal_hold_time[i+24] = 80;
				}
			}
			rc = 0;
		}
		break;
	case IO_SIG_ONEBIT:
		{
			unsigned int index = pbuf[1];
			index <<= 8;
			index  |= pbuf[0];
			if(index < 8) {
				io_out[0] ^= code_msk[index];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			} else if(index < 16) {
				io_out[1] ^= code_msk[index - 8];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			} else if(index < 24) {
				io_out[2] ^= code_msk[index - 16];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			} else if(index < 32) {
				io_out[3] ^= code_msk[index - 24];
				switch_signal_hold_time[index] = 80;
				rc = 0;
			}
		}
		break;
	case IO_SET_DELAY_ENABLE:
		{
			if(pbuf[0] < 8) {
				unsigned char msk = code_msk[pbuf[0]];
				if(pbuf[0]&msk) {
				    dcb->delay_trig_enable[0] |=  msk;
				} else {
					dcb->delay_trig_enable[0] &= ~msk;
				}
				rc = 0;
			} else if(pbuf[0] < 16) {
				unsigned char msk = code_msk[pbuf[0]-1];
				if(pbuf[1]&msk) {
				    dcb->delay_trig_enable[1] |=  msk;
				} else {
					dcb->delay_trig_enable[1] &= ~msk;
				}
				rc = 0;
			}
		}
		break;
	case IO_SET_DELAY_ONOFF:
		{
			if(pbuf[0] < 8) {
				unsigned char msk = code_msk[pbuf[0]];
				if(pbuf[0]&msk) {
				    dcb->delay_on_off[0] |=  msk;
				} else {
					dcb->delay_on_off[0] &= ~msk;
				}
				rc = 0;
			} else if(pbuf[0] < 16) {
				unsigned char msk = code_msk[pbuf[0]-1];
				if(pbuf[1]&msk) {
				    dcb->delay_on_off[1] |=  msk;
				} else {
					dcb->delay_on_off[1] &= ~msk;
				}
				rc = 0;
			}
		}
		break;
	case IO_SET_DELAY_TIME:
		{
			if(pbuf[0] < 8) {
				dcb->delay_time_set[0][pbuf[0]] = pbuf[1];
				rc = 0;
			} else if(pbuf[0] < 16) {
				dcb->delay_time_set[1][pbuf[0]-1] = pbuf[1];
				rc = 0;
			}
		}
		break;
	default:
		break;
	}

/*
	switch(req)
	{
	case IO_OUT_SET:
	case IO_SET_ONEBIT:
		{
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[0]&msk) {
					switch_signal_hold_time[i] = 80;
				}
			}
			for(i=0;i<8;i++) {
				unsigned char msk = code_msk[i];
				if(pbuf[1]&msk) {
					switch_signal_hold_time[i+8] = 80;
				}
			}
		}
		break;
	default:
		break;
	}
	*/

	return rc;
}

