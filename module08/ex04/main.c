/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:11:50 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/18 21:06:06 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define DDR_SPI    DDRB
# define MOSI       PB3
# define SCK        PB5
# define SS         PB2

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

void SPI_send(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {}
}

void SPI_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
    SPI_send(0xFF | (brightness & 0x1F));
    SPI_send(b);
    SPI_send(g);
    SPI_send(r);
}

void init_SPI(void)
{
    DDR_SPI = (1 << MOSI | 1 << SCK | 1 << SS);                 // MOSI = Master Out Slave In
                                                                // SCK = Serial Clock
                                                                // SS = Slave Select
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);              // SPE = SPI Enable
                                                                // MSTR = Master Enabled
                                                                // SPRO = SPI clock Rate
    DDRB |= (1 << SS);
}

void	add_char(char *str, char c, int len)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (i < len)
		str[i] = c;
}

void    del_char(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    str[i - 1] = '\0';
}

int is_nb(char c)
{
    return (c >= '0' && c <= '9');
}

int is_valid_char(char c)
{
    return (c >= 'A' && c <= 'F');
}

int	ft_bzero(char *str, int len)
{
	int	i;

	i = -1;
	while (++i < len)
		str[i] = '\0';
    return (-1);
}

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

int	ft_strlen(char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i])
    {
        if (s1[i] != s2[i])
            return (-1);
        i++;
    }
    if (s1[i] != s2[i])
        return (-1);
    return (0);
}

void	prompt(char *str, int len)
{
	char	c = '\0';
    int     i = 0;

	ft_bzero(str, len);
	while (c != 13) // carriage return (enter)
	{
		if (UCSR0A & (1 << RXC0)) // receive complete flag
		{
			c = uart_rx();
            if (is_printable(c) && i < len)
            {
				add_char(str, c, len);
                uart_tx(c);
                i++;
            }
            if (c == 127 && i > 0)
            {
				del_char(str);
                uart_printsr("\x08 \x08"); // move cursor left, print space, move cursor left
                i--;
            }
		}
	}
}

uint16_t get_hex_value(char c)
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

uint8_t hex_to_int(char c1, char c2)
{
    return ((get_hex_value(c1) * 16) + get_hex_value(c2));
}

void set_one_led(int led_idx, uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
    {
        if (i == led_idx)
            SPI_rgb(r, g, b, 31);
        else
            SPI_rgb(0, 0, 0, 31);
    }
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}

void set_all_leds(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
        SPI_rgb(r, g, b, 31);
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}

void wheel(uint8_t pos) 
{
    pos = 255 - pos;
    if (pos < 85) 
        set_all_leds(255 - pos * 3, 0, pos * 3);
    else if (pos < 170) 
    {
        pos = pos - 85;
        set_all_leds(0, pos * 3, 255 - pos * 3);
    }
    else
    {
        pos = pos - 170;
        set_all_leds(pos * 3, 255 - pos * 3, 0);
    }
}

void launch_rainbow()
{
    TCCR1B = (1 << WGM13 | 1 << CS11 | 1 << CS10 );
    ICR1 = 65535;
    while (1)
        wheel(TCNT1 >> 8);
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

void go_spi_rgb(char *buffer)
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    int led_nb;

    buffer += 1;
    
    r = hex_to_int(buffer[0], buffer[1]);
    g = hex_to_int(buffer[2], buffer[3]);
    b = hex_to_int(buffer[4], buffer[5]);
    led_nb = get_hex_value(buffer[7]) - 6;

    set_one_led(led_nb, r, g, b);
}

int check_prompt(char *buffer)
{
    if (ft_strcmp(buffer, "#FULLRAINBOW") == 0)
    {
        launch_rainbow();
        return (1);
    }
    if (ft_strlen(buffer) != 9 && ft_strlen(buffer) != 12)
        return (0);
    if (buffer[0] != '#')
        return (0);
    buffer += 1;
    for (int i = 0; i < 8; i++)
    {
        if (!is_nb(buffer[i]) && !is_valid_char(buffer[i]))
            return (0);
    }
    if (buffer[7] != '6' && buffer[7] != '7' && buffer[7] != '8')
        return (0);
    return (1);
}

int main()
{
    uartinit();
    init_SPI();
    char buffer[13];

    while (1)
    {
        uart_printsr("Enter a #RRGGBBDx color : ");
        prompt(buffer, 13);
        uart_printsr("\r\n");
        if (!check_prompt(buffer))
        {
            uart_printsr("Wrong format ! Aborting..\r\n");
            continue ;
        }
        go_spi_rgb(buffer);
    }
}