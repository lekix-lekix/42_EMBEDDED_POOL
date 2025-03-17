/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 11:49:24 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 11:21:30 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

int check_PIND(int button)
{
    if ((PIND & (1 << button)) == 0) // checking if button is pressed
    {
        _delay_ms(50);
        if ((PIND & (1 << button)) == 0) // confirmation, boucing effect
            return (1);
        return (0);
    }
    return (0);
}

void set_leds(int *i, int button)
{
    if (*i > 15)
        *i = 0;                               // PORTB = i : actually lights the correct leds while i < 7
    else if (*i < 0)                          // because D1 D2 D3 are conveniently located at the last 3 bits of PORTB 
        *i = 15;                              // ex : 5 as binary is 0101 so PORTB = 5 -> 0b00000101 -> leds : [0.1.0.1] 
    if (*i > 7)                               // but D4 is the 5th bit from the right, so its needed to manually set it on
        PORTB = 1 << 4 | (*i - 8);            // with 1 << 4 (->0b00010000), OR it with our var starting from 0 again (i - 8)
    else                                      // and start counting from 0 again (i - 8).
        PORTB = *i;                           // ex. for i = 15 : 1 << 4 (0b00010000) | 7 (0b00000111) 
    while ((PIND & (1 << button)) == 0) {}    // result : 0b00010111 -> all leds are on
}

int	main(void)
{
	DDRB = (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4); // initializing the leds as output
    int i = 0;
	while (1)
	{
		if (check_PIND(PD2))  // left switch pressed
        {
            i++;
            set_leds(&i, PD2);
        }
        else if (check_PIND(PD4)) // right switch pressed
        {
            i--;
            set_leds(&i, PD4);
        }
	}
}

