/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_forget.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:28:07 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 17:11:25 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void    forget_key_content(int addr)
{
    uint8_t byte;
    int i = 0;

    eeprom_write(addr, M_EMPTY);
    addr++;
    while (i < 69)
    {
        byte = eeprom_read(addr + i);
        if (byte == M_CONTENT)
        {
            eeprom_write(addr + i, M_EMPTY);
            break;
        }
        i++;
    }
}

void    ft_forget(void)
{
    char key[33];
    uint8_t byte;

    ft_bzero(key, 33);
    
    uart_printsr("ENTER A KEY: ");
    prompt(key, 33);
    uart_printsr("\r\n");
    for (int addr = 0; addr < 1024; addr++)
    {
        byte = eeprom_read(addr);
        if (byte == M_KEY && key_match(key, addr + 1))
        {
            forget_key_content(addr);
            uart_printsr("DELETED KEY/VALUE PAIR\r\n");
            return ;
        }
    }
    uart_printsr("KEY NOT FOUND\r\n");
}
