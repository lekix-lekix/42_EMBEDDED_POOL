/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:34:19 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/09 14:58:23 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void uartinit(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0); // setting the "Transmit Enabled" bit in the UCSR0B register
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
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
        uart_tx(str[i]); // sending char by char
    }
}

ISR(TIMER1_COMPA_vect, ISR_BLOCK)
{
    uart_printstr("Hello World!\r\n");
    reti(); // interrupts are being deactivated once one is triggered, so this reactivates the interrupts
}

void init_timer()
{
    OCR1A = 31250; // (CPU freq / prescaler) * freq desired -> (16mhz / 1024) * 2 = 31250
    TIMSK1 = (1 << OCIE1A); // enabling an interrupt on when an Output Compare A Match happens
    TCCR1B = (1 << WGM12 | 1 << CS12 | 1 << CS10); // toggling CTC mode with 1024 prescaler
}

int main ()
{
    sei(); // activating interrupts globally
    init_timer();
    uartinit();
    while (1) {}
}

// prescaler = 16 000 000 (cpu freq) * 2 (freq desired, hz) / 65636 (overflow value) ~= 487