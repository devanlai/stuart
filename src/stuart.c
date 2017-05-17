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

#include <stdio.h>
#include <string.h>

#include <libopencm3/stm32/usart.h>

#include "config.h"
#include "target.h"
#include "payload.h"

#ifndef BAUDRATE
#define BAUDRATE 115200
#endif

#ifndef NUM_TEST_BYTES
#define NUM_TEST_BYTES (1024*1024)
#endif

int main(void) {
    target_clock_setup();
    target_gpio_setup();
    target_set_led(false);

    usart_set_baudrate(TEST_USART, BAUDRATE);
    usart_set_databits(TEST_USART, 8);
    usart_set_parity(TEST_USART, USART_PARITY_NONE);
    usart_set_stopbits(TEST_USART, USART_STOPBITS_1);

    usart_set_mode(TEST_USART, USART_MODE_TX);
    usart_set_flow_control(TEST_USART, USART_FLOWCONTROL_NONE);

    usart_enable(TEST_USART);

    while (1) {
        if (target_get_button()) {
            target_set_led(true);
            size_t bytes_sent = 0;
            while (bytes_sent < NUM_TEST_BYTES) {
                const uint8_t* data = _binary_payload_txt_start;
                while (bytes_sent < NUM_TEST_BYTES && data != _binary_payload_txt_end) {
                    usart_send_blocking(TEST_USART, *data++);
                    bytes_sent++;
                }
            }
            target_set_led(false);
        }
    }

    return 0;
}
