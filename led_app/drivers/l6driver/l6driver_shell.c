/*-------------------------------------Homework l7-task1-----------------------------------*/

#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>

static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0));

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
    shell_print(sh, "sensor fetch OK — LED ON");
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
    shell_print(sh, "sensor get works! LED OFF");
    return 0;
}
/* sensor info — prints device name and ready state */
static int cmd_info_handler(const struct shell *sh, int argc, char** argv){
    shell_info(sh, "Sensor info:\t");
    shell_info(sh, "  name  :%s\t", dev->name);
    shell_info(sh, "  ready : %s\t", device_is_ready(dev) ? "yes" : "no");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(l6driver_subsensor,
    SHELL_CMD_ARG(fetch, NULL, "Fetching the channel for our simple LED driver", cmd_channel_fetch_handler, 1, 0), //IMP - counting starts without the parent command name
    SHELL_CMD_ARG(get,NULL, "Getting the channel from our simple LED driver ", cmd_channel_get_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "Print device info and state", cmd_info_handler, 1,0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(sensor, &l6driver_subsensor, "sample set of commands for L6 driver", NULL);