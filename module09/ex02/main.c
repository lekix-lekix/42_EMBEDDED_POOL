/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 12:22:53 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/17 15:40:41 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include.h"

# define INPUT_REG_0 0x00
# define OUTPUT_REG_0 0x02
# define OUTPUT_REG_1 0x03
# define CONFIG_REG_0 0x06
# define CONFIG_REG_1 0x07

#define SEG_0 0b00111111  // 0
#define SEG_1 0b00000110  // 1
#define SEG_2 0b01011011  // 2
#define SEG_3 0b01001111  // 3
#define SEG_4 0b01100110  // 4
#define SEG_5 0b01101101  // 5
#define SEG_6 0b01111101  // 6
#define SEG_7 0b00000111  // 7
#define SEG_8 0b01111111  // 8
#define SEG_9 0b01101111  // 9

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

int main ()
{
    int state = 0b01111111;

    i2c_init();
    expander_set_register(CONFIG_REG_0, state);
    expander_set_register(OUTPUT_REG_0, state);
    expander_set_register(CONFIG_REG_1, 0x00);
    expander_set_register(OUTPUT_REG_1, SEG_2);
    while (1)
    {
        
    }
}