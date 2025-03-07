/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:01:03 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/07 11:53:35 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

void uartinit(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0); // setting the "Transmit Enabled" bit in the UCSR0B register
}

void uart_tx(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {} // checking that no tranmission is ongoing, by checking the "Data Register Empty" flag
    UDR0 = c; // loading the data buffer with our char
}

int main ()
{
    uartinit();
    while (1)
    {
        uart_tx('Z');
        _delay_ms(1000);
    }
}

// Baud rate register calculation : (cpu_freq / (16 * baud rate)) - 1
// here : (16 000 000 / (16 * 115200)) - 1 = 7.6805.. rounded at 8 