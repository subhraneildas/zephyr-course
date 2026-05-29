//----------------------------------Homework L7-task2---------------------------------------------
#include <zephyr/drivers/sensor.h>   // restored — sensor API is back
#include "l6driver.h"
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
//#include<zephyr/device.h>
#define DT_DRV_COMPAT my_l6driver

LOG_MODULE_REGISTER(l6driver, LOG_LEVEL_INF);

/* ---------- Per-device structs ---------- */

// Static (ROM) config — unchanged
struct led_driver_config {
    struct gpio_dt_spec led;
};

// Dynamic (RAM) data — blink_period_ms replaces toggle_count as the custom parameter
struct led_driver_data {
    uint32_t blink_period_ms;   // controlled via custom API; used by application in k_msleep
};

// ---------- Sensor API implementations----------------- 

// sample_fetch : turns LED on  
static int channel_fetch_my_impl(const struct device *dev,
                                 enum sensor_channel chan)
{
    const struct led_driver_config *cfg = dev->config;
    gpio_pin_set_dt(&cfg->led, 1);
   // LOG_INF("Channel %d - LED ON!\n", chan);
    return 0;
}

// channel_get : turns LED off  
static int channel_get_my_impl(const struct device *dev,
                               enum sensor_channel chan,
                               struct sensor_value *val)
{
    const struct led_driver_config *cfg = dev->config;
    gpio_pin_set_dt(&cfg->led, 0);
    //LOG_INF("Channel %d - LED OFF!\n", chan);
    return 0;
}

// ---------- Custom API implementation---------- 

static int set_blink_speed_impl(const struct device *dev, uint32_t period_ms)
{
    struct led_driver_data *data = dev->data;
    data->blink_period_ms = period_ms;
    LOG_INF("LED blinking speed set to %u ms", period_ms);
    return 0;
}

static uint32_t get_blink_speed_impl(const struct device *dev)
{
    const struct led_driver_data *data = dev->data;
    return data->blink_period_ms;
}

/* ---------- Combined API vtable ----------
 *
 * Cannot use  static DEVICE_API(sensor, api_led)  here because that macro
 * produces  const struct sensor_driver_api api_led  — a type with no room
 * for set_blink_speed / get_blink_speed.
 * We declare it directly as struct l6driver_api, which embeds
 * struct sensor_driver_api as its first member (see l6driver.h).
 */
static const struct l6driver_api api_led = {
    .sensor = {
        .sample_fetch = channel_fetch_my_impl,
        .channel_get  = channel_get_my_impl,
    },
    .set_blink_speed = set_blink_speed_impl,
    .get_blink_speed = get_blink_speed_impl,
};

// ---------- Custom driver initialisation ---------- 

static int init(const struct device *dev)
{
    const struct led_driver_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("LED GPIO not ready yet!!!!\n");
        return -ENODEV;
    }
    gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE); //setting GPIO pin as output
    LOG_INF("LED Device Driver Initialized!!\n");
    return 0;
}

/* ---------- Instantiation ----------
 *
 * DEVICE_DT_INST_DEFINE(inst, init, pm, data_ptr, config_ptr, level, prio, api_ptr)
 *
 * data_ptr   → &led_driver_data_##inst  (dynamic: holds blink_period_ms in RAM)
 * config_ptr → &led_driver_config_##inst (static: GPIO spec stored in ROM)
 * blink_period_ms initialised to 500 ms as the default speed
 */
#define DEV_INST(inst)                                                          \
    static const struct led_driver_config led_driver_config_##inst = {         \
        .led = GPIO_DT_SPEC_INST_GET(inst, gpios),                             \
    };                                                                          \
    static struct led_driver_data led_driver_data_##inst = {                   \
        .blink_period_ms = 200, /*Default value for setting blink_speed*/       \ 
    };                                                                          \
    DEVICE_DT_INST_DEFINE(inst, init, NULL,                                     \
                          &led_driver_data_##inst,                              \
                          &led_driver_config_##inst,                            \
                          POST_KERNEL, 80, &api_led);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);

