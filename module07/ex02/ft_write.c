/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:32:38 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 18:03:01 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void write_pair(char *key, char *content, unsigned int addr)
{
    int i = 0;

    eeprom_write(addr, M_KEY);
    addr++;
    while (key[i])
    {
        eeprom_write(addr, key[i]);
        addr++;
        i++;
    }
    eeprom_write(addr, '\0');
    addr++;
    eeprom_write(addr, M_CONTENT);
    addr++;
    addr++;
    i = 0;
    while (content[i])
    {
        eeprom_write(addr, content[i]);
        addr++;
        i++;
    }
    eeprom_write(addr, '\0');
    addr++;
    eeprom_write(addr, M_EMPTY);
}

int key_match(char *key, unsigned int addr)
{
    char key_cmp[33];
    int i = -1;

    ft_bzero(key_cmp, 33);
    while (++i < 33 && i != M_CONTENT)
        key_cmp[i] = eeprom_read(addr + i);
    key_cmp[i - 1] = '\0';
    if (ft_strcmp(key, key_cmp) == 0)
        return (1);
    return (0);
}

int check_empty_space(uint8_t addr)
{
    int i = 1;
    uint8_t byte;

    byte = eeprom_read(addr);
    while (byte != M_KEY && byte != M_CONTENT)
    {
        byte = eeprom_read(addr);
        addr++;
        i++;
    }
    return (i);
}

void ft_write(void)
{
    uint8_t byte;
    char key[33];
    char content[33];
    int total_len;
    
    ft_bzero(key, 33);
    ft_bzero(content, 33);
    
    uart_printsr("ENTER KEY: ");
    prompt(key, 32);
    if (ft_strlen(key) == 0)
    {
        uart_printsr("\r\nKEY CANNOT BE EMPTY\r\n");
        return ;
    }
    uart_printsr("\r\n\0");
    uart_printsr("ENTER CONTENT: ");
    prompt(content, 32);
    if (ft_strlen(content) == 0)
    {
        uart_printsr("\r\nCONTENT CANNOT BE EMPTY\r\n");
        return ;
    }
    uart_printsr("\r\n\0");
    
    int free_space = 0;
    total_len = (ft_strlen(key) + 1) + (ft_strlen(content) + 1) + 3; 
    for (int addr = 0; addr < 1024; addr++)
    {
        byte = eeprom_read(addr);
        if (byte == M_KEY && key_match(key, addr + 1))
        {
            uart_printsr("KEY ALREADY FOUND AND MUST BE UNIQUE.\r\n");
            return ;
        }
        if (byte == M_EMPTY && check_empty_space(addr) > total_len)
        {
            write_pair(key, content, addr);
            uart_printsr("DONE ! ADDRESS: ");
            uint8_putnbr_hex(addr);
            uart_printsr("\r\n\0");
            return ;
        }
        if (free_space >= total_len)
        {
            write_pair(key, content, addr - free_space);
            uart_printsr("DONE ! ADDRESS: ");
            uint8_putnbr_hex(addr);
            uart_printsr("\r\n");
            return ;
        }
        if (byte == 0xFF)
            free_space++;
        else if (byte != 0xFF)
            free_space = 0;
    }
    uart_printsr("NO SPACE LEFT\r\n\0");
}