/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 01:58:31 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

#include <avr/io.h>
#include <util/delay.h>

void uart_init(void);

void i2c_init(void);
void i2c_write(unsigned char data);
void i2c_read(void);
void i2c_start(void);
void i2c_stop(void);
void print_status(uint8_t status_code);
uint8_t i2c_getstatus(void);

#endif