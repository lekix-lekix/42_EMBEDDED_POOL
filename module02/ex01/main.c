/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:34:19 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/06 17:47:55 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uart_printstr(const char *str)
{
    int i = 0;

    while (str[i])
    {
        while (!(UCSR0A & (1 << UDRE0))) {}
        UDR0 = str[i];
        i++;
    }
}

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
    uart_printstr("Hello World!\r\n");
    reti();
}

void uartinit(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0); // setting the "Transmit Enabled" bit in the UCSR0B register
}

void uart_tx(unsigned char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {} // checking that no transmission is ongoing, by checking the "Data Register Empty" flag
    UDR0 = c; // loading the data buffer with our char
}

void init_timer()
{
    OCR1A = 3124; // setting PWM resolution, aka when to trigger an event
    ICR1 = 62499; // setting TOP value, aka full phase of the PWM
    TIMSK1 = (1 << OCIE1A);
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13 | 1 << WGM12 | 1 << CS12); // toggling fast PWM mode 14 with 256 prescaler
}

int main ()
{
    sei();
    init_timer();
    uartinit();
    while (1) {}
}