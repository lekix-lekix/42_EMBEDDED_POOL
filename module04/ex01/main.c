/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:36:57 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 19:46:17 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

ISR(TIMER0_COMPA_vect)
{
    static int inc = 2;
    
    if (OCR1A == 254)
        inc = -2;
    else if (OCR1A == 0)
        inc = 2;
    OCR1A += inc;
}

int main ()
{
    DDRB = (1 << PB1);
    TCCR1A = (1 << COM1A1 | 1 << WGM10);
    TCCR1B = (1 << CS12);
    ICR1 = 255;
    OCR1A = 0;
    
    TCCR0B = (1 << CS02);
    TCCR0A = (1 << WGM00);
    TIMSK0 = (1 << OCIE0A);
    
    sei();
    while (1) {}
}

