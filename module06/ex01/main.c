/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:32:10 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/15 11:20:22 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include.h"

void uart_tx(char c)
{
    while (!(UCSR0A & (1 << UDRE0))) {} // checking that no transmission is ongoing, by checking the "Data Register Empty" flag
    UDR0 = c; // loading the data buffer with our char
}

void uart_printstr(char *str)
{
    for (int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

void uart_init(void)
{
    UBRR0L = 8; // initializing the baud rate low register value
    UBRR0H = 0x00; // as the value is < 255, no need to set anything in the high register
    UCSR0B = (1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0); // "Transmit Enabled" / "Receive Enabled" / "Receive Complete Interrupt Enable"
    UCSR0C = (1 << UCSZ00 | 1 << UCSZ01 | 0 << USBS0); // UCSZ00 + UCSZ01 = 8bits CharSize, USBS0 << 0 = one stop bit
}

void i2c_init(void)
{
    TWBR = 72;          // Prescaler already set to 1
    TWSR = 0;
}

void i2c_write(unsigned char data)
{
    TWDR = data;                                        // Writing into the TWI Data Register
    TWCR = (1 << TWINT | 1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {}
}

uint8_t i2c_read(void)
{
    TWCR = (1 << TWINT | 1 << TWEN | 1 << TWEA);        // TWEA : asking for ACK
    while (!(TWCR & (1 << TWINT))) {}
    return (TWDR);                                      // Reading from TWI Data Register
}

void i2c_start(void)
{                                                      // TWINT = TWI interrupt flag, when set starts the the operation of the TWI
    TWCR = (1 << TWSTA | 1 << TWINT | 1 << TWEN);      // TWSTA = checks if bus is available and generate a START condition if so
    while (!(TWCR & (1 << TWINT))) {}                  // TWSR = TWI status register
}                                                      // TWEN = starts thw TWI module

void i2c_stop(void)
{
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);  // TWSTO : TWI stop to send at the end of communication
    while (!(TWCR & (1 << TWSTO))) {}
}

uint8_t i2c_getstatus(void)
{
    return (TWSR & 0xF8);                              // Ignoring last 3 bits (prescaler) to get status  
}

void aht_calibration(void)
{
    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 0);
    i2c_write(0xBE);                                    // Calibration command (0xBE, 0x08, 0x00)
    i2c_write(0x08);
    i2c_write(0x00);
    _delay_ms(10);                                      // Waiting 10 ms as asked in the datasheet 
}

void	ft_putchar_hex(char c)
{
    char hex[] = "0123456789ABCDEF";
    for (int i = 0; hex[i]; i++)
    {
        if (i == c)
            uart_tx(hex[i]);
    }
}

void	ft_putnbr_hex(uint8_t nb)
{
	long int	number;

	number = nb;
	if (number >= 0 && number < 16)
    {
		ft_putchar_hex(number);
    }
	if (number >= 16)
	{
		ft_putnbr_hex(number / 16);
		ft_putchar_hex(number % 16);
	}
}

void init_sensor()
{
    _delay_ms(40);
    
    i2c_start();                                                // Sensor initialization : 
    i2c_write(SENSOR_ADDR << 1 | 0);                            // sending sensor adress << 1 | 0 : starting as Master Transmitter
    i2c_write(0x71);                                            // Status request
    i2c_stop();

    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 1);                            // Restarting as Master Receiver to read the sensor status
    if (!(i2c_read() & 0x08))                                   // Reading status, if bit 3 is off : calibration needed
    {
        uart_printstr("Calibration needed\r\n");
        aht_calibration();
    }    
}

void get_measurement()
{
    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 0);                            // Starting comm. with sensor as Master Transmitter
    i2c_write(0xAC);                                            // Request for measurement : 0xAC, 0x33, 0x00
    i2c_write(0x33);
    i2c_write(0x00);
    i2c_stop();
    _delay_ms(80);                                              // Waiting for the measurement to be done

    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 1);                            // Restarting as Master Receiver
    for (int i = 0; i < 7; i++)
    {
        i2c_read();                                             // Reading the measures
        uint8_t res = TWDR;
        if (res < 16)
            uart_tx('0');
        ft_putnbr_hex(res);
        uart_printstr(" ");
    }
    uart_printstr("\r\n");
    i2c_stop();
}

int main ()
{
    uart_init();
    i2c_init();
    init_sensor();
    while (1) 
        get_measurement();
}

// i2c baud = (freq_cpu / desired_freq(100khz) - 16) / (2 * prescaler)
// i2c baud = (16000000 / 100000 - 16) / (2 * 1)
// = 72