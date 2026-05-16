/*
 * Copyright (c) 2023, Meta
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
//#include <zephyr/usb/usb_device.h>
//#include <zephyr/drivers/uart.h>
int main(void)
{
 
   //const struct device *console = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
   //if (!device_is_ready(console)) {
    //    return -ENODEV;
    //}  
   //if (usb_enable(NULL) != 0) {
    //  return -1;
   // }

    /* Wait for DTR signal — host serial monitor has connected */
   // uint32_t dtr = 0;
   // while (!dtr) {
    //    uart_line_ctrl_get(console, UART_LINE_CTRL_DTR, &dtr);
    //    k_sleep(K_MSEC(100));
    //}
    printk("--------------- Inside main application loop-----------------------\n");
	printk("Hello, C++ world! Board: %s\t, SoC: %s\n", CONFIG_BOARD, CONFIG_SOC);
    printk("\n---------------End of main application loop--------------");	
	return 0;
}
