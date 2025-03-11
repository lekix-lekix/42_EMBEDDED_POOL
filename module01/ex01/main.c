/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:30:43 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 10:38:00 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main ()
{
    DDRB = 2; // setting D2 as output
    OCR1A = 31249; // setting TOP value (see calculation at bottom)
    TCCR1A = (1 << COM1A0); // Toggle OC1A/OC1B on compare match mode and clear. (OC1A being the D2 led on the board)
    TCCR1B = (1 << WGM12); // Activating CTC mode to trigger an event and reset phase when TOP value is reached
    TCCR1B |= (1 << CS12); // Selecting 256 prescaler (prescaler CS10 => CS12)
    while (1) {}
}

// We want 1 overflow per s (1 hz : a led cycle should last 1s). Timer at 16bit, so overflow value is 65536
// Default timer overflow : 65536 / 16 000 000 (cpu freq) = 4.1 ms.
// Prescaler formula :
// (overflow value * prescaler) / cpu freq = freq desired
// so : prescaler = cpu freq * freq desired (in hz) / overflow value;
// in this case : prescaler = 16 000 000 * 1 / 65636 ~= 244;
// TOP value = cpu freq / prescaler
// here = 16 000 000 / 256 - 1 = 62.499 (/2 to get 0.5 hz) 