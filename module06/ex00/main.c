/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:32:10 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/15 11:23:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

void uart_init(void)
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

void uart_printstr(char *str)
{
    for (int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

void i2c_init(void)
{
    TWBR = 72;          // Prescaler already set to 1
    TWCR = (1 << TWEN); // starting the TWI module (i2c)
}

void i2c_start(void)                              // TWEN = Enable TWI module
{                                                 // TWINT = TWI interrupt flag, when set starts the the operation of the TWI
    TWCR = (1 << TWSTA | 1 << TWINT | 1 << TWEN); // TWSTA = checks if bus is available and generate a START condition if so
    while (!(TWCR & (1 << TWINT))) {}             // TWSR = TWI status register
    if ((TWSR & 0xF8) == 0x08)                    // Checking if start went through
        uart_printstr("START acknowledge.\r\n");        
}

void i2c_stop(void)
{
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);  // TWSTO = generate stop on the bus
}

int main ()
{
    uart_init();
    i2c_init();
    i2c_start();
    i2c_stop();
    while (1) {}
}

// i2c baud = (freq_cpu / desired_freq(100khz) - 16) / (2 * prescaler)
// i2c baud = (16000000 / 100000 - 16) / (2 * 1)
// = 72