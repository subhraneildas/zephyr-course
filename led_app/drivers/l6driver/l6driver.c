// #include <zephyr/drivers/sensor.h>   // replaced by custom API header
#include "l6driver.h"
#include<zephyr/logging/log.h>
#include<zephyr/drivers/gpio.h>
//#include<zephyr/device.h>
#define DT_DRV_COMPAT my_l6driver

LOG_MODULE_REGISTER(l6driver,LOG_LEVEL_INF);

// Static (ROM) config — unchanged
struct led_driver_config{
    struct gpio_dt_spec led;
};

// Dynamic (RAM) data — mutable runtime state (new)
struct led_driver_data {
    uint32_t toggle_count;   // incremented on every on/off call
};

// // Old sensor API: channel_fetch turned LED on
// static int channel_fetch_my_impl(const struct device *dev, enum sensor_channel chan){
//     const struct led_driver_config *cfg = dev->config;
//     gpio_pin_set_dt(&cfg->led,1);
//     LOG_INF("Fetch channel %d - LED is On!\n", chan);
//     return 0;
//     }
// // Old sensor API: channel_get turned LED off
// static int channel_get_my_impl(const struct device *dev,
//                     enum sensor_channel chan,
//                     struct sensor_value *val){
//     const struct led_driver_config *cfg = dev->config;
//     gpio_pin_set_dt(&cfg->led, 0);
//     LOG_INF("Get Channel %d - LED is Off!\n", chan);
//     return 0;
//               }

// Custom API implementations (new)
static int led_on_impl(const struct device *dev)
{
    const struct led_driver_config *cfg  = dev->config;
    struct led_driver_data         *data = dev->data;
    gpio_pin_set_dt(&cfg->led, 1);
    data->toggle_count++;
    LOG_INF("LED On  — toggle count: %u", data->toggle_count);
    return 0;
}

static int led_off_impl(const struct device *dev)
{
    const struct led_driver_config *cfg  = dev->config;
    struct led_driver_data         *data = dev->data;
    gpio_pin_set_dt(&cfg->led, 0);
    data->toggle_count++;
    LOG_INF("LED Off — toggle count: %u", data->toggle_count);
    return 0;
}

static int get_toggle_count_impl(const struct device *dev, uint32_t *count)
{
    const struct led_driver_data *data = dev->data;
    *count = data->toggle_count;
    return 0;
}

// // Old sensor API vtable
// static DEVICE_API(sensor, api_led) = {
//     .sample_fetch = channel_fetch_my_impl,
//     .channel_get= channel_get_my_impl,
// };

// Custom API vtable (new)
static const struct l6driver_api api_led = {
    .on               = led_on_impl,
    .off              = led_off_impl,
    .get_toggle_count = get_toggle_count_impl,
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

// Updated DEV_INST: added led_driver_data_##inst for the dynamic struct,
// and passed it as the 4th argument (data ptr) to DEVICE_DT_INST_DEFINE.
// Previously the 4th arg was NULL (no dynamic data).
 #define DEV_INST(inst)                                                         \
    static const struct led_driver_config led_driver_config_##inst = {         \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios),                             \
    };                                                                          \
    static struct led_driver_data led_driver_data_##inst = {                   \
        .toggle_count = 0,                                                      \
    };                                                                          \
    DEVICE_DT_INST_DEFINE(inst, init, NULL,                                     \
                          &led_driver_data_##inst,                              \
                          &led_driver_config_##inst,                            \
                          POST_KERNEL, 80, &api_led);

 DT_INST_FOREACH_STATUS_OKAY(DEV_INST);

// DEVICE_DT_INST_DEFINE(0,
//                      init,
//                      NULL,
//                      NULL,                  // was: no data struct
//                      &led_driver_config0,
//                      POST_KERNEL,
//                      80,
//                      &api_led);
