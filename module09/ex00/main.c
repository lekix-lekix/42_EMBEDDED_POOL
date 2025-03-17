/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:51:59 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 02:38:11 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void expander_set_D9()
{
    i2c_start();
    i2c_write(0x40);
    i2c_write(0x06);
    print_status(i2c_getstatus());
    i2c_write(0xFF & (0 << 3));
    print_status(i2c_getstatus());
    i2c_stop();
}

int main ()
{
    uint8_t state = 0b11110111;

    uart_init();
    i2c_init();
    expander_set_D9();
    while (1)
    {
        i2c_start();
        i2c_write(0x40);
        i2c_write(0x02);
        i2c_write(state);
        _delay_ms(1000);
        state ^= (1 << 3);
    }
}