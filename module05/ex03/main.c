/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:47:34 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/13 18:32:59 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TEMP_RATIO 0.071022727

void uartinit(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // "Transmit Enabled" / "Receive Enabled" / "Receive Complete Interrupt Enable"
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {} // checking that no transmission is ongoing, by checking the "Data Register Empty" flag
    UDR0 = c; // loading the data buffer with our char
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))) {} // checking that dats transmission is complete by checking the "Receive Complete"
    return (UDR0); // RX and TX share the same buffer register
}

void uart_printstr(char *str)
{
    for (int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

void	ft_putchar(char c)
{
    uart_tx(c + '0');
}

void	ft_putnbr(int nb)
{
	long int	number;

	number = nb;
	if (number < 0)
	{
		number = -number;
		ft_putchar('-');
	}
	if (number >= 0 && number <= 9)
		ft_putchar(number);
	if (number > 9)
	{
		ft_putnbr(number / 10);
		ft_putchar(number % 10);
	}
}

int main ()
{
    ADMUX = 0b00001000;     // activating the temperature sensor
    ADMUX |= (1 << REFS0 | 1 << REFS1); // setting reference voltage, here 1.1v
    ADCSRA = (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // prescaler 128 (see calculations)
    ADCSRA |= (1 << ADEN); // starting the ADC
    uartinit();
    while (1)
    {
        _delay_ms(20);
        ADCSRA |= (1 << ADSC);      // saying "i want a conversion"
        while (ADCSRA & (1 << ADSC)) {}  // once it's off : "here's your value mate"
        ft_putnbr((ADCL | ADCH << 8) * TEMP_RATIO); // temp ratio = expected temp / voltage
        uart_printstr("\r\n");
    }
}

// Table 23-2. Sensor Output Code versus Temperature (Typical Values)
// Temperature/°C –40°C +25°C +125°C
//               0x010D 0x0160 0x01E0
//                       352
// 25 degree / 352 mv = 0.071022727