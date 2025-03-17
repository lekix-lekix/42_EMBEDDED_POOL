/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:38:27 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 00:10:41 by kipouliq         ###   ########.fr       */
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

int get_conversion(void)
{
    ADCSRA |= (1 << ADSC);      // saying "i want a conversion"
    while (ADCSRA & (1 << ADSC)) {}  // once it's off : "here's your conversion bro"
    return (ADCH); // reading into high register since its an 8 bit conversion
}

void init_ADC(void)
{
    ADMUX = (1 << REFS0); // setting reference voltage, here AVCC
    ADMUX |= (1 << ADLAR); // 8 bits conversion
    ADMUX &= 0xF0; // setting the last four MUX bits to 0, aka using ADC0 (useless in this case)
    ADCSRA = (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // prescaler 128 (see calculations)
    ADCSRA |= (1 << ADEN); // starting the ADC
}

void set_leds(int nb)
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
    {
        if (i < nb)
            SPI_rgb(0, 255, 0, 31);
        else
            SPI_rgb(0, 0, 0, 31);
    }
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}

int main ()
{
    init_ADC();
    init_SPI();
    int nb = 0;
    while (1)
    {
        _delay_ms(20);
        nb = get_conversion();
        if (nb < 76)
            set_leds(0);
        else if (nb >= 76 && nb < 153)
            set_leds(1);
        else if (nb >= 153 && nb < 255)
            set_leds(2);
        else if (nb == 255)
            set_leds(3);
    }
}