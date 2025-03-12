/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:36:57 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/12 15:42:07 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

ISR(TIMER0_COMPA_vect)
{
    static int inc = 2;
    
    if (OCR1A == 254)                           // OCR1A being the duty cycle of timer 1
        inc = -2;                               // modifying OCR1A increases or decreases the led brightness
    else if (OCR1A == 0)
        inc = 2;
    OCR1A += inc;
}

int main ()
{
    DDRB = (1 << PB1);
    TCCR1A = (1 << COM1A1 | 1 << WGM10);        // setting timer 1 to PWM mode to control the led dimming
    TCCR1B = (1 << CS10);
    
    TCCR0A = (1 << WGM00 | 1 << WGM01);        // timer 0 to fast PWM mode
    TCCR0B = (1 << CS02);                      // prescaler 256 (256 overflow / s)
    TIMSK0 = (1 << OCIE0A);                    // triggers an interrupt every overflow
    
    sei();
    while (1) {}
}

