/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 15:52:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/09 19:28:28 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init_rgb()
{
    DDRD = (1 << PD6 | 1 << PD5 | 1 << PD3);      // activating leds
    TCCR0A = (1 << WGM00 | 1 << WGM01);           // activating timer0 to control brightness 
    TCCR0B = (1 << CS00 | 1 << CS01);             

    TCCR0A |= (1 << COM0A1 | 1 << COM0B1);
    
    TCCR2A = (1 << COM2B1 | 1 << WGM20 | 1 << WGM21);
    TCCR2B = (1 << CS20 | 1 << CS21);
    
    TCCR1B = (1 << WGM13 | 1 << CS12 | 1 << CS10);
    ICR1 = 65535;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0A = r;
    OCR0B = g;
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