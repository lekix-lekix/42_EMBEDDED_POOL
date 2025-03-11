/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:06:54 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 14:35:27 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile int flag = 0;

ISR(INT0_vect)
{
	if (!flag)
	{
		EIMSK = 0;
		flag = 1;
    }
}

int	main(void)
{
	DDRB = (1 << PB0);
	EICRA = (1 << ISC01);
	EIMSK = (1 << INT0);
	sei();
	while (1)
	{
		if (flag == 1)
		{
			_delay_ms(100);
			PORTB ^= (1 << PB0);
			_delay_ms(100);
	        EIMSK = (1 << INT0);
			flag = 0;
		}
	}
}