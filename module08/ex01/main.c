/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:24:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 22:35:56 by kipouliq         ###   ########.fr       */
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
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    SPI_send(0xFF | (brightness & 0x1F));
    SPI_send(b);
    SPI_send(g);
    SPI_send(r);
    SPI_send(0xFF);
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

void set_rgb()
{
    static int i = 1;
    int tab[7][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    SPI_rgb(tab[i][0] * 255, tab[i][1] * 255, tab[i][2] * 255, 31);
    if (i == 6)
        i = 0;
    else
        i++;
}

int main ()
{
    init_SPI();
    while (1)
    {
        set_rgb();
        _delay_ms(1000);
    }
}