/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 13:51:23 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/14 15:04:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

# define SENSOR_ADDR 0x38

#include <avr/io.h>
#include <util/delay.h>

void print_status(uint8_t status_code);
void uart_printstr(char *str);

#endif