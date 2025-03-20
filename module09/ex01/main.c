/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:22:53 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/20 13:31:14 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

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

int check_pin(int exp_register, int button)
{
    i2c_start();
    i2c_write(0x40);
    i2c_write(exp_register);
    i2c_start();
    i2c_write(0x41);
    i2c_read();
    if (!(TWDR & (1 << button)))
    {
        _delay_ms(50);
        if (!(TWDR & (1 << button)))
            return (1);
    }
    return (0);
}

// int return_bit()

int main ()
{
    int state = 0b11100001;
    int led_state = 0b11111111;
    int led_mask = 0b00001110;

    int nb = 0;

    i2c_init();
    expander_set_register(CONFIG_REG_0, state);
    expander_set_register(OUTPUT_REG_0, led_state);
    while (1)
    {
        if (check_pin(INPUT_REG_0, 0))
        {
            nb++;
            led_state = (~led_mask) | ~(nb % 8 << 1); // car nb max = 7 = 0b00000111 comme on bitshift de 1 on aura toujours un 0 en dernier et quatre 0 en premier
            expander_set_register(OUTPUT_REG_0, led_state);
            while (check_pin(INPUT_REG_0, 0)) {}
        }
    }
}