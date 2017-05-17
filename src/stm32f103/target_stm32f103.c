/*
 * Copyright (c) 2017, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "target.h"
#include "config.h"

/* Set STM32 to 72 MHz. */
void target_clock_setup(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

void target_gpio_setup(void) {
    /* Enable GPIOA, GPIOB, and GPIOC clocks. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Setup LED */
    {
        const uint8_t mode = GPIO_MODE_OUTPUT_10_MHZ;
        const uint8_t conf = (LED_OPEN_DRAIN ? GPIO_CNF_OUTPUT_OPENDRAIN
                                             : GPIO_CNF_OUTPUT_PUSHPULL);
        gpio_set_mode(LED_GPIO_PORT, mode, conf, LED_GPIO_PIN);
    }

    /* Setup button */
    {
        gpio_set_mode(BUTTON_GPIO_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, BUTTON_GPIO_PIN);
        if (BUTTON_ACTIVE_HIGH) {
            gpio_clear(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        } else {
            gpio_set(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        }
    }

    /* Enable UART clock */
    rcc_periph_clock_enable(TEST_USART_CLOCK);

    /* Setup TX pin */
    gpio_set_mode(TEST_USART_GPIO_PORT, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, TEST_USART_GPIO_TX);
}

void target_set_led(bool on) {
    if (on ^ LED_OPEN_DRAIN) {
        gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
    } else {
        gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
    }
}

bool target_get_button(void) {
    if (BUTTON_ACTIVE_HIGH) {
        return gpio_get(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
    } else {
        return !gpio_get(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
    }
}
