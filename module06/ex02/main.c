/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:32:10 by kipouliq          #+#    #+#             */
/*   Updated: 2025/03/15 11:55:25 by kipouliq         ###   ########.fr       */
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
    TWCR = (1 << TWEN); // starting the TWI module (i2c)
}

void i2c_write(unsigned char data)
{
    TWDR = data;                                      // Writing into the TWI Data Register
    TWCR = (1 << TWINT | 1 << TWEN);
    while (!(TWCR & (1 << TWINT))) {}
}

void i2c_read(void)
{
    TWCR = (1 << TWINT | 1 << TWEN | 1 << TWEA);        // TWEA : asking for ACK
    while (!(TWCR & (1 << TWINT))) {}                   // Reading from TWI Data Register
}

void i2c_start(void)
{                                                      // TWINT = TWI interrupt flag, when set starts the the operation of the TWI
    TWCR = (1 << TWSTA | 1 << TWINT | 1 << TWEN);      // TWSTA = checks if bus is available and generate a START condition if so
    while (!(TWCR & (1 << TWINT))) {}                  // TWSR = TWI status register
}                                                      // TWEN = starts thw TWI module

void i2c_stop(void)
{
    TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);  // TWSTO : TWI stop to send at the end of communication
}

uint8_t i2c_getstatus(void)
{
    return (TWSR & 0xF8);
}

void aht_calibration(void)
{
    i2c_stop();
    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 0);
    i2c_write(0xBE);                                    // Calibration command (0xBE, 0x08, 0x00)
    i2c_write(0x08);
    i2c_write(0x00);
}

void	ft_putchar(char c)
{
	uart_tx(c);
}

void	ft_putnbr(uint32_t nb)
{
	long int	number;

	number = nb;
	if (number < 0)
	{
		number = -number;
		ft_putchar('-');
	}
	if (number >= 0 && number <= 9)
		ft_putchar(number + '0');
	if (number > 9)
	{
		ft_putnbr(number / 10);
		ft_putchar(number % 10 + '0');
	}
}

void trigger_measurement(void)
{
    i2c_stop();
    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 0);    // Trigger measurement (0xAC, 0x33, 0x00) as Master Transmitter (write (= 0 ) in last bit)
    i2c_write(0xAC);                    // Need to wait 80 ms according to AHT20 datasheet  
    i2c_write(0x33);                        
    i2c_write(0x00);
    _delay_ms(80);
}

void write_measures(float temp, float humid)
{
    float humidity = (humid * 100.0f) / 1048576.0f;                 // Formulas in the datasheet
    float temperature = (temp * 200.0f / 1048576.0f) - 50.0f;
    int virg = (int)((temperature - (int)temperature) * 10); 

    uart_printstr("Temperature: ");
    ft_putnbr((int)temperature);
    uart_tx('.');
    ft_putnbr(virg);
    uart_printstr("°C, Humidity: ");
    ft_putnbr((int)humidity);
    uart_printstr("% \r\n");
}

uint32_t get_temp(uint8_t *data)
{
    uint32_t temp = ((uint32_t)(data[2] & 0x0F)) << 16 | ((uint32_t)data[3] << 8) | data[4];
    return (temp);                          // datasheet says 20 bits each per measure
}

uint32_t get_humid(uint8_t *data)
{
    uint32_t humid = ((uint32_t)data[0] << 12 | (uint32_t)data[1] << 4 | ((uint32_t)data[2] & 0xF0));
    return (humid);
}

void init_sensor()
{
    _delay_ms(40);
    
    i2c_start();
    i2c_write(SENSOR_ADDR << 1);
    i2c_write(0x71);
    i2c_stop();

    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 1);
    i2c_read();
    if (!(TWDR & 0x08))
    {
        uart_printstr("Calibration needed\r\n");
        aht_calibration();
    }
}

void get_measurements(float *temp, float *humid)
{
    uint8_t data[5];
    
    i2c_start();
    i2c_write(SENSOR_ADDR << 1);
    i2c_write(0xAC);
    i2c_write(0x33);               // asking for measurement
    i2c_write(0x00);
    i2c_stop();
    _delay_ms(80);

    i2c_start();
    i2c_write(SENSOR_ADDR << 1 | 1);
    for (int i = 0; i < 6; i++)
    {
        i2c_read();
        if (i == 0)
        {
            TWDR = 0;           // first byte (status) ignored
            continue;
        }
        uint8_t res = TWDR;
        data[i - 1] = res;
    }
    *temp += get_temp(data);
    *humid += get_humid(data);
}

void    get_print_measures()
{
    float temp = 0;
    float humid = 0;

    for (int i = 0; i < 3; i++)
        get_measurements(&temp, &humid);
    write_measures(temp / 3.0f, humid / 3.0f);
}

int main ()
{    
    uart_init();
    i2c_init();
    init_sensor();
    
    while (1)
    {
        get_print_measures();
        _delay_ms(100);
    }
}

// i2c baud = (freq_cpu / desired_freq(100khz) - 16) / (2 * prescaler)
// i2c baud = (16000000 / 100000 - 16) / (2 * 1)
// = 72