/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:26:49 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 00:58:53 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"



int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

void	add_char(char *str, char c, int len)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	if (i < len)
		str[i] = c;
}

void    del_char(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    str[i - 1] = '\0';
}

int is_nb(char c)
{
    return (c >= '0' && c <= '9');
}

int is_valid_char(char c)
{
    return (c >= 'A' && c <= 'Z');
}

uint8_t get_hex_value(char c)
{
	char	base[] = "0123456789abcdef";
	int		i;

	i = 0;
	while (base[i])
	{
		if (base[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

uint16_t hex_to_int_hun(char *str)
{
    return ((get_hex_value(str[0]) * 256 ) + (get_hex_value(str[1]) * 16) + get_hex_value(str[2]));
}

uint16_t hex_to_int(char *str)
{
    return ((get_hex_value(str[0]) * 16) + get_hex_value(str[1]));
}

// int	check_prompt(char *str)
// {
// 	if (ft_strlen(str) != 10 && ft_strlen(str) != 11)
// 		return (ft_bzero(str));
// 	for (int i = 0; str[i]; i++)
// 	{
//         if (str[i] == 8 && str[i] != ' ')
//             return (ft_bzero(str));
//         if (str[i] != 8 && is_valid_char(str[i]) && !is_nb(str[i]))
//             return (ft_bzero(str));
//         if ((i < 5 && str[i] != '0') || (i == 5 && hex_to_int_hun(str + i) > 1023))
//             return (ft_bzero(str));
//         if (i == 9 && hex_to_int(str + i) > 255)
//             return (ft_bzero(str));
// 	}
// 	return (0);
// }

void	prompt(char *str, int len)
{
	char	c = '\0';
    int     i = 0;

	ft_bzero(str, len);
	while (c != 13) // carriage return (enter)
	{
		if (UCSR0A & (1 << RXC0)) // receive complete flag
		{
			c = uart_rx();
            if (is_printable(c) && i < len)
            {
				add_char(str, c, len);
                uart_tx(c);
                i++;
            }
            if (c == 127 && i > 0)
            {
				del_char(str);
                uart_printsr("\x08 \x08"); // move cursor left, print space, move cursor left
                i--;
            }
		}
	}
}