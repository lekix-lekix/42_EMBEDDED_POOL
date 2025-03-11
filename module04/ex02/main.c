/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 19:48:05 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 20:23:04 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

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

void set_leds(int inc)
{
    static int i = 0;
    
    i += inc;
    if (i > 15)
        i = 0;                               // PORTB = i : actually lights the correct leds while i < 7
    else if (i < 0)                          // because D1 D2 D3 are conveniently located at the last 3 bits of PORTB 
        i = 15;                              // ex : 5 as binary is 0101 so PORTB = 5 -> 0b00000101 -> leds : [0.1.0.1] 
    if (i > 7)                               // but D4 is the 5th bit from the right, so its needed to manually set it on
        PORTB = 1 << 4 | (i - 8);            // with 1 << 4 (->0b00010000), OR it with our var starting from 0 again (i - 8)
    else                                      // and start counting from 0 again (i - 8).
        PORTB = i;                           // ex. for i = 15 : 1 << 4 (0b00010000) | 7 (0b00000111)    // result : 0b00010111 -> all leds are on
}

ISR(INT0_vect)
{
    if (check_PIND(PD2))
        set_leds(1);
}

ISR(PCINT2_vect)
{
    if (check_PIND(PD4))
        set_leds(-1);
}

int	main(void)
{
	DDRB = (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4); // initializing the leds as output
    PORTD = (1 << PD2 | 1 << PD4);
    EICRA = (1 << ISC01);
	EIMSK = (1 << INT0);
    PCICR = (1 << PCIE2);
    PCMSK2 = (1 << PCINT20);
    sei();
	while (1){}
}