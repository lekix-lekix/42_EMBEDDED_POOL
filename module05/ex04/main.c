/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:20:58 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/13 14:48:16 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0B = r;              // timer 0 and 1 duty cycles (LED dimming)
    OCR0A = g;              // are being controlled by the value of TNCT1 (so by timer2)
    OCR2B = b;
}

void wheel(uint8_t pos) 
{
    pos = 255 - pos;
    if (pos < 85) 
        set_rgb(255 - pos * 3, 0, pos * 3);
    else if (pos < 170) 
    {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    }
    else
    {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

void set_leds(uint8_t value)
{
    if (value == 0)
        PORTB = 0;
    if (value > 63)
        PORTB = (1 << PB0);
    if (value > 128)
        PORTB = (1 << PB0 | 1 << PB1);
    if (value > 192)
        PORTB = (1 << PB0 | 1 << PB1 | 1 << PB2);
    if (value == 255)
        PORTB = (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4);
}

void init_rgb()
{
    DDRD = (1 << PD6 | 1 << PD5 | 1 << PD3);      // activating leds
    TCCR0A = (1 << WGM00 | 1 << WGM01);           // timer0 set to fast PWM mode 3 
    TCCR0B = (1 << CS00 | 1 << CS01);             // with a 64 prescaler

    TCCR0A |= (1 << COM0A1 | 1 << COM0B1);        // OC0A (PD6 (green)) and OC0B (PD5 (red)) cleared on compare match
    
    TCCR2A = (1 << WGM20 | 1 << WGM21);           // timer2 set to fast PWM mode 3
    TCCR2B = (1 << CS20 | 1 << CS21);             // with a 64 prescaler

    TCCR2A |= (1 << COM2B1);                      // OC2B (PD3 (blue)) cleared on compare match
}

void init_sensor()
{
    ADMUX = (1 << REFS0); // setting reference voltage as AVCC
    ADMUX |= (1 << ADLAR); // 8 bits conversion
    // MUX bits to 0 set by default == le potentiometre activated
    ADCSRA = (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // setting prescaler to 128
    ADCSRA |= (1 << ADEN); // starting the ADC
}

int main ()
{
    init_sensor();
    init_rgb();
    DDRB = (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4);
    while (1)
    {
        ADCSRA |= (1 << ADSC); // asking for a conversion
        while (1 & (1 << ADSC)) {} // wait for it..
        wheel(ADCH); // done ! lets read it
        set_leds(ADCH);
    }
}