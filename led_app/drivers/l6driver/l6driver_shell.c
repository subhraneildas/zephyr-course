/*-------------------------------------Homework l7-task2-----------------------------------*/

#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <stdlib.h>
#include "l6driver.h"
/** To set a range on the parameter controlled by the custom API
 * we define the end limits and use a simple range check within the shell sub-
 * commands set_blink to check for an out of bounds value for the parameter.
*/
#define BLINK_MS_MIN 50U
#define BLINK_MS_MAX 5000U

static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0)); //reference to dts node

/**Command handlers for fetch, get, setting blink speed and getting blink speed from channel
 * cmd_channel_fetch_handler -> sensor_sample_fetch functionality to turn LED on
 * cmd_channel_get_handler -> sensor_channel_get functionality to turn LED off
 * cmd_info_handler -> displays information about the sensor device and its state
 * --------------------New functions----------------------------------------------------------
 * cmd_blink_set -> Used to set the blinking speed of the on-board LED within a legal range
 *                  and with argument count check
 * cmd_blink_get -> Used to obtain the blinking speed of the on-board LED during run-time
 * 
 * ---------------------NOTE: New functions for custom API follow this structure---------------
 * * sensor set <ms>
 * * SHELL_CMD_ARG mandatory_args = 2:
 *   argv[0] = "set"   (the subcommand token, counts as 1)
 *   argv[1] = value   (the integer argument, counts as 2)
 * The shell rejects the call before reaching this handler if fewer than 2 tokens are present.
 *  
 */
static int cmd_channel_fetch_handler(const struct shell *sh, int argc, char** argv ){
    if(!device_is_ready(dev)){
        shell_error(sh, "Sensor device not ready yet!");
        return -ENODEV;
    }
    int ret = sensor_sample_fetch(dev);
    if(ret){
        shell_error(sh,"Sensor fetch failed!!! Error:%d", ret);
        return ret;
    }
    shell_print(sh, "sensor fetch OK! LED ON");
    return 0;
}

static int cmd_channel_get_handler(const struct shell *sh, int argc, char** argv){
    if(!device_is_ready(dev)){
        shell_error(sh, "Sensor get channel not ready yet!");
        return -ENODEV;
    }
    struct sensor_value val = {0};
    int ret = sensor_channel_get(dev,SENSOR_CHAN_ALL, &val);
    if(ret){
        shell_error(sh,"Sensor get failed!!! Error:%d", ret);
        return ret;
    }
    shell_print(sh, "sensor get OK! LED OFF");
    return 0;
}
/* sensor info — prints device name and ready state */
static int cmd_info_handler(const struct shell *sh, int argc, char** argv){
    shell_info(sh, "Sensor info:\t");
    shell_info(sh, "  name  :%s\t", dev->name);
    shell_info(sh, "  ready : %s\t", device_is_ready(dev) ? "yes" : "no");
    return 0;
}

/*Setting blink rate shell command handler*/
static int cmd_blink_set(const struct shell *sh, size_t argc, char** argv){
    if(!device_is_ready(dev)){
        shell_error(sh,"Device not ready yet!");
        return -ENODEV;
    }
    char *end;
    unsigned long ms= strtoul(argv[1], &end, 10);
    //Null check for the parsed string to unsigned long value
    if(*end!='\0'){
        shell_error(sh,"%s is not a valid integer. Check input again!", argv[1]);
        return -EINVAL;
    }
    //range checking for the blink rate value entered by user
    if(ms<BLINK_MS_MIN || ms>BLINK_MS_MAX){
        shell_error(sh,"Entered value %lu ms for blink rate is out of legal bounds. Allowed range : %u to %u ms.",ms,BLINK_MS_MIN,BLINK_MS_MAX);
        return -ERANGE;
    }
    l6driver_set_blink_speed(dev, (uint32_t)ms); //type casting done to unsigned 32 bit value to match our function signature in l6driver.h file
    shell_print(sh,"Blinking rate for LED is successfully set to %lu ms.", ms);
    return 0;
}

/*Getting blink rate shell command handler*/
static int cmd_blink_get(const struct shell *sh, size_t argc, char** argv){
     if (!device_is_ready(dev)) {
        shell_error(sh, "Device not yet ready!");
        return -ENODEV;
    }
    shell_print(sh, "Current blink period of LED is: %u ms",
                l6driver_get_blink_speed(dev));
    return 0;
}
//Merged subcommand table 

SHELL_STATIC_SUBCMD_SET_CREATE(l6driver_subsensor,
    SHELL_CMD_ARG(fetch, NULL, "Fetching the channel for our simple LED driver", cmd_channel_fetch_handler, 1, 0), //IMP - counting starts without the parent command name
    SHELL_CMD_ARG(get,NULL, "Getting the channel from our simple LED driver ", cmd_channel_get_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Print device info and state", cmd_info_handler, 1,0),
    SHELL_CMD_ARG(set, NULL, "Sets the blinking speed of the on-board LED between 50-5000ms", cmd_blink_set, 2, 0), /* mandatory_args=2: "set" token + one integer argument */
    SHELL_CMD_ARG(get, NULL, "Tells the user the current blinking speed of the on-board LED", cmd_blink_get, 1, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(sensor, &l6driver_subsensor, "sample set of commands for L6 driver", NULL);