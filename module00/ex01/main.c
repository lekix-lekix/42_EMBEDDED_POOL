/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 11:49:24 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/04 14:03:47 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

int main (void)
{
    DDRB = 0b00000001; // DDRx : controls i/o of 8 pins of the microcontroller (B: pins 0-7)
                        // set input or output to the corresponding bit
    
    PORTB |= (1 << PB0); // turns on/off the bit controlling the LED
    
    while (1) {} // the microcontroller needs to stay active, even for doing nothing
}