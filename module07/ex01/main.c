/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:51:46 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 17:46:01 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

int main ()
{
    uint16_t addr = 0;   
    uint16_t data = 0;   
    char str[12];
    unsigned char byte; 
    
    uartinit();
    ft_bzero(str);

    while (1)
    {
        prompt(str);
        uart_printsr("\n\n\r\0");
        if (!str[0])
            continue;
        addr = hex_to_int_hun(str + 5);
        data = hex_to_int(str + 9);
        byte = eeprom_read(addr);
        if (byte == data)
            uart_printsr("value is already there \n\r\0");
        else
        {
            eeprom_write(addr, data);
            eeprom_hexdump(addr);
        }
        ft_bzero(str);
    }
}