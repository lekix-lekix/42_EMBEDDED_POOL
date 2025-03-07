/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 13:14:35 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/07 14:43:28 by kipouliq         ###   ########.fr       */
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

ISR(USART_RX_vect, ISR_BLOCK)
{
    char c = uart_rx();
    if (c >= 'a' && c <= 'z')
        uart_tx(c - 32);
    else if (c >= 'A' && c <= 'Z')
        uart_tx(c + 32);
    else
        uart_tx(c);
}

int main ()
{
    uartinit();
    uart_printstr("Hey there!: \n");
    _delay_ms(500);
    uart_printstr("Please enter your login: \n");
    uart_tx(9); // tab
    int flag;
    // char username[64];
    // char password[64];
    while (1) 
    {
        uart_printstr("Please enter your login:\r\n");
        uart_tx(9); // tab
        while (flag)
        {
            
        }
    }
}