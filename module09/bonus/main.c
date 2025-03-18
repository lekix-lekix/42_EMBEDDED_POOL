#include "./include.h"

int bright_value = 0;

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

void expander_set_register(int exp_register, int state)
{
    i2c_start();
    i2c_write(0x40); 
    i2c_write(exp_register);
    i2c_write(state);
    i2c_stop();
}

void light_segment(int state, int seg_char)
{
    expander_set_register(CONFIG_REG_0, state);
    expander_set_register(OUTPUT_REG_0, state);
    expander_set_register(OUTPUT_REG_1, seg_char);
    
    _delay_ms(1);
    expander_set_register(CONFIG_REG_0, state);
    expander_set_register(OUTPUT_REG_1, 0x00);
    _delay_ms(1);

}

void    light_str(char *str, int time)
{
    int screen_tab[] = {SCREEN_0, SCREEN_1, SCREEN_2, SCREEN_3};
    int j = -1;
    int len = ft_strlen(str);

    if (ft_strlen(str) == 1)
    {
        for (int i = 0; i < time; i++)
            light_segment(SCREEN_0, char_to_seg(str[0]));
        return ;
    }

    while (++j < time)
    {
        for (int i = 0; i < len; i++)
            light_segment(screen_tab[i], char_to_seg(str[i]));
    }
}

void light_portb_leds()
{
    int leds[] = {PB0, PB1, PB2, PB4};

	DDRB = (1 << PB0 | 1 << PB1 | 1 << PB2 | 1 << PB4); // initializing the leds as output
    for (int i = 0; i < 4; i++)
    {
        PORTB = (1 << leds[i]);
        _delay_ms(40);
    }
    for (int i = 3; i >= 0; i--)
    {
        PORTB = (1 << leds[i]);
        _delay_ms(40);
    }
}

void light_i2c_leds()
{
    int D11 = 0b11111101;
    int D10 = 0b11111011;
    int D9 = 0b11110111;
    int leds[] = {D11, D10, D9};

    i2c_init();

    for (int i = 0; i < 3; i++)
    {
        expander_set_register(CONFIG_REG_0, leds[i]);
        expander_set_register(OUTPUT_REG_0, leds[i]);
        _delay_ms(40);
    }
    for (int i = 2; i >= 0; i--)
    {
        expander_set_register(CONFIG_REG_0, leds[i]);
        expander_set_register(OUTPUT_REG_0, leds[i]);
        _delay_ms(40);
    }
    expander_set_register(CONFIG_REG_0, 0xFF);
}

void light_spi_leds_fwd(uint32_t color)
{
    init_SPI();

    for (int i = 0; i < 3; i++)
    {
        set_one_led(i, color, 10);
        if (i < 2)
            _delay_ms(70);
    }
    for (int i = 0; i < 3; i++)
        set_one_led(i, 0x000000, 10);
}

void light_spi_leds_bwd(uint32_t color)
{
    init_SPI();

    for (int i = 3; i >= 0; i--)
    {
        set_one_led(i, color, 10);
        _delay_ms(70);
    }
    for (int i = 0; i < 3; i++)
        set_one_led(i, 0x000000, 10);
}

uint32_t get_color(uint16_t r, uint16_t g, uint16_t b)
{
    return (((uint32_t)r & 0xFF) << 16) | (((uint32_t)g & 0xFF) << 8) | (((uint32_t)b & 0xFF) << 0);
}


void spi_leds_colors()
{
    int tab[6][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {0, 1, 1}, {1, 0, 1}};

    for (int i = 0; i < 6; i++)
    {
        if (i % 2)
            light_spi_leds_bwd(get_color(tab[i][0] * 255, tab[i][1] * 255, tab[i][2] * 255));
        else
            light_spi_leds_fwd(get_color(tab[i][0] * 255, tab[i][1] * 255, tab[i][2] * 255));
    }
}

void wheel(uint8_t pos) 
{
    pos = 255 - pos;
    if (pos < 85) 
        set_all_leds(255 - pos * 3, 0, pos * 3, bright_value);
    else if (pos < 170) 
    {
        pos = pos - 85;
        set_all_leds(0, pos * 3, 255 - pos * 3, bright_value);
    }
    else
    {
        pos = pos - 170;
        set_all_leds(pos * 3, 255 - pos * 3, 0, bright_value);
    }
}

void launch_rainbow()
{
    init_SPI();

    TCCR1B = (1 << WGM13 /* | 1 << CS11 | 1 << CS10  */| 1 << CS12 );
    ICR1 = 65535;
    for (int i = 0; i < 5000; i++)
        wheel(TCNT1 >> 8);
    set_all_leds(0, 0 ,0, 0);
}


void setup_timer0(int prescaler) 
{
    // Mettre TIMER0 en mode "Phase Correct PWM"
    TCCR0A = 0;   // Mode normal (phase correct = WGM02:0 = 1)
    TCCR0B = prescaler;  // Prescaler 64 → fréquence correcte

    TIMSK0 |= (1 << TOIE0);  // Activer l'interruption Overflow
    sei();  // Activer les interruptions globales
}

// Interruption TIMER0 Overflow (quand le compteur dépasse 255 et repart de 0)
ISR(TIMER0_OVF_vect) {
    bright_value = (bright_value + 1) %31;  // Incrémente et remet à 0 après 31
}

int main ()
{
    int state = 0b01111111;

    i2c_init();
    expander_set_register(CONFIG_REG_0, state);
    expander_set_register(OUTPUT_REG_0, state);
    expander_set_register(CONFIG_REG_1, 0x00);
    while (1)
    {
        setup_timer0(CS00);
        light_str("BYE", 100);
        light_str("42  ", 100);
        light_str("EBDD", 100);
        light_str("POOL", 100);
        light_str("C T ", 100);
        light_str("COOL", 100);
        _delay_ms(200);
        for (int i = 0; i < 3; i++)
            light_portb_leds();
        PORTB = 0;
        for (int i = 0; i < 4; i++)
            light_i2c_leds();
        for (int i = 0; i < 2; i++)
            spi_leds_colors();
        _delay_ms(100);
        setup_timer0(1 << CS02 | 1 << CS00);
        launch_rainbow();
        _delay_ms(500);
        setup_timer0(1 << CS02);
        _delay_ms(200);
        launch_rainbow();
        light_str("C1AO", 400);
        _delay_ms(500);

        // light_str("HEY", 100);
        // for (int j = 0; j < 85; j++)
        // {
        //     light_segment(SCREEN_0, SEG_I);
        // }

        // for (int j = 0; j < 85; j++)
        // {
        //     light_segment(SCREEN_0, SEG_L);
        //     light_segment(SCREEN_1, SEG_O);
        //     light_segment(SCREEN_2, SEG_V);
        //     light_segment(SCREEN_3, SEG_E);
        // }
        //     for (int j = 0; j < 85; j++)
        // {
        //     light_segment(SCREEN_0, SEG_G);
        //     light_segment(SCREEN_1, SEG_A);
        //     light_segment(SCREEN_2, SEG_G);
        //     light_segment(SCREEN_3, SEG_A);
        // }
    }
}