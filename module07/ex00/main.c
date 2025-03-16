/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 01:06:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>

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
    char hex[] = "0123456789abcdef";
    for (int i = 0; hex[i]; i++)
    {
        if (i == c)
            uart_tx(hex[i]);
    }
}

void	uint16_putnbr_hex(uint16_t nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number < 16)
    {
		ft_putchar_hex(number);
    }
	if (number >= 16)
	{
		uint16_putnbr_hex(number / 16);
		ft_putchar_hex(number % 16);
	}
}

void	uint8_putnbr_hex(uint8_t nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number < 16)
		ft_putchar_hex(number);
	if (number >= 16)
	{
		uint16_putnbr_hex(number / 16);
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
    uint16_putnbr_hex(i);
    uart_tx(' ');
}

unsigned char eeprom_read(unsigned int addr)
{
    while (EECR & (1 << EEPE)) {}
    EEAR = addr;
    EECR |= (1 << EERE);
    return (EEDR);
}

void    print_line_content()
{
    uint8_t content;
    uint8_t byte_nb = 0;
    static int16_t line_nb = 0;
    
    for (int i = 0; i < 16; i++)
    {
        content = eeprom_read(line_nb);
        if (content < (int8_t)16)
            uart_tx('0');
        uint8_putnbr_hex(content);
        byte_nb++;
        if (byte_nb == 4)
        {
            uart_tx(' ');
            byte_nb = 0;
        }
        line_nb++;
    }
}

void    eeprom_hexdump(void)
{
    uart_init();
    uint16_t i = 0;
    while (i < 1024)
    {
        print_addr(i);
        print_line_content(i);
        uart_printstr("\n\r\0");
        i += 16;
    }
}

int main ()
{
    uart_init();
    // uart_printstr("\x1B[2J\x1B[H");
    eeprom_hexdump();
    while (1) {}
}