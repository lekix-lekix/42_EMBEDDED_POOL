/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:06:54 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/10 16:05:25 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect)
{
    cli();
    PORTB ^= (1 << PB0);
    sei();
}

int main ()
{
    DDRB = (1 << PB0);
    EICRA = (1 << ISC01 | 1 << ISC00);
    EIMSK = (1 << INT0);
    sei();
    while (1) {}
}