/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 11:49:24 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/03 19:03:17 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

int main (void)
{
    DDRB = 0b00000001; // DDRx : set i/o to the corresponding bit of the 8 pins (here: B)
    PORTB |= (1 << PB0); // turns on/off the bit controlling the LED 
}