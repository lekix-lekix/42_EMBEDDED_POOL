/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:32:10 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/18 17:38:14 by lekix            ###   ########.fr       */
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

void print_status(uint8_t status_code)
{
    switch (status_code) {
    case TW_START:
    uart_printstr("START acknowledge.\r\n");
    break;
    case TW_REP_START:
    uart_printstr("REPEATED START acknowledge.\r\n");
    break;
    case TW_MT_SLA_ACK:
    uart_printstr("Master Transmitter: Slave ACK\r\n");
    break;
    case TW_MT_SLA_NACK:
    uart_printstr("Master Transmitter : Slave NACK\r\n");
    break;
    case TW_MT_DATA_ACK:
    uart_printstr("Master Transmitter : Data ACK\r\n");
    break;
    case TW_MT_DATA_NACK:
    uart_printstr("Master Transmitter: Data NACK\r\n");
    break;
    case TW_MR_SLA_ACK:
    uart_printstr("Master Receiver : Slave ACK\r\n");
    break;
    case TW_MR_SLA_NACK:
    uart_printstr("Master Receiver : Slave NACK\r\n");
    break;
    case TW_MR_DATA_ACK:
    uart_printstr("Master Receiver : Data ACK\r\n");
    break;
    case TW_MR_DATA_NACK:
    uart_printstr("Master Receiver : Data NACK\r\n");
    break;
    case TW_MT_ARB_LOST:
    uart_printstr("Arbitration Lost\r\n");
    break;
    case TW_ST_SLA_ACK:
    uart_printstr("Slave Transmitter : Slave ACK\r\n");
    break;
    case TW_ST_ARB_LOST_SLA_ACK:
    uart_printstr("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
    break;
    case TW_ST_DATA_ACK:
    uart_printstr("Slave Transmitter : Data ACK\r\n");
    break;
    case TW_ST_DATA_NACK:
    uart_printstr("Slave Transmitter : Data NACK\r\n");
    break;
    case TW_ST_LAST_DATA:
    uart_printstr("Slave Transmitter : Last Data\r\n");
    break;
    case TW_SR_SLA_ACK:
    uart_printstr("Slave Receiver : Slave ACK\r\n");
    break;
    case TW_SR_ARB_LOST_SLA_ACK:
    uart_printstr("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
    break;
    case TW_SR_GCALL_ACK:
    uart_printstr("General Call : Slave ACK\r\n");
    break;
    case TW_SR_ARB_LOST_GCALL_ACK:
    uart_printstr("Arbitration Lost in General Call, Slave ACK\r\n");
    break;
    case TW_SR_DATA_ACK:
    uart_printstr("Slave Receiver : Data ACK\r\n");
    break;
    case TW_SR_DATA_NACK:
    uart_printstr("Slave Receiver : Data NACK\r\n");
    break;
    case TW_SR_GCALL_DATA_ACK:
    uart_printstr("General Call : Data ACK\r\n");
    break;
    case TW_SR_GCALL_DATA_NACK:
    uart_printstr("General Call : Data NACK\r\n");
    break;
    case TW_SR_STOP:
    uart_printstr("Slave Receiver : STOP received\r\n");
    break;
    case TW_NO_INFO:
    uart_printstr("No state information available\r\n");
    break;
    case TW_BUS_ERROR:
    uart_printstr("Bus Error\r\n");
    break;
    default:
    uart_printstr("Unknown Status Code\r\n");
}
}

void uart_init(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // "Transmit Enabled" / "Receive Enabled" / "Receive Complete Interrupt Enable"
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
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