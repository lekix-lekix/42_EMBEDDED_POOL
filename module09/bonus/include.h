/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/18 21:11:18 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

#define SEG_0 0b00111111  // 0
#define SEG_1 0b00000110  // 1
#define SEG_2 0b01011011  // 2
#define SEG_3 0b01001111  // 3
#define SEG_4 0b01100110  // 4
#define SEG_5 0b01101101  // 5
#define SEG_6 0b01111101  // 6
#define SEG_7 0b00000111  // 7
#define SEG_8 0b01111111  // 8
#define SEG_9 0b01101111  // 9

#define SEG_A  0b01110111  // A
#define SEG_B  0b01111100  // B
#define SEG_C  0b00111001  // C
#define SEG_D  0b01011110  // D
#define SEG_E  0b01111001  // E
#define SEG_F  0b01110001  // F
#define SEG_G  0b00111101  // G
#define SEG_H  0b01110100  // H
#define SEG_I  0b00000111  // I
#define SEG_J  0b00011110  // J
#define SEG_L  0b00111000  // L
#define SEG_M  0b01110101  // M
#define SEG_N  0b01010100  // N
#define SEG_O  0b00111111  // O
#define SEG_P  0b01110011  // P
#define SEG_Q  0b01100111  // Q (ressemble à A)
#define SEG_R  0b01010000  // R
#define SEG_S  0b01101101  // S
#define SEG_T  0b01111000  // T
#define SEG_U  0b00111110  // U
#define SEG_V  0b00111110  // V (comme un U sans le segment du haut)
#define SEG_Y  0b01101110  // Y
#define SEG_Z  0b01011011  // Z 

#define SCREEN_0 0b11101111
#define SCREEN_1 0b11011111
#define SCREEN_2 0b10111111
#define SCREEN_3 0b01111111

# define INPUT_REG_0 0x00
# define OUTPUT_REG_0 0x02
# define OUTPUT_REG_1 0x03
# define CONFIG_REG_0 0x06
# define CONFIG_REG_1 0x07

# define DDR_SPI    DDRB
# define MOSI       PB3
# define SCK        PB5
# define SS         PB2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void i2c_init(void);
void i2c_write(unsigned char data);
void i2c_read(void);
void i2c_start(void);
void i2c_stop(void);

void init_SPI(void);

void set_one_led(int led_idx, uint32_t color, int brightness);
void set_all_leds(uint8_t r, uint8_t g, uint8_t b, int brightness);

uint8_t char_to_seg(char c);

#endif