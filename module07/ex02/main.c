/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:33:17 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 01:06:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

char *eeprom_get_line(char *line, uint16_t key_addr)
{
    for (int i = 0; i < 65 || i == CONTENT; i++)
        line[i] = eeprom_read(key_addr + i);
    return (line);
}

uint8_t key_match(char *key, uint16_t addr, char *content)
{
    ft_bzero(content, 65);
    eeprom_get_line(content, addr);
    if (ft_strcmp(key, content) == 0)
        return (1);
    return (0);
}

int16_t search_key(char *key, char *content)
{
    uint8_t byte;
    uint8_t line_nb = 0;

    for (int i = 0; i < 1024; i++)
    {
        byte = eeprom_read(line_nb);
        if (byte == KEY && key_match(key, i + 1, content))
            return (i);
    }
    return (-1);
}

void ft_read()
{
    char key[65];
    char content[65];
    uint16_t key_addr;
    
    ft_bzero(key, 65);
    ft_bzero(content, 65);

    uart_printsr("ENTER KEY: \n\r\0");
    prompt(key, 65);
    uart_printsr("\n\r\0");
    key_addr = search_key(key, content);
    if (key_addr != (uint16_t)-1)
    {
        uart_printsr(content);
        return ;
    }
    else
        uart_printsr("empty\n\r\0");
}

void ft_write()
{
    uart_printsr("WRITE FUNCTION\n\r\0");
}

void ft_forget()
{
    uart_printsr("FORGET FUNCTION\n\r\0");
}

int main ()
{
    char cmd_prompt[7];

    uartinit();
    ft_bzero(cmd_prompt, 7);
    while (1)
    {
        prompt(cmd_prompt, 7);
        uart_printsr("\n\r\0");
        if (ft_strcmp(cmd_prompt, "READ") == 0)
            ft_read();
        else if (ft_strcmp(cmd_prompt, "WRITE") == 0)
            ft_write();
        else if (ft_strcmp(cmd_prompt, "FORGET") == 0)
            ft_forget();
        else if (ft_strcmp(cmd_prompt, "PRINT") == 0)
            eeprom_hexdump(-1);
        else
            uart_printsr("Unknown command!\n\r\0");
    }
}