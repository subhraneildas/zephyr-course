# Note for building homework L6 task 2
The simple LED driver fetch and get functionality from l6-task1 is extended with two custom api extensions set_blink_speed and get_blink_speed respectively. They are used to increment and print the blinking speed of the blue on-board LED or nano 33 ble board starting from 200ms upto 1000ms and it resets back to 200 upon reaching 1000ms.

# New files - l6driver.h
1. It contains the combined vtable for sensor_driver_api (with higher priority) followed by set/get_blink_speed.
2. Contains inline wrappers for set/get_blink_speed

# Changed files - l6driver.c, main.cpp

    ## l6driver.c

    1. Dynamic struct led_driver_data{uint32_t blink_period_ms;} has the blink_period_ms value used to change the speed of the LED during run time.
    2. Vtable declared as struct l6driver_api: 
        static const struct l6driver_api api_led = {
            .sensor = {
                .sample_fetch = channel_fetch_my_impl,
                .channel_get  = channel_get_my_impl,
            },
            .set_blink_speed = set_blink_speed_impl,
            .get_blink_speed = get_blink_speed_impl,
        };

    ## main.cpp

    1. Unchanged API functionality for led on and off
    2. Added l6driver_set_blink_speed() and l6driver_get_blink_speed() function calls


## Manual Zephyr Setup

Follow the following guide:
- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#).

Make sure to select appropriate OS and to perform all steps till
[Build the Blinky Sample](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#build-the-blinky-sample).
