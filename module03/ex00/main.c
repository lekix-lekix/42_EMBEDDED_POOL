/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 11:54:34 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 10:47:27 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect)
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
    OCR1A = 62499; // (CPU freq / prescaler) * freq desired -> (16mhz / 256) * 1 - 1 = 62499
    TIMSK1 = (1 << OCIE1A); // enabling an interrupt on when an Output Compare A Match happens
    TCCR1B = (1 << WGM12 | 1 << CS12); // toggling CTC mode with 256 prescaler
}

int main ()
{
    sei();
    DDRD = (1 << PD3 | 1 << PD5 | 1 << PD6);
    PORTD = (1 << PD5);
    init_timer();
    while (1) {}
}

// prescaler : (cpu freq / overflow value) * freq desired (hz) - 1 -> 16000000 / 65636 * 1 ~= 244