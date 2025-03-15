/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/15 00:51:13 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {} // checking that no transmission is ongoing, by checking the "Data Register Empty" flag
    UDR0 = c; // loading the data buffer with our char
}

void uart_printstr(char *str)
{
    for (int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

void uart_init(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // "Transmit Enabled" / "Receive Enabled" / "Receive Complete Interrupt Enable"
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
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

void	ft_putnbr_hex(uint16_t nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number < 16)
    {
		ft_putchar_hex(number);
    }
	if (number >= 16)
	{
		ft_putnbr_hex(number / 16);
		ft_putchar_hex(number % 16);
	}
}

void    print_addr(int16_t i)
{
    if (i < 16)
        uart_printstr("000000");
    else if (i >= 16 && i <= 255)
        uart_printstr("00000");
    else
        uart_printstr("0000");
    ft_putnbr_hex(i);
    uart_tx(' ');
}

void    print_line_content(uint8_t range)
{
    int8_t *addr;
    int8_t content;
    for (int i = 0; i < 16; i++)
    {
        addr = (int8_t *)(range + i);
        eeprom_busy_wait();
        content = eeprom_read_byte((const uint8_t *)addr);
        if (content < 16)
            uart_printstr("0");
        ft_putnbr_hex(content);
        uart_printstr(" ");
    }
}



int main ()
{
    eeprom_busy_wait();
    eeprom_hexdump();
    while (1){}
}