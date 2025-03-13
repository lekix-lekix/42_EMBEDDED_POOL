/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:32:10 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/13 18:34:57 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>

#define FREQ_CPU 16000000
#define BITRATE (FREQ_CPU / 100000 - 16) / 2 + 1

void i2c_init(void)
{
    TWBR = 72; // Prescaler already set to 1
    TWCR = (1 << TWEN);
}

void i2c_start(void)
{
    TWCR = (1 << TWSTA | 1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {}
    if ((TWSR & 0xF8) == 0x08)
    {
        DDRB = 1;
        PORTB = 1;
    }
}

// TWBR calculation : f_cpu / scl_freq_desired(100khz or less) - 16 / 2 * prescaler

int main ()
{
    i2c_init();
    i2c_start();
    while (1) {}
}