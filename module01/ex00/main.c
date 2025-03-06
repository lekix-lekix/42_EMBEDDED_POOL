/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:10:38 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/06 10:22:19 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main ()
{
    DDRB = 2; // initializing D2
    PORTB = 2;
    long int i = 0;
    while (1)
    {
        i++;
        if (i == 235294) // 16 000 000 (cpu freq) / ~34s(t) / 2 (1 hz per whole led cycle)
        {
            PORTB ^= 2; // flipping D2 bit
            i = 0;
        }
    }
}
