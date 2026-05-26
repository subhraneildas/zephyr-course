//#---------------------------------Homework L6-task1---------------------------
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>   // restored — sensor_sample_fetch / sensor_channel_get
#include "l6driver.h"                // custom API: l6driver_set/get_blink_speed
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

//namespace{
   // void test(){
        //const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(led_driver));
   //     struct sensor_value val;
   //     sensor_sample_fetch(dev);
        //LOG_INF("Channel fetch acquired: %d\n",ret1);
  //      auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
  //      LOG_INF("Channel returned: %d\n",ret);
   // }
//}

int main(void){
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(led_driver0));
    if(!device_is_ready(driver)){
            LOG_ERR("LED driver not ready yet!\n");
            return 0;
        }
        k_msleep(500);
        LOG_INF("LED driver is now ready!!\n");

    struct sensor_value val = {0};

    // Set an initial blink speed via the custom API (overwrites the 500 ms default
    // stored in the driver's dynamic data struct)
    l6driver_set_blink_speed(driver, 200);

   // test();
    while (1) {
        // LED on — via sensor API (sample_fetch)
        sensor_sample_fetch(driver);
        k_msleep(l6driver_get_blink_speed(driver));   // sleep duration from dynamic struct

        // LED off — via sensor API (channel_get)
        sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
        k_msleep(l6driver_get_blink_speed(driver));   // same period for off time

        // Gradually slow the blink speed to show the parameter changing at runtime
        uint32_t speed = l6driver_get_blink_speed(driver);
        if (speed < 1000) {
            l6driver_set_blink_speed(driver, speed + 100);
        }
        else{
            speed=200;//resetting the blink loop for led
            l6driver_set_blink_speed(driver, speed);
        }
        LOG_INF("Current blink period: %u ms", l6driver_get_blink_speed(driver));
        }
    return 0;
}
