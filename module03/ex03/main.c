/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 19:24:07 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/09 20:07:16 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uartinit(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // setting the "Receive Enabled" bit in the UCSR0B register
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
}

char uart_rx(void)
{
    while (!(UCSR0A & (1 << RXC0))) {} // checking that dats transmission is complete by checking the "Receive Complete"
    return (UDR0); // RX and TX share the same buffer register
}

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {} // checking that no transmission is ongoing, by checking the "Data Register Empty" flag
    UDR0 = c; // loading the data buffer with our char
}

void uart_printstr(const char *str)
{
    int i = -1;

    while (str[++i])
    {
        while (!(UCSR0A & (1 << UDRE0))) {}
        UDR0 = str[i]; // sending char by char
    }
}

void init_rgb()
{
    DDRD = (1 << PD6 | 1 << PD5 | 1 << PD3);      // activating leds
    TCCR0A = (1 << WGM00 | 1 << WGM01);           // activating timer0 to control brightness 
    TCCR0B = (1 << CS00 | 1 << CS01);             

    TCCR0A |= (1 << COM0A1 | 1 << COM0B1);
    
    TCCR2A = (1 << COM2B1 | 1 << WGM20 | 1 << WGM21);
    TCCR2B = (1 << CS20 | 1 << CS21);
    
    TCCR1B = (1 << WGM13 | 1 << CS12 | 1 << CS10);
    ICR1 = 65535;
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    OCR0A = r;
    OCR0B = g;
    OCR2B = b;
}

void    ft_bzero(char *str)
{
    int i = -1;
    while (++i < 64)
        str[i] = '\0';
}

void    add_char(char *str, char c)
{
    int i = 0;
    while (str[i])
        i++;
    if (i < 7)
    {
        str[i] = c;
        str[i + 1] = '\0';
    }
}

int get_hex_value(char c)
{
    char base[] = "0123456789ABCDEF";
    int i = -1;

    while (base[++i])
    {
        if (base[i] == c)
            return (i);
    }
    return (-1);
}

void hex_to_int(int *r, int *g, int *b, char *str)
{
    int i = 0;
    int nb;
    
    str += 1;
    while (str[i])
    {
        nb = (get_hex_value(str[i]) * 16) + get_hex_value(str[i + 1]);
        if (i < 2)
            *r = nb;
        else if (i >= 2 && i < 4)
            *g = nb;
        else
            *b = nb;
        i += 2;
    }
}

void prompt(char *str)
{
    char c = '\0';
    while (c != 13) // carriage return (enter)
    {
        if (UCSR0A & (1 << RXC0)) // receive complete flag
        {
            c = uart_rx();
            add_char(str, c);
            uart_tx(c);
        }
    }
}

ISR(USART_RX_vect, ISR_BLOCK)
{
    char str[8];
    int r;
    int g;
    int b;

    ft_bzero(str);
    prompt(str);
    hex_to_int(&r, &g, &b, str);
    init_rgb();
    set_rgb(r, g, b);
}

int main ()
{
    sei();
    uartinit();
    while (1) {}
}