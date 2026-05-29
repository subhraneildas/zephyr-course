//#---------------------------------Homework L7-task1---------------------------
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#define SLEEP_TIME_MS 1000   
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void){
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(led_driver0));
    if(!device_is_ready(driver)){
            LOG_ERR("LED driver not ready yet!\n");
            return 0;
        }
        k_msleep(500);
        LOG_INF("LED driver is now ready!!\n");

    struct sensor_value val = {0};
    while (1) {
        sensor_sample_fetch(driver);
        k_msleep(SLEEP_TIME_MS);
        sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
        k_msleep(SLEEP_TIME_MS);
        }
    return 0;
}
