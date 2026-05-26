#ifndef L6DRIVER_H_
#define L6DRIVER_H_

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>   // needed for struct sensor_driver_api
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------- Custom API function pointer types ----------

typedef int      (*l6driver_set_blink_speed_t)(const struct device *dev,
                                               uint32_t period_ms);
typedef uint32_t (*l6driver_get_blink_speed_t)(const struct device *dev);

/*---------- Combined API vtable ----------
 *
 * struct sensor_driver_api MUST be the first member so that the sensor
 * subsystem's internal cast  (sensor_driver_api *)dev->api  still lands
 * on the right function pointers at offset 0.
 * Our custom pointers sit after it and are reachable by
 * casting dev->api to struct l6driver_api.
 */
struct l6driver_api {
    struct sensor_driver_api sensor;        /* placed first since sensor subsystem looks here first while building*/
    l6driver_set_blink_speed_t set_blink_speed;
    l6driver_get_blink_speed_t get_blink_speed;
};

//---------- Inline wrappers (to be called from application main) ---------- 

//Sets blinking speed of LED and stored in the driver's 
//dynamic data struct at runtime.
static inline int l6driver_set_blink_speed(const struct device *dev,
                                           uint32_t period_ms)
{
    const struct l6driver_api *api = (const struct l6driver_api *)dev->api;
    return api->set_blink_speed(dev, period_ms);
}

//Reads and returns the current blink period from
// the driver's dynamic data struct, ie., const struct device *dev
static inline uint32_t l6driver_get_blink_speed(const struct device *dev)
{
    const struct l6driver_api *api = (const struct l6driver_api *)dev->api;
    return api->get_blink_speed(dev);
}

#ifdef __cplusplus
}
#endif

#endif /* L6DRIVER_H_ */
