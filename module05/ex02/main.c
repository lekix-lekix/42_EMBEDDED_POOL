/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:07:29 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/12 20:47:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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

void uart_printsr(char *str)
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
    ADMUX = (1 << REFS0); // setting reference voltage, here AVCC
    // ADMUX |= (1 << ADLAR); // 8 bits conversion
    ADCSRA = (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // prescaler 128 (see calculations)
    ADCSRA |= (1 << ADEN); // starting the ADC
    uartinit();
    while (1)
    {
        _delay_ms(20);
        ADMUX &= 0xF8;              // last 3 bits to zero = reading from ADC0 (le potentiometre)
        ADCSRA |= (1 << ADSC);      // saying "i want a conversion"
        while (ADCSRA & (1 << ADSC)) {}  // once it's off : "here's your conversion bro"
        ft_putnbr(ADCL | ADCH << 8); // reading low register FIRST (!) and then adding the high register on top of it
        uart_printsr(", ");
        ADMUX &= 0xF8;              // reset last bits
        ADMUX |= 1;                 // last bit to 1 = reading from ADC1
        ADCSRA |= (1 << ADSC);      // saying "i want a conversion"
        while (ADCSRA & (1 << ADSC)) {}  // once it's off : "here's your conversion bro"
        ft_putnbr(ADCL | ADCH << 8);
        uart_printsr(", ");
        ADMUX &= 0xF8;              // reset last bits
        ADMUX |= 2;                 // reading from ADC
        ADCSRA |= (1 << ADSC);      // saying "i want a conversion"
        while (ADCSRA & (1 << ADSC)) {}  // once it's off : "here's your conversion bro"
        ft_putnbr(ADCL | ADCH << 8);
        uart_printsr("\r\n");
    }
}