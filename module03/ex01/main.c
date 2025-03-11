/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 15:17:58 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 10:47:54 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
    static int i = 1;
    int tab[7][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    PORTD = (tab[i][0] << PD5 | tab[i][1] << PD6 | tab[i][2] << PD3);
    if (i == 6)
        i = 0;
    else
        i++;
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
