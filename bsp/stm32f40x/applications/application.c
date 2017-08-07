/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
/*begin:add by wxc : in 2017.0717 for add led */
#ifdef RT_USING_LED
#include <drivers/pin.h>
#endif
/*end:add by wxc : in 2017.0717 for add led */

/*begin:add by wxc : in 2017.0718 for add serial1 */		
#ifdef RT_USING_UART1
#include <drivers/usart.h>
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#endif
/*end:add by wxc : in 2017.0718 for add serial1 */		
#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif
/*begin:add by wxc : in 2017.07.19 for add sdio and SD card */	
#ifdef RT_USING_SDIO_DFS
#include <ff.h>
#endif
/*end:add by wxc : in 2017.07.19 for add sdio and SD card */	

#ifdef RT_USING_USB_DEVICE_SD
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 
#endif
rt_thread_t tid[10];

void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart1");
    gdb_start();
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif
/*begin:add by wxc : in 2017.0717 for add led */
#ifdef RT_USING_LED
		// PE5,PE6   12 13
		rt_pin_mode(12,PIN_MODE_OUTPUT);
		rt_pin_mode(13,PIN_MODE_OUTPUT);
		for(;;)
		{
			rt_pin_write(12,PIN_LOW);
			rt_pin_write(13,PIN_HIGH);
			rt_thread_delay(100);
			rt_pin_write(12,PIN_HIGH);
			rt_pin_write(13,PIN_LOW);
			rt_thread_delay(100);		
		}
		
#endif
/*end:add by wxc : in 2017.0717 for add led */		
}
/*begin:add by wxc : in 2017.0718 for add serial1 */		
#ifdef RT_USING_UART1
rt_device_t Usart1 = &(serial1.parent);
void rt_serial_thread_entry(void* parameter)
{
	
//	struct rt_serial_rx_fifo* rx_fifo = (struct rt_serial_rx_fifo *)serial1.serial_rx;
	serial1.parent.init(Usart1);
	serial1.parent.open(Usart1,RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_RX);
	rt_uint8_t buff[1025];
	rt_uint8_t sendbuff[1025];
	
	
	
	for(;;)
	{
		rt_thread_delay(1);
		//serial1.parent.write(Usart1,0,(rt_uint8_t *)"hello world wxc,RT\r\n",20);				
		int recvCount = serial1.parent.read(Usart1,0,&buff[0],1000);
		if(recvCount > 0)
		{
			sprintf((char *)sendbuff,"%d,%s\r\n",recvCount,buff);
			serial1.parent.write(Usart1,0,sendbuff,strlen((char *)sendbuff));							
			memset(sendbuff,0,sizeof(sendbuff));
			memset(buff,0,sizeof(buff));
		}
	}
}
#endif
/*end:add by wxc : in 2017.0718 for add serial1 */		

/*begin:add by wxc : in 2017.07.19 for add sdio and SD card */		
#ifdef RT_USING_SDIO_DFS

FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"感谢野火的例子STM32 F429开发板 今天是个好日子，新建文件系统测试文件\r\n"; 
void rt_dfs_thread_entry(void* parameter)
{
	//rt_uint8_t sendbuff[1025] = {"3123213213123cnskcbejvbsancsjac ,mznc\r\n"};		
		
    res_sd = f_mount(&fs,"0:",1);
	

	if(res_sd == FR_NO_FILESYSTEM)
	{
		    
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			res_sd = f_mount(NULL,"0:",1);			      
			res_sd = f_mount(&fs,"0:",1);
		}
        else
        {
            //error
        }
	}
    else if(res_sd!=FR_OK)
    {

        //error
    }
    else
    {
    //ok
    }
    f_mkdir("wxc");
    //f_chdir("wxc");
    res_sd = f_open(&fnew, "0:Fatfs.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {    
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_sd==FR_OK)
    {      
        //ok
    }
    else
    {
      //error
    }    
        /* 不再读写，关闭文件 */
    f_close(&fnew);
    }
    else
    {	
        //error
    }
	
	for(;;)
	{
		rt_thread_delay(100);
			
	}
}
#endif
/*end:add by wxc : in 2017.07.19 for add sdio and SD card */	

#ifdef RT_USING_USB_DEVICE_SD

USB_OTG_CORE_HANDLE USB_OTG_dev;

void rt_usb_thread_entry(void* parameter)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2	
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	for(;;)
	{
		rt_thread_delay(100);
	}
}
#endif

int rt_application_init()
{
    
		int i = 0 ;
    tid[i] = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid[i] != RT_NULL)
        rt_thread_startup(tid[i]);
		i++;
/*begin:add by wxc : in 2017.0718 for add serial1 */				
#ifdef RT_USING_UART1
    tid[i] = rt_thread_create("serial",
        rt_serial_thread_entry, RT_NULL,
        2048*2, RT_THREAD_PRIORITY_MAX/3 +1, 18);

    if (tid[i] != RT_NULL)
        rt_thread_startup(tid[i]);
		i++;
#endif
/*end:add by wxc : in 2017.0718 for add serial1 */				


/*begin:add by wxc : in 2017.07.19 for add sdio and SD card */	
#ifdef RT_USING_SDIO_DFS
    tid[i] = rt_thread_create("SD0",
        rt_dfs_thread_entry, RT_NULL,
        2048*2, RT_THREAD_PRIORITY_MAX/3 +2, 19);

    if (tid[i] != RT_NULL)
        rt_thread_startup(tid[i]);
		i++;
#endif
/*end:add by wxc : in 2017.07.19 for add sdio and SD card */		
#ifdef RT_USING_USB_DEVICE_SD
    tid[i] = rt_thread_create("USB",
        rt_usb_thread_entry, RT_NULL,
        2048*10, RT_THREAD_PRIORITY_MAX/4, 19);

    if (tid[i] != RT_NULL)
        rt_thread_startup(tid[i]);
		i++;
#endif		
    return 0;
}

/*@}*/
