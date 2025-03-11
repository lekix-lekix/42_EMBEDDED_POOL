/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 19:24:07 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/11 12:41:10 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void	uartinit(void)
{
	UBRR0L = 8;                                        // initializing the baud rate low register value
	UBRR0H = 0x00;                                     // as the value is < 255 no need to set anything in the high register
	UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0);  // setting the "Receive Enabled" bit in the UCSR0B register
	UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))) {} // checking that dats transmission is complete by checking the "Receive Complete"
	return (UDR0); // RX and TX share the same buffer register
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0))) {}         // checking that no transmission is ongoing,by checking the "Data Register Empty" flag
	UDR0 = c; // loading the data buffer with our char
}

void	uart_printstr(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		while (!(UCSR0A & (1 << UDRE0))) {}
		UDR0 = str[i]; // sending char by char
	}
}

void init_rgb()
{
    DDRD = (1 << PD6 | 1 << PD5 | 1 << PD3);      // activating leds
    TCCR0A = (1 << WGM00 | 1 << WGM01);           // timer0 set to fast PWM mode 3 
    TCCR0B = (1 << CS00 | 1 << CS01);             // with a 64 prescaler

    TCCR0A |= (1 << COM0A1 | 1 << COM0B1);        // OC0A (PD6 (green)) and OC0B (PD5 (red)) cleared on compare match
    
    TCCR2A = (1 << WGM20 | 1 << WGM21);           // timer2 set to fast PWM mode 3
    TCCR2B = (1 << CS20 | 1 << CS21);             // with a 64 prescaler

    TCCR2A |= (1 << COM2B1);                      // OC2B (PD3 (blue)) cleared on compare match
    
    TCCR1B = (1 << WGM13 | 1 << CS12 | 1 << CS10); // Timer1 set to PWM phase and freq correct mode 8 with a 1024 prescaler
    ICR1 = 65535;                                  // = count from bottom to top
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0B = r;
	OCR0A = g;
	OCR2B = b;
}

void	ft_bzero(char *str)
{
	int	i;

	i = -1;
	while (++i < 8)
		str[i] = '\0';
}

void	add_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (i < 7)
		str[i] = c;
}

int	get_hex_value(char c)
{
	char	base[] = "0123456789ABCDEF";
	int		i;

	i = 0;
	while (base[i])
	{
		if (base[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	hex_to_int(char c, char d)
{
	return ((get_hex_value(c) * 16) + get_hex_value(d));
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int is_nb(char c)
{
    return (c >= '0' && c <= '9');
}

int is_valid_char(char c)
{
    return (c >= 'A' && c <= 'Z');
}

int	check_prompt(char *str)
{
	if (ft_strlen(str) != 7)
		return (-1);
    if (str[0] != '#')
        return (-1);
	for (int i = 1; str[i]; i++)
	{
        if (!is_valid_char(str[i]) && !is_nb(str[i]))
           return (-1);
	}
	return (0);
}

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

void	prompt(void)
{
    char	str[8];
	char	c = '\0';
    int     i = 0;

	ft_bzero(str);
	while (c != 13) // carriage return (enter)
	{
		if (UCSR0A & (1 << RXC0)) // receive complete flag
		{
			c = uart_rx();
            if (is_printable(c) && i < 7)
            {
                add_char(str, c);
                uart_tx(c);
                i++;
            }
		}
	}
	if (check_prompt(str) == -1)
	{
		uart_printstr("\n\rWrong format detected! Aborting..\n\r");
		return ;
	}
	int r = hex_to_int(*(str + 1), *(str + 2));
	int g = hex_to_int(*(str + 3), *(str + 4));
	int b = hex_to_int(*(str + 5), *(str + 6));
    uart_printstr("\n\rHere comes the color baby!\r\n");
	init_rgb();
	set_rgb(r, g, b);
}

int	main(void)
{
	uartinit();
    uart_printstr("\x1B[2J\x1B[H");
	while (1)
	{
		uart_printstr("Please enter a hex RGB color (#FFFFFF format): ");
		prompt();
	}
}
