/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:54:34 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/09 15:17:27 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
    static int rgb = 1;

    if (rgb == 3)
        rgb = 0;
    if (rgb == 0)
        PORTD = (1 << PD5);
    else if (rgb == 1)
        PORTD = (1 << PD6);
    else
        PORTD = (1 << PD3);
    rgb++;
}

void init_timer()
{
    OCR1A = 15625; // (CPU freq / prescaler) * freq desired -> (16mhz / 1024) * 2 = 31250
    TIMSK1 = (1 << OCIE1A); // enabling an interrupt on when an Output Compare A Match happens
    TCCR1B = (1 << WGM12 | 1 << CS12 | 1 << CS10); // toggling CTC mode with 1024 prescaler
}

int main ()
{
    sei();
    DDRD = (1 << PD3 | 1 << PD5 | 1 << PD6);
    PORTD = (1 << PD5);
    init_timer();
    while (1) {}
}
