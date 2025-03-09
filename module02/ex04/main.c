/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:14:35 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/09 11:41:34 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_printstr(const char *str)
{
    int i = -1;

    while (str[++i])
    {
        while (!(UCSR0A & (1 << UDRE0))) {}
        UDR0 = str[i]; // sending char by char
    }
}

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

void    add_char(char *str, char c)
{
    int i = 0;
    while (str[i])
        i++;
    if (i < 63)
    {
        str[i] = c;
        str[i + 1] = '\0';
    }
}

void    del_char(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    str[i - 1] = '\0';
}

int ft_strcmp(char *s1, char *s2)
{
    int i = -1;

    while (s1[++i])
    {
        if (s1[i] != s2[i])
            return (0);
    }
    if (s1[i] != s2[i])
        return (0);
    return (1);
}

void    ft_bzero(char *str)
{
    int i = -1;
    while (++i < 64)
        str[i] = '\0';
}

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

void prompt(char *str, int pwd)
{
    char c = '\0';
    int i = 0;
    while (c != 13) // carriage return (enter)
    {
        if (UCSR0A & (1 << RXC0)) // receive complete flag
        {
            c = uart_rx();
            if (pwd && is_printable(c) && i < 63)
            {
                uart_tx('*');
                add_char(str, c);
                i++;
            }
            else if (is_printable(c) && i < 63)
            {
                add_char(str, c);
                uart_tx(c);
                i++;
            }
            if (c == 127 && i > 0)
            {
                del_char(str);
                uart_printstr("\x08 \x08"); // move cursor left, print space, move cursor left
                i--;
            }
        }
    }
    uart_printstr("\r\n\t");
}

void run_wild(void)
{
    uart_printstr("\x1b[2J\x1b[H");
    DDRB = (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB5);
    DDRD = (1 << PD3 | 1 << PD5 | 1 << PD6);
    int rgb;
    long int i = 0;
    rgb = 0;
    while (1)
    {
        i++;
        if (i == 20000)
        {
            if (rgb == 0)
            {
                uart_printstr("\x1b[1;92;9mWELCOME LEKIX\t");
                PORTD ^= (1 << PD3);
            }
            if (rgb == 1)
            {
                uart_printstr("\x1b[1;93;9mWELCOME LEKIX\t");
                PORTD ^= (1 << PD5);
            }
            if (rgb == 2)
            {
                uart_printstr("\x1b[1;96;9mWELCOME LEKIX\t");
                PORTD ^= (1 << PD6);
            }
        }
        if (i == 80294)
        {
            PORTB ^= (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4 | 1 << PB5);
            i = 0;        
        }
        if (rgb == 2)
            rgb = 0;
        else
            rgb++;
    }
}

void user_pwd_prompt()
{
    char buff_user[64];
    char buff_pwd[64];
    char *user = "lekix";
    char *pwd = "password";
    
    ft_bzero(buff_user);
    ft_bzero(buff_pwd);
    uart_printstr("username: ");
    prompt(buff_user, 0);
    uart_printstr("password: ");
    prompt(buff_pwd, 1);
    if (ft_strcmp(buff_user, user) && ft_strcmp(buff_pwd, pwd))
        run_wild();
    else
        uart_printstr("Wrong username / password combination!\r\n\t");
}

int main ()
{
    uartinit();
    uart_printstr("\x1B[2J\x1B[H");
    uart_printstr("Hey there! \r\n");
    _delay_ms(500);
    while (1) 
    {
        _delay_ms(500);
        uart_printstr("Please enter your login:\r\n\t");
        uart_tx(2);
        user_pwd_prompt();
    }
}
