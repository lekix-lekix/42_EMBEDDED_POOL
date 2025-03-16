/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 20:33:17 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 17:13:48 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

int main ()
{
    char cmd_prompt[7];

    uartinit();
    ft_bzero(cmd_prompt, 7);
    while (1)
    {
        uart_printsr("ENTER COMMAND : ");
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