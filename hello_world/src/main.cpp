/*
 * Copyright (c) 2023, Meta
 *
 * SPDX-License-Identifier: Apache-2.0
 */

//#include <iostream>
#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
int main(void)
{
 
	const struct device *usb_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

   if (usb_enable(NULL) != 0) {
      return -1;
    }

    /* Wait for DTR signal — host serial monitor has connected */
    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(usb_dev, UART_LINE_CTRL_DTR, &dtr);
        k_sleep(K_MSEC(100));
    }
	//std::cout << "Hello, C++ world! " << CONFIG_BOARD << std::endl;
	printk("Hello, C++ world! %s\n", CONFIG_BOARD);	
	return 0;
}
