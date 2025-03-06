/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:18:02 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/06 11:58:50 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main ()
{
    DDRB = 2;
    OCR1A = 3124; // setting PWM resolution, aka when to trigger an event
    ICR1 = 62499; // setting TOP value, aka full phase of the PWM
    TCCR1A = (1 << COM1A1 | 1 << WGM11); // Clear OC1A/B on compare match and set it at bottom
                                        // basically sets the led on from 0 to 0CR1A value
    TCCR1B = (1 << WGM13 | 1 << WGM12 | 1 << CS12); // toggling fast PWM mode 14 with 256 prescaler
    while (1) {}
}