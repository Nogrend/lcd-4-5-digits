
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

void LCD_4_5_Digits::set_value(int16_t input_value)
{
    if (_is_value_valid(input_value))
    {
        _set_minus_sign_if_negative_value(input_value);

        _abs_value = abs(input_value);

        _u = _abs_value % 10;
        _t = (_abs_value / 10) % 10;
        _h = (_abs_value / 100) % 10;
        _th = (_abs_value / 1000) % 10;
        _tth = _abs_value / 10000;

        _value_to_set_lcd[4] = _number[_u];
        _value_to_set_lcd[3] = _number[_t];
        _value_to_set_lcd[2] = _number[_h];
        _value_to_set_lcd[1] = _number[_th];

        if (_tth > 0)
        {
            _value_to_set_lcd[0] |= 0b00010000;
        }
        else
        {
            _value_to_set_lcd[0] &= ~0b00010000;
            _remove_excess_zeros();
        }
    }
    else
    {
        _set_overflow();
    }

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
    digitalWrite(_data_pin, HIGH);
}

void LCD_4_5_Digits::_set_overflow(void)
{
    _value_to_set_lcd[0] = 0x00;
    for (uint8_t i = 1; i < 5; i++)
    {
        _value_to_set_lcd[i] = 0b01000000;
    }
}

void LCD_4_5_Digits::_set_minus_sign_if_negative_value(int16_t value)
{
    if (value < 0)
        _value_to_set_lcd[0] |= 0b00000001;
    else
        _value_to_set_lcd[0] &= ~0b00000001;
}

bool LCD_4_5_Digits::_is_value_valid(int16_t value)
{
    if (value >= -19999 && value <= 19999)
        return true;
    return false;
}

void LCD_4_5_Digits::_remove_excess_zeros(void)
{
    for (uint8_t i = 1; i < 4; i++)
    {
        if (_value_to_set_lcd[i] == _number[0])
            _value_to_set_lcd[i] = 0b00000000;
        else
            break;
    }
}