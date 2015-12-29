#include <rtthread.h>
#include "enc28j60.h"

void INT_handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	enc28j60_isr();

	/* leave interrupt */
	rt_interrupt_leave();
}

void rt_hw_enc28j60_init(void)
{
    /* config RESET IO : P16 */

    /* reset */
    FM3_GPIO->PDOR1 &= ~(1<<6);
    rt_thread_delay(1);
    /* release reset */
    FM3_GPIO->PDOR1 |=  (1<<6);

    enc28j60_attach("spi20");

    /* settting INT : P1A INT05_1 */
	/* ÅäÖÃÖÐ¶Ï */
}
