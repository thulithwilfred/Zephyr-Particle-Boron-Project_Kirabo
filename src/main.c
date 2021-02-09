/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/i2c.h>
#include "conf.h"
#include "fonts.h"
#include "ssd1306.h"

/* TODO add thread support. */
#define STACK_SIZE 1024
#define THREAD_PRIORITY 5 /* Lower Numerics has higher priority, -Ve Priorities are cooperitive threads, +Ve Priorities  are Preemtible  */

/* Creating threads at compile time */
K_THREAD_DEFINE(tid_thread_blink_ext_led, STACK_SIZE, thread_blink_ext_led, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(tid_thread_blink_brd_led, STACK_SIZE, thread_blink_brd_led, NULL, NULL, NULL, THREAD_PRIORITY, 0, 0);

void thread_blink_ext_led(void *unused1, void *unused2, void *unused3)
{

	bool led_is_on = true;

	while (1)
	{
		gpio_pin_set(dev_gpio0, EXT_LED_PIN, !(int)led_is_on);
		led_is_on = !led_is_on;
		k_msleep(SLEEP_TIME_MS_1);
	}
}

void thread_blink_brd_led(void *unused1, void *unused2, void *unused3)
{
	bool led_is_on = true;

	while (1)
	{
		gpio_pin_set(dev_brd_led2, PIN, (int)led_is_on);
		led_is_on = !led_is_on;
		k_msleep(SLEEP_TIME_MS_2);
	}
}

void main(void)
{

	/* Initialize Pheripherals */
	setup();
	/* Send message to display */
	SSD1306_Hello();
}

void SSD1306_Hello(void)
{
	SSD1306_GotoXY(10, 10);
	SSD1306_Puts("Good Morning", &Font_11x18, 1);
	SSD1306_GotoXY(10, 30);
	SSD1306_Puts(">debug", &Font_7x10, 1);
	SSD1306_GotoXY(10, 40);
	SSD1306_Puts("0x69A", &Font_7x10, 1);

	SSD1306_UpdateScreen();
}

/* Initialise pins and various sub-systems */
void setup(void)
{
	int r1, r2, r3;
	/* Internal Initialization, init_* functions will return 0 if successful */
	r1 = init_led_pin();
	r2 = init_gpio();
	r3 = init_i2c();

	if (r1 || r2 || r3)
	{
		/* Initialization Failed */
		k_msleep(20000);
	}

	/* External Device Initialization */
	SSD1306_Init();
}

/* Initialize ON_BRD_LED, will return 0 on completion */
int init_led_pin(void)
{
	dev_brd_led2 = device_get_binding(LED0);
	if (!dev_brd_led2)
	{
		/* Unable to retrive device structure */
		return -ENODEV;
	}
	else
	{
		/* Configrue BRD_LED using attained deivice struct as GPIO_OUT */
		return gpio_pin_configure(dev_brd_led2, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	}
}

/* Init I2C, will return 0 on completeion */
int init_i2c(void)
{
	dev_i2c0 = device_get_binding(DT_LABEL(I2CX));
	if (!dev_i2c0)
	{
		/* Unable to retrive device structure */
		return -ENODEV;
	}
	else
	{
		/*Configure I2C using attained device struct, using the following parameters */
		return i2c_configure(dev_i2c0, I2C_SPEED_FAST | I2C_MODE_MASTER);
	}
}

/* Init GPIO_PIN_31, will return 0 on completion */
int init_gpio(void)
{
	dev_gpio0 = device_get_binding(DT_LABEL(GP));
	if (!dev_gpio0)
	{
		/* Unable to retrive device structure */
		return -ENODEV;
	}
	else
	{
		/* Configure GPIO_PIN_31 as GPIO Out for external LED use */
		return gpio_pin_configure(dev_gpio0, EXT_LED_PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	}
}
