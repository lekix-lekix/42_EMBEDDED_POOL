/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c_print_status.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:48:15 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 01:59:08 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"
#include <util/twi.h>

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

void uart_init(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // "Transmit Enabled" / "Receive Enabled" / "Receive Complete Interrupt Enable"
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
}


void	print_status(uint8_t status_code)
{
	switch (status_code)
	{
	case TW_START:
		uart_printstr("START acknowledge.\r\n");
		break ;
	case TW_REP_START:
		uart_printstr("REPEATED START acknowledge.\r\n");
		break ;
	case TW_MT_SLA_ACK:
		uart_printstr("Master Transmitter: Slave ACK\r\n");
		break ;
	case TW_MT_SLA_NACK:
		uart_printstr("Master Transmitter : Slave NACK\r\n");
		break ;
	case TW_MT_DATA_ACK:
		uart_printstr("Master Transmitter : Data ACK\r\n");
		break ;
	case TW_MT_DATA_NACK:
		uart_printstr("Master Transmitter: Data NACK\r\n");
		break ;
	case TW_MR_SLA_ACK:
		uart_printstr("Master Receiver : Slave ACK\r\n");
		break ;
	case TW_MR_SLA_NACK:
		uart_printstr("Master Receiver : Slave NACK\r\n");
		break ;
	case TW_MR_DATA_ACK:
		uart_printstr("Master Receiver : Data ACK\r\n");
		break ;
	case TW_MR_DATA_NACK:
		uart_printstr("Master Receiver : Data NACK\r\n");
		break ;
	case TW_MT_ARB_LOST:
		uart_printstr("Arbitration Lost\r\n");
		break ;
	case TW_ST_SLA_ACK:
		uart_printstr("Slave Transmitter : Slave ACK\r\n");
		break ;
	case TW_ST_ARB_LOST_SLA_ACK:
		uart_printstr("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
		break ;
	case TW_ST_DATA_ACK:
		uart_printstr("Slave Transmitter : Data ACK\r\n");
		break ;
	case TW_ST_DATA_NACK:
		uart_printstr("Slave Transmitter : Data NACK\r\n");
		break ;
	case TW_ST_LAST_DATA:
		uart_printstr("Slave Transmitter : Last Data\r\n");
		break ;
	case TW_SR_SLA_ACK:
		uart_printstr("Slave Receiver : Slave ACK\r\n");
		break ;
	case TW_SR_ARB_LOST_SLA_ACK:
		uart_printstr("Arbitration Lost in SLA+R/W, Slave ACK\r\n");
		break ;
	case TW_SR_GCALL_ACK:
		uart_printstr("General Call : Slave ACK\r\n");
		break ;
	case TW_SR_ARB_LOST_GCALL_ACK:
		uart_printstr("Arbitration Lost in General Call, Slave ACK\r\n");
		break ;
	case TW_SR_DATA_ACK:
		uart_printstr("Slave Receiver : Data ACK\r\n");
		break ;
	case TW_SR_DATA_NACK:
		uart_printstr("Slave Receiver : Data NACK\r\n");
		break ;
	case TW_SR_GCALL_DATA_ACK:
		uart_printstr("General Call : Data ACK\r\n");
		break ;
	case TW_SR_GCALL_DATA_NACK:
		uart_printstr("General Call : Data NACK\r\n");
		break ;
	case TW_SR_STOP:
		uart_printstr("Slave Receiver : STOP received\r\n");
		break ;
	case TW_NO_INFO:
		uart_printstr("No state information available\r\n");
		break ;
	case TW_BUS_ERROR:
		uart_printstr("Bus Error\r\n");
		break ;
	default:
		uart_printstr("Unknown Status Code\r\n");
	}
}