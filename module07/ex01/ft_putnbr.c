/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 16:35:54 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 12:01:27 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void	uint8_ft_putchar_hex(uint8_t nb)
{
    char hex[] = "0123456789abcdef";
    for (uint8_t i = 0; i < 16; i++)
    {
        if (i == nb)
            uart_tx(hex[i]);
    }
}

void	uint16_ft_putchar_hex(uint16_t nb)
{
    char hex[] = "0123456789abcdef";
    for (uint16_t i = 0; i < 16; i++)
    {
        if (i == nb)
            uart_tx(hex[i]);
    }
}

void	ft_putchar_hex_red(uint8_t nb)
{
    char hex[] = "0123456789abcdef";
    for (uint8_t i = 0; i < 16; i++)
    {
        if ((uint8_t)i == nb)
        {
            uart_printsr("\x1b[91m");
            uart_tx(hex[i]);
            uart_printsr("\x1b[0m\0");
        }
    }
}

void	uint16_putnbr_hex(uint16_t nb)
{
	uint16_t	number;

	number = nb;
	if (number < 16)
    {
		uint16_ft_putchar_hex(number);
    }
	if (number >= 16)
	{
		uint16_putnbr_hex(number / 16);
		uint16_ft_putchar_hex(number % 16);
	}
}

void	uint8_putnbr_hex(uint8_t nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number < 16)
		uint8_ft_putchar_hex(number);
	if (number >= 16)
	{
		uint8_putnbr_hex(number / 16);
		uint8_ft_putchar_hex(number % 16);
	}
}

void uint8_putnbr_hex_red(uint8_t nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number < 16)
		ft_putchar_hex_red(number);
	if (number >= 16)
	{
		uint8_putnbr_hex_red(number / 16);
		ft_putchar_hex_red(number % 16);
	}
}

void	ft_putchar(char c)
{
    uart_tx(c + '0');
}

void	ft_putnbr(int nb)
{
	long int	number;

	number = nb;
	if (number < 0)
	{
		number = -number;
		ft_putchar('-');
	}
	if (number >= 0 && number <= 9)
		ft_putchar(number);
	if (number > 9)
	{
		ft_putnbr(number / 10);
		ft_putchar(number % 10);
	}
}
