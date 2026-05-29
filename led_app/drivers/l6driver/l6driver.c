#include <zephyr/drivers/sensor.h>
#include<zephyr/logging/log.h>
#include<zephyr/drivers/gpio.h>
//#include<zephyr/device.h>
#define DT_DRV_COMPAT my_l6driver

LOG_MODULE_REGISTER(l6driver,LOG_LEVEL_INF); 

struct led_driver_config{
    struct gpio_dt_spec led;
}; 
//Channel fetch functionality
static int channel_fetch_my_impl(const struct device *dev, enum sensor_channel chan){
    const struct led_driver_config *cfg = dev->config;
    gpio_pin_set_dt(&cfg->led,1);
    //LOG_INF("Fetch channel %d - LED is On!\n", chan);
    return 0;  
    }
//Channel get functionality
static int channel_get_my_impl(const struct device *dev,
                    enum sensor_channel chan,
                    struct sensor_value *val){
    const struct led_driver_config *cfg = dev->config;
    gpio_pin_set_dt(&cfg->led, 0);
    //LOG_INF("Get Channel %d - LED is Off!\n", chan);
    return 0;
              }

static DEVICE_API(sensor, api_led) = {
    .sample_fetch = channel_fetch_my_impl,
    .channel_get= channel_get_my_impl,
};

static int init(const struct device *dev){
    const struct led_driver_config *cfg = dev->config;

    if(!gpio_is_ready_dt(&cfg->led)){
        LOG_ERR("LED GPIO not ready yet!!!!\n");
        return -ENODEV;
    }
    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE); //so that the GPIO pin is always set as an output pin
    LOG_INF("LED Device Driver Initialized!!\n");
    return 0;
}

 #define DEV_INST(inst) static const struct led_driver_config led_driver_config_##inst = {.led = GPIO_DT_SPEC_INST_GET(inst,gpios),};  DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, &led_driver_config_##inst, POST_KERNEL, 80, &api_led);

 DT_INST_FOREACH_STATUS_OKAY(DEV_INST);
