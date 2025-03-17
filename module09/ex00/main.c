/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:51:59 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 16:35:14 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

# define INPUT_REG_0 0x00
# define OUTPUT_REG_0 0x02
# define CONFIG_REG_0 0x06

void expander_set_register(int exp_register, int state)
{
    i2c_start();
    i2c_write(0x40); 
    i2c_write(exp_register);
    i2c_write(state);
    i2c_stop();
}

// void expander_set_D9()
// {
//     i2c_start();
//     i2c_write(0x40);
//     i2c_write(0x06);
//     i2c_write(0xFF & (0 << 3));
//     i2c_stop();
// 


int main ()
{
    uint8_t state = 0xFF & (0 << 3);

    i2c_init();
    expander_set_register(CONFIG_REG_0, 0xFF);
    expander_set_register(CONFIG_REG_0, 0b11110111);
    expander_set_register(OUTPUT_REG_0, 0b11110111);

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