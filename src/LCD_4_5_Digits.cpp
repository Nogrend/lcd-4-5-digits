
#include <LCD_4_5_Digits.h>

LCD_4_5_Digits::LCD_4_5_Digits(uint8_t latch, uint8_t clock, uint8_t data)
{
    _latch_pin = latch;
    _clock_pin = clock;
    _data_pin = data;
}

LCD_4_5_Digits::~LCD_4_5_Digits()
{
}

void LCD_4_5_Digits::init(void)
{
    pinMode(_latch_pin, OUTPUT);
    pinMode(_clock_pin, OUTPUT);
    pinMode(_data_pin, OUTPUT);
    _set_lcd();
}

void LCD_4_5_Digits::clear(void)
{
    for (uint8_t i = 0; i < sizeof(_value_to_set_lcd); i++)
    {
        _value_to_set_lcd[i] = 0x00;
    }
    _set_lcd();
}

void LCD_4_5_Digits::all_on(void)
{
    for (uint8_t i = 0; i < sizeof(_value_to_set_lcd); i++)
    {
        _value_to_set_lcd[i] = 0xff;
    }
    _set_lcd();
}

void LCD_4_5_Digits::set_value(uint16_t input_value)
{
    // 1 0 0 0 0
    //   1 0 0 0
    //     1 0 0
    //       1 0
    //         1
    uint8_t tth = 0, th = 0, h = 0, t = 0, u = 0; // ten thousands, thousands,hundreds,tens,units

    u = input_value % 10;
    t = (input_value / 10) % 10;
    h = (input_value / 100) % 10;
    th = input_value / 1000;

    _value_to_set_lcd[4] = _number[u];
    _value_to_set_lcd[3] = _number[t];
    _value_to_set_lcd[2] = _number[h];
    _value_to_set_lcd[1] = _number[th];

    if (tth > 0)
        _value_to_set_lcd[0] |= 0b00010000;
    else
        _value_to_set_lcd[0] &= ~0b00010000;

    _set_lcd();
}

void LCD_4_5_Digits::_set_lcd(void)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        digitalWrite(_latch_pin, LOW);
        shiftOut(_data_pin, _clock_pin, MSBFIRST, _value_to_set_lcd[i]);
        digitalWrite(_latch_pin, HIGH);
    }
}