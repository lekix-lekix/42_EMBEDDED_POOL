/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:30:44 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 17:57:02 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

char *eeprom_get_line(char *line, uint16_t key_addr)
{
    for (int i = 0; i <= 32 || i == M_CONTENT; i++)
        line[i] = eeprom_read(key_addr + i);
    return (line);
}

void get_content_from_key(unsigned int addr, char *content)
{
    uint8_t byte = 0;

    int i = 0;
    while (i < 36 && byte != M_CONTENT)
    {
        byte = eeprom_read(addr);
        addr++;
        i++;
    }
    addr++;
    i = 0;
    while (i < 33)
    {
        byte = eeprom_read(addr);
        if (byte == 0 || byte == M_EMPTY)
            break;
        content[i] = byte;
        i++;
        addr++;
    }
    content[i] = '\0';
}

void search_key(char *key)
{
    uint8_t byte;
    char content[33];

    ft_bzero(content, 33);
    
    for (int addr = 0; addr < 1024; addr++)
    {
        byte = eeprom_read(addr);

        if (byte == M_KEY && key_match(key, addr + 1))
        {
            get_content_from_key(addr, content);
            uart_printsr("CONTENT FOUND: ");
            uart_printsr(content);
            uart_printsr("\r\n");
            return ;
        }
    }
    uart_printsr("KEY NOT FOUND !\r\n");
}

void ft_read()
{
    char key[33];
    
    ft_bzero(key, 33);

    uart_printsr("ENTER KEY: ");
    prompt(key, 32);
    if (ft_strlen(key) == 0)
    {
        uart_printsr("\r\nKEY CANNOT BE EMPTY\r\n");
        return ;
    }
    uart_printsr("\n\r\0");
    search_key(key);
}
