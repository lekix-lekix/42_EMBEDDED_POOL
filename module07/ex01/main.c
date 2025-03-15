/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/15 20:26:42 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

int main ()
{
    uint16_t addr;
    uint8_t data;
    char str[12];
    unsigned char byte; 
    
    uartinit();
    ft_bzero(str);
    while (1)
    {
        prompt(str);
        if (!str[0])
            continue;
        addr = hex_to_int_hun(str + 5);
        data = hex_to_int(str + 9);
        
        // uint8_putnbr_hex(addr);
        // uart_printsr("\n\r");
        // uint16_putnbr_hex(data);
        // uart_printsr("\n\r");
        byte = eeprom_read(addr);
        if (byte == data)
            continue ;
        else
        {
            eeprom_write(addr, data);
            eeprom_hexdump(addr);
        }
        ft_bzero(str);
    }
}