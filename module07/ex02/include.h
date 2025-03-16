/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:25:47 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 16:29:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

# define M_KEY     0x8d
# define M_CONTENT 0x8e
# define M_EMPTY   0x8f

#include <avr/io.h>
#include <avr/eeprom.h>

void uartinit(void);
void uart_tx(char c);
char uart_rx(void);
void uart_printsr(char *str);

void    prompt(char *str, int len);
void	ft_putchar_hex_red(char c);

int     ft_strlen(char *str);
int     ft_strcmp(char *s1, char *s2);
int	    ft_bzero(char *str, int len);
int     is_printable(char c);

unsigned char eeprom_read(unsigned int addr);
void    eeprom_write(unsigned int addr, unsigned char data);
void    eeprom_hexdump(int16_t print_red);

void	uint16_putnbr_hex(uint16_t nb);
void	uint8_putnbr_hex(uint8_t nb);
void    uint8_putnbr_hex_red(uint8_t nb);
void	ft_putnbr(int nb);
uint16_t hex_to_int_hun(char *str);
uint16_t hex_to_int(char *str);

int key_match(char *key, unsigned int addr);
// uint8_t key_match(char *key, uint16_t addr, char *content);

void ft_read(void);
void ft_write(void);
void ft_forget(void);

#endif