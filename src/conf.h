#ifndef _CONF_H
#define _CONF_H

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS_2 1000
#define SLEEP_TIME_MS_1 100
#define EXT_LED_PIN 31

/* Device tree node identifier for GPIO0 */
#define GP DT_NODELABEL(gpio0)
#if DT_NODE_HAS_STATUS(GP, okay)
const struct device *dev_gpio0;
#else
#error "GPIO0 NODE ERROR"
#endif

/* Device tree node identifier for UART0 */
#define UART0 DT_NODELABEL(uart0)
#if DT_NODE_HAS_STATUS(UART0, okay)
const struct device *dev_uart0;
const struct uart_config uart_cfg = {
    .baudrate = 9600,
    .parity = UART_CFG_PARITY_NONE,
    .stop_bits = UART_CFG_STOP_BITS_1,
    .data_bits = UART_CFG_DATA_BITS_8,
    .flow_ctrl = UART_CFG_FLOW_CTRL_NONE};
#else
#error "UART0 NODE ERROR"
#endif

/* Device tree node identifier for I2C0 */
#define I2CX DT_NODELABEL(i2c0)
#if DT_NODE_HAS_STATUS(I2CX, okay)
const struct device *dev_i2c0;
#else
#error "I2C NODE ERROR"
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED2_NODE DT_ALIAS(led2)

#if DT_NODE_HAS_STATUS(LED2_NODE, okay)
#define LED0 DT_GPIO_LABEL(LED2_NODE, gpios)
#define PIN DT_GPIO_PIN(LED2_NODE, gpios)
#define FLAGS DT_GPIO_FLAGS(LED2_NODE, gpios)
const struct device *dev_brd_led2;
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0 ""
#define PIN 0
#define FLAGS 0
#endif

int init_led_pin(void);

int init_i2c(void);

int init_gpio(void);

void setup(void);

void SSD1306_Hello(void);

int init_uart(void);

static inline void write_uart_string(const char *str, const struct device *dev_uart);

/* Threads - Defined at compile time, using K_THREAD_DEFINE  */
void thread_blink_ext_led(void *unused1, void *unused2, void *unused3);

void thread_blink_brd_led(void *unused1, void *unused2, void *unused3);

#endif
