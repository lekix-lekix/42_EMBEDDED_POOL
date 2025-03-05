/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 20:07:02 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/05 20:40:47 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	check_PIND(int button)
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

int calc_duty_cycle(int percent)
{
    return ((31250 * percent) / 100);
}

int	main(void)
{
	DDRB = 2;
	ICR1 = 31250;
    int duty_cycle = 10;
	OCR1A = calc_duty_cycle(duty_cycle); // setting TOP value (see calculation at bottom)
	TCCR1A = (1 << COM1A1) | (1 << WGM11); // Toggle OC1A/OC1B on Compare Match. (OC1A being the D2 led on the board)
	TCCR1B = (1 << WGM13 | 1 << WGM12 | 1 << CS12); // toggling fast PWM mode
    while (1)
    {
        if (check_PIND(PD2))
        {
            duty_cycle += 10;
            OCR1A = calc_duty_cycle(duty_cycle);
            while ((PIND & (1 << PD2)) == 0) {}
        }
        else if (check_PIND(PD4))
        {
            duty_cycle -= 10;
            OCR1A = calc_duty_cycle(duty_cycle);
            while ((PIND & (1 << PD4)) == 0) {}
        }
        
    }
}