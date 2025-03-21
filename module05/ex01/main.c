/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:48:51 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/13 18:39:29 by kipouliq         ###   ########.fr       */
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

void	ft_putchar_hex(char c)
{
    char hex[] = "0123456789ABCDEF";
    for (int i = 0; hex[i]; i++)
    {
        if (i == c)
            uart_tx(hex[i]);
    }
}

void	ft_putnbr_hex(int nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number <= 16)
		ft_putchar_hex(number);
	if (number > 16)
	{
		ft_putnbr_hex(number / 16);
		ft_putchar_hex(number % 16);
	}
}

void init_ADC(void)
{
    ADMUX = (1 << REFS0); // setting reference voltage, here AVCC
    ADMUX |= (1 << ADLAR); // 8 bits conversion
    ADMUX &= 0xF0; // setting the last four MUX bits to 0, aka using ADC0 (useless in this case)
    ADCSRA = (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0); // prescaler 128 (see calculations)
    ADCSRA |= (1 << ADEN); // starting the ADC
}

void set_conversion_type(int conversion)
{
    ADMUX &= 0xF8; // reset which ADC is being read from
    if (conversion != 0)
        ADMUX |= conversion;
}

int get_conversion(void)
{
    ADCSRA |= (1 << ADSC);      // saying "i want a conversion"
    while (ADCSRA & (1 << ADSC)) {}  // once it's off : "here's your conversion bro"
    return (ADCH); // reading into high register since its an 8 bit conversion
}

int main ()
{
    init_ADC();
    uartinit();
    
    uint8_t nb;
    int i = 0;
    while (1)
    {
        i = -1;
        while (++i < 3)
        {
            _delay_ms(20);
            set_conversion_type(i);
            nb = get_conversion();
            if (nb == 0 || (nb > 0 && nb <= 9))
                uart_tx('0');
            ft_putnbr_hex(nb);
            if (i < 2)
                uart_printsr(", ");
        }
        uart_printsr("\r\n");
    }
}
