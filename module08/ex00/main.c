/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 18:30:19 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 20:15:08 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void SPI_send(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {}
}

int main ()
{
    DDR_SPI = (1 << MOSI) | (1 << SCK) | (1 << SS); // MOSI = Master Out Slave In
                                                    // SCK = Serial Clock
                                                    // SS = Slave Select
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);  // SPE = SPI Enable
                                                    // MSTR = Master Enabled
                                                    // SPRO = SPI clock Rate
    PORTB = 0;
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    SPI_send(0xFF);
    SPI_send(0x00);
    SPI_send(0x00);
    SPI_send(0xFF);
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
    while (1) {
    }
}