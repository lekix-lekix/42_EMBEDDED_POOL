/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i2c.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:45:09 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 01:46:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void i2c_init(void)
{
    TWBR = 72;          // Prescaler already set to 1
    TWSR = 0;
}

void i2c_write(unsigned char data)
{
    TWDR = data;                                        // Writing into the TWI Data Register
    TWCR = (1 << TWINT | 1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {}
}

void i2c_read(void)
{
    TWCR = (1 << TWINT | 1 << TWEN | 1 << TWEA);        // TWEA : asking for ACK
    while (!(TWCR & (1 << TWINT))) {}                   
}

void i2c_start(void)
{                                                      // TWINT = TWI interrupt flag, when set starts the the operation of the TWI
    TWCR = (1 << TWSTA | 1 << TWINT | 1 << TWEN);      // TWSTA = checks if bus is available and generate a START condition if so
    while (!(TWCR & (1 << TWINT))) {}                  // TWSR = TWI status register
}                                                      // TWEN = starts thw TWI module

void i2c_stop(void)
{
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);  // TWSTO : TWI stop to send at the end of communication
    while (!(TWCR & (1 << TWSTO))) {}
}

uint8_t i2c_getstatus(void)
{
    return (TWSR & 0xF8);                              // Ignoring last 3 bits (prescaler) to get status  
}

