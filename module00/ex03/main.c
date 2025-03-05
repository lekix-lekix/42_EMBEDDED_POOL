/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 11:49:24 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/04 17:46:39 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

int main ()
{
    DDRB = 0b00000001; // DDRx : controls i/o of 8 pins of the microcontroller (B: pins 0-7)
                        // set input or output to the corresponding bit (here : pin 0)
    while (1)
    {
        if ((PIND & (1 << PD2)) == 0)   // PINx : checking state of said pin (0 == switch pressed)
        {
            _delay_ms(50);  // avoid rebound effect : wait for the switch signal to be stable
            if ((PIND & (1 << PD2)) == 0) // and checking again
            {
                PORTB ^= (1 << PB0);    // flipping the led bit 
                while ((PIND & (1 << PD2)) == 0) {} // blocking execution while button is pressed (avoid blinking led)
            }
        }
    }
}