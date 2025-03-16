/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:25:47 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 11:56:46 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

#include <avr/io.h>
#include <avr/eeprom.h>

void uartinit(void);
void uart_tx(char c);
char uart_rx(void);
void uart_printsr(char *str);

void    prompt(char *str);
void	ft_putchar_hex_red(uint8_t nb);
int	    ft_bzero(char *str);

unsigned char eeprom_read(unsigned int addr);
void    eeprom_write(unsigned int addr, unsigned char data);
void    eeprom_hexdump(int16_t print_red);

void	uint16_putnbr_hex(uint16_t nb);
void	uint8_putnbr_hex(uint8_t nb);
void    uint8_putnbr_hex_red(uint8_t nb);
void	ft_putnbr(int nb);
uint32_t hex_to_int_hun(char *str);
uint16_t hex_to_int(char *str);

void    print_addr(uint16_t i);


#endif