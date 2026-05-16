/*
 * Copyright (c) 2020 Jefferson Lee.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/init.h>
//#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

//static int board_my_board_scratch_init(void){
 //To print sth before entering the main loop of application	

//}

static int board_init(void)
{

	//int res;
	/*static const struct gpio_dt_spec pull_up =
		GPIO_DT_SPEC_GET(DT_PATH(zephyr_user), pull_up_gpios);
	static const struct gpio_dt_spec user_led =
		GPIO_DT_SPEC_GET(DT_ALIAS(led4), gpios);

	if (!gpio_is_ready_dt(&pull_up)) {
		return -ENODEV;
	}

	if (!gpio_is_ready_dt(&user_led)) {
		return -ENODEV;
	}*/
	printk("-----------Booting....Before main loop begins. Test Message--------------\n");
	printk("Board %s with SoC:%s from scratch initialised\n", CONFIG_BOARD, CONFIG_SOC);
	printk("\n-----------End of Test Message and now entering main loop----------------");
	return 0;
	//res = gpio_pin_configure_dt(&pull_up, GPIO_OUTPUT_HIGH);
	//if (res) {
	//	return res;
	//}
	//return gpio_pin_configure_dt(&user_led, GPIO_OUTPUT_HIGH);
}

SYS_INIT(board_init, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE);
