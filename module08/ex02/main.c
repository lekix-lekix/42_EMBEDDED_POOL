/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:24:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 00:17:47 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define DDR_SPI    DDRB
# define MOSI       PB3
# define SCK        PB5
# define SS         PB2

void SPI_send(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {}
}

void SPI_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
    SPI_send(0xFF | (brightness & 0x1F));
    SPI_send(b);
    SPI_send(g);
    SPI_send(r);
}

void init_SPI(void)
{
    DDR_SPI = (1 << MOSI | 1 << SCK | 1 << SS);                 // MOSI = Master Out Slave In
                                                                // SCK = Serial Clock
                                                                // SS = Slave Select
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);              // SPE = SPI Enable
                                                                // MSTR = Master Enabled
                                                                // SPRO = SPI clock Rate
    DDRB |= (1 << SS);
}

void set_one_led(int led_idx)
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
    {
        if (i == led_idx)
            SPI_rgb(0, 255, 0, 31);
        else
            SPI_rgb(0, 0, 0, 31);
    }
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}

int main ()
{
    init_SPI();
    while (1)
    {
        for (int i = 0; i < 3; i++)
        {
            set_one_led(i);
            _delay_ms(250);
        }
    }
}
