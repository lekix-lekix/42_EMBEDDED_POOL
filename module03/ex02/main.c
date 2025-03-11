/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 15:52:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 12:48:15 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init_rgb()
{
    DDRD = (1 << PD6 | 1 << PD5 | 1 << PD3);      // activating leds
    TCCR0A = (1 << WGM00 | 1 << WGM01);           // timer0 set to fast PWM mode 3 
    TCCR0B = (1 << CS00 | 1 << CS01);             // with a 64 prescaler

    TCCR0A |= (1 << COM0A1 | 1 << COM0B1);        // OC0A (PD6 (green)) and OC0B (PD5 (red)) cleared on compare match
    
    TCCR2A = (1 << WGM20 | 1 << WGM21);           // timer2 set to fast PWM mode 3
    TCCR2B = (1 << CS20 | 1 << CS21);             // with a 64 prescaler

    TCCR2A |= (1 << COM2B1);                      // OC2B (PD3 (blue)) cleared on compare match
    
    TCCR1B = (1 << WGM13 | 1 << CS12 | 1 << CS10); // Timer1 set to PWM phase and freq correct mode 8 with a 1024 prescaler
    ICR1 = 65535;                                   // = count from bottom to top
}

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

int main ()
{
    init_rgb();
    while (1)
        wheel(TCNT1 >> 8);        
}