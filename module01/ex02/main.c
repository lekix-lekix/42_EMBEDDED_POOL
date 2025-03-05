/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:18:02 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/05 20:02:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main ()
{
    DDRB = 2;
    OCR1A = 3124; // setting TOP value (see calculation at bottom)
    ICR1 = 31249;
    TCCR1A = (1 << COM1A1 | 1 << WGM11); // Toggle OC1A/OC1B on Compare Match. (OC1A being the D2 led on the board)
    TCCR1B = (1 << WGM13 | 1 << WGM12 | 1 << CS12); // toggling fast PWM mode
    while (1) {}
}