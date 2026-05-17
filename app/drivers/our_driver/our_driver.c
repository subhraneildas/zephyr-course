#include <zephyr/drivers/sensor.h>
#include<zephyr/logging/log.h>
#define DT_DRV_COMPAT our_driver
//LOG_MODULE_DECLARE(our_driver,LOG_LEVEL_INF);
LOG_MODULE_REGISTER(our_driver,LOG_LEVEL_INF); //Since we need to register a new driver we need LOG_MODULE_REGISTER macro
// but if we need more than 1 drivers in the same module then we use LOG_MODULE_DECLARE for the subsequent drivers. REGISTER THE FIRST,
// DECLARE AS MANY TIMES AS NEEDED

 
static int channel_my_impl(const struct device *dev,
				    enum sensor_channel chan,
				    struct sensor_value *val){
    LOG_INF("Hello from this Channel Get, channel %d", chan);
    return 0;  
    }


static DEVICE_API(sensor, api_iomico_lecture) = {
    .channel_get = channel_my_impl,
};

static int init(const struct device *dev){
    LOG_INF("Device Initialized!!");
    return 0;
}
DEVICE_DT_INST_DEFINE(0, init,  NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);

