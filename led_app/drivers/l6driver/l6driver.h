#ifndef L6DRIVER_H_
#define L6DRIVER_H_

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Custom API vtable ---------- */

typedef int (*l6driver_on_t)(const struct device *dev);
typedef int (*l6driver_off_t)(const struct device *dev);
typedef int (*l6driver_get_toggle_count_t)(const struct device *dev,
                                           uint32_t *count);

struct l6driver_api {
    l6driver_on_t               on;
    l6driver_off_t              off;
    l6driver_get_toggle_count_t get_toggle_count;
};

/* ---------- Inline wrappers (called from application) ---------- */

/** Turn the LED on and increment the toggle counter. */
static inline int l6driver_on(const struct device *dev)
{
    const struct l6driver_api *api = (const struct l6driver_api *)dev->api;
    return api->on(dev);
}

/** Turn the LED off and increment the toggle counter. */
static inline int l6driver_off(const struct device *dev)
{
    const struct l6driver_api *api = (const struct l6driver_api *)dev->api;
    return api->off(dev);
}

/**
 * Read how many times the LED has been toggled since boot.
 *
 * @param dev   LED driver device handle.
 * @param count Output: current toggle count stored in the dynamic struct.
 * @return 0 on success.
 */
static inline int l6driver_get_toggle_count(const struct device *dev,
                                            uint32_t *count)
{
    const struct l6driver_api *api = (const struct l6driver_api *)dev->api;
    return api->get_toggle_count(dev, count);
}

#ifdef __cplusplus
}
#endif

#endif /* L6DRIVER_H_ */
