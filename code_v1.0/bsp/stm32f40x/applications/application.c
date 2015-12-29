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

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#include "enc28j60.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

#ifdef RT_USING_DFS
#include <dfs_fs.h>
#endif

void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif
		
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    usr_components_init();
#endif
		/* initialization RT-Thread Components */
		rt_platform_init();

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        //rt_hw_stm32_eth_init();
				enc28j60_attach("spi11");	
        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif
	
#ifdef RT_USING_DFS
   {
		rt_bool_t mount_flag = RT_FALSE;

#if (RT_DFS_ELM_USE_LFN != 0) && (defined RT_DFS_ELM_CODE_PAGE_FILE)
		{
				extern void ff_convert_init(void);
				ff_convert_init();
		}
#endif

		if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
		{
				rt_kprintf("sd0 mount to /SD \n");
		}
		else
		{
				rt_kprintf("sd0 mount to /SD failed!\n");
			  rt_kprintf("retry!\n");
			  //dfs_mkfs("elm", "sd0");

				/* re-try mount. */
				if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
				{
						mount_flag = RT_TRUE;
				}
		}
	}
#endif /* RT_USING_DFS */
	rt_thread_delay(500);	
	file_test();
	
//	udpclient("192.168.2.208",5000,10);
	tcpclient("114.80.207.247",5005);//"114.80.207.247"
}

void rt_dbg_thread_entry(void* parameter)
{
		uint16_t reg;
		uint8_t eir;
		struct rt_spi_device * spi_device;
		spi_device = (struct rt_spi_device *)rt_device_find("spi11");
    
		while(1)
		{
			reg = enc28j60_phy_read(spi_device, PHSTAT1);
			rt_kprintf("PHSTAT1=%x\r\n",PHSTAT1);
		
			reg = enc28j60_phy_read(spi_device, PHSTAT2);
			rt_kprintf("PHSTAT2=%x\r\n",PHSTAT2);
			
			eir = spi_read(spi_device, ESTAT);
			rt_kprintf("ESTAT=%x\r\n",ESTAT);
			
			eir = spi_read(spi_device, EIR);
			rt_kprintf("eir=%x\r\n",eir);
			rt_thread_delay(500);	
		}
}

int rt_application_init()
{
    rt_thread_t tid,tid1;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

		tid1 = rt_thread_create("enc_dbg",
			rt_dbg_thread_entry, RT_NULL,
			1024, RT_THREAD_PRIORITY_MAX/3+3, 20);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
		
    return 0;
}

/*@}*/
