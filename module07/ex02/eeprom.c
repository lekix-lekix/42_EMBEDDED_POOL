/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:20:09 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 01:06:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void    print_addr(int16_t i)
{
    if (i < 16)
        uart_printsr("000000");
    else if (i >= 16 && i <= 255)
        uart_printsr("00000");
    else
        uart_printsr("0000");
    uint16_putnbr_hex(i);
    uart_tx(' ');
}

unsigned char eeprom_read(unsigned int addr)
{
    while (EECR & (1 << EEPE)) {}
    while (SPMCSR & (1 << SELFPRGEN)) {}
    EEAR = addr;
    EECR |= (1 << EERE);
    return (EEDR);
}

void    eeprom_write(unsigned int addr, unsigned char data)
{
    while (EECR & (1 << EEPE)) {}
    while (SPMCSR & (1 << SELFPRGEN)) {}
    EEAR = addr;
    EEDR = data;
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
}

void    print_line_content(int16_t *line_nb, int16_t print_red_addr)
{
    uint8_t content;
    uint8_t byte_nb = 0;
    
    for (int i = 0; i < 16; i++)
    {
        content = eeprom_read(*line_nb);
        if (content < (int8_t)16 && *line_nb == print_red_addr)
            ft_putchar_hex_red(0);
        else if (content < (int8_t)16)
            uart_tx('0');
        if (*line_nb == print_red_addr)
            uint8_putnbr_hex_red(content);
        else
            uint8_putnbr_hex(content);
        byte_nb++;
        if (byte_nb == 4)
        {
            uart_tx(' ');
            byte_nb = 0;
        }
        *line_nb += 1;
    }
}

void    eeprom_hexdump(int16_t print_red)
{
    uartinit();
    uint16_t i = 0;
    int16_t line_nb = 0;
    while (i < 1024)
    {
        print_addr(i);
        print_line_content(&line_nb, print_red);
        uart_printsr("\n\r\0");
        i += 16;
    }
}
