/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:49:01 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 12:16:19 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define DDR_SPI    DDRB
# define MOSI       PB3
# define SCK        PB5
# define SS         PB2

int led = 0;
int rgb_i = 0;
uint8_t colors[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0 ,0}};

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

void set_one_led(int idx, uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
    {
        if (i == idx)
            SPI_rgb(r, g, b, 31);
        else
            SPI_rgb(0, 0, 0, 31);
    }
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
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

int	check_PIND(int button)
{
	if ((PIND & (1 << button)) == 0) // checking if button is pressed
	{
		_delay_ms(50);
		if ((PIND & (1 << button)) == 0) // confirmation, boucing effect
			return (1);
		return (0);
	}
	return (0);
}

void spi_conversion()
{
    
}

void spi_set_all_leds()
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
    {
        SPI_rgb(colors[i][0], colors[i][1], colors[i][2], 31);
    }
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}

int main ()
{
    init_ADC();
    init_SPI();
    
    while (1)
    {
        if (check_PIND(PD2))
        {
            rgb_i++;
            if (rgb_i == 3)
                rgb_i = 0;
            spi_set_all_leds();
            while (check_PIND(PD2)) {}
        }
        if (check_PIND(PD4))
        {
            led++;
            if (led == 3)
                led = 0;
            rgb_i = 0;
            spi_set_all_leds();
            while (check_PIND(PD4)) {}
        }
        colors[led][rgb_i] = get_conversion();
        spi_set_all_leds();
    }
}