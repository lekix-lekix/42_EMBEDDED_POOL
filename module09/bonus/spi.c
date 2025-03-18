#include "./include.h"

void SPI_send(char data)
{
    SPDR = data;
    while (!(SPSR & (1 << SPIF))) {}
}

void SPI_rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness)
{
    SPI_send(0xE0 | (brightness & 0x1F));
    SPI_send(b);
    SPI_send(g);
    SPI_send(r);
}

void init_SPI(void)
{
    DDR_SPI = (1 << MOSI | 1 << SCK | 1 << SS);                 // MOSI = Master Out Slave In
                                                                // SCK = Serial Clock
                                                                // SS = Slave Select
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1);              // SPE = SPI Enable
                                                                // MSTR = Master Enabled
                                                                // SPRO = SPI clock Rate
    DDRB |= (1 << SS);
}

void set_one_led(int led_idx, uint32_t color, int brightness)
{
    uint8_t r = (color >> 16) & 0xFF;  // R = 0xFF  
    uint8_t g = (color >> 8)  & 0xFF;  // G = 0x88  
    uint8_t b = (color >> 0)  & 0xFF;  // B = 0x00

    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
    {
        if (i == led_idx)
            SPI_rgb(r, g, b, brightness);
        else
            SPI_rgb(0, 0, 0, brightness);
    }
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}

void set_all_leds(uint8_t r, uint8_t g, uint8_t b, int brightness)
{
    for (int i = 0; i < 4; i++)
        SPI_send(0x00);
    for (int i = 0; i < 3; i++)
        SPI_rgb(r, g, b, brightness);
    for (int i = 0; i < 4; i++)
        SPI_send(0xFF);
}
