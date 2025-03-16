/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:54:47 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/16 19:56:23 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

# define DDR_SPI    PORTB 
# define MOSI       PB3
# define SCK        PB4
# define SS         PB2

#endif