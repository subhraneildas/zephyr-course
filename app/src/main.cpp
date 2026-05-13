#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//#define BLINK_SLEEP_TIME_MS 1000
//#define APP_HEARTBEAT_PERIOD_MS 500
//-----------------Playing around with devicetree aliases and nodes for RGB LED-----------------------------
/* The devicetree node identifier for the "led0" alias. */
//#define LED_NODE DT_ALIAS(led1)
/*Now we make a DT_NODE_LABEL reference to our overlay references*/
//#define LED_NODE DT_NODELABEL(blue_led)
/*Now we access the green led using the path method although rarely used in production*/
//#define LED_NODE DT_PATH(leds, led6)
/*Now lastly we use DT_ALIAS to access a newly created alias my_red_led in my_own.overlay file*/
//#define LED_NODE DT_ALIAS(my_red_led)
//-----------------------------------------------------------------------------------------------------------
//---------------------homework L4 task1---------------------------------------------------------
#define LED_NODE DT_ALIAS(app_led)
//------------------------------------------------------------------------------------------------
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        //k_msleep(BLINK_SLEEP_TIME_MS);
        //k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
        //---------------------homework L4 task1---------------------------------------------------------
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
