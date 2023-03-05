
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
    _show_lcd();
}

void LCD_4_5_Digits::_show_lcd(void)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        digitalWrite(_latch_pin, LOW);
        shiftOut(_data_pin, _clock_pin, MSBFIRST, _value_to_set_lcd[i]);
        digitalWrite(_latch_pin, HIGH);
    }
    digitalWrite(_data_pin, HIGH);
}

void LCD_4_5_Digits::clear(void)
{
    for (uint8_t i = 0; i < sizeof(_value_to_set_lcd); i++)
    {
        _value_to_set_lcd[i] = 0x00;
    }
    _show_lcd();
}

void LCD_4_5_Digits::all_on(void)
{
    for (uint8_t i = 0; i < sizeof(_value_to_set_lcd); i++)
    {
        _value_to_set_lcd[i] = 0xff;
    }
    _show_lcd();
}

void LCD_4_5_Digits::set_integer(int16_t value)
{
    if (_is_integer_valid(value))
    {
        _set_minus_sign_if_negative(value);
        _abs_value = abs(value);

        _value_lcd[0] = _abs_value % 10;          // units
        _value_lcd[1] = (_abs_value / 10) % 10;   // tens
        _value_lcd[2] = (_abs_value / 100) % 10;  // hundreds
        _value_lcd[3] = (_abs_value / 1000) % 10; // thousands
        _value_lcd[4] = _abs_value / 10000;       // ten thousands

        _show_value_on_display();
        _show_lcd();
    }
    else
    {
        _set_overflow();
    }
}

void LCD_4_5_Digits::set_float(float value, uint8_t decimals)
{
    /*                dec
    -1.9999 ~ 1.9999 | 4 10000 -> geen leading zero
    -19.999 ~ 19.999 | 3  1000
    -199.99 ~ 199.99 | 2   100
    -1999.9 ~ 1999.9 | 1    10
    */

    int16_t rounded = (int16_t)roundf(value * _round_multiplier[decimals]);

    if (_is_integer_valid(rounded))
    {
        _set_minus_sign_if_negative(rounded);

        _abs_value = abs(rounded);
        _value_lcd[0] = _abs_value % 10;          // units
        _value_lcd[1] = (_abs_value / 10) % 10;   // tens
        _value_lcd[2] = (_abs_value / 100) % 10;  // hundreds
        _value_lcd[3] = (_abs_value / 1000) % 10; // thousands
        _value_lcd[4] = _abs_value / 10000;       // ten thousands

        _show_value_on_display();
        _set_decimal_dot(true, decimals);

        _show_lcd();
    }
    else
    {
        _set_overflow();
    }
}

void LCD_4_5_Digits::_set_overflow(void)
{
    _value_to_set_lcd[0] = 0x00;
    for (uint8_t i = 1; i < 5; i++)
    {
        _value_to_set_lcd[i] = 0b01000000;
    }
    _show_lcd();
}

void LCD_4_5_Digits::_set_minus_sign_if_negative(int16_t value)
{
    if (value < 0)
        _value_to_set_lcd[0] |= _symbol[MINUS];
    else
        _value_to_set_lcd[0] &= ~_symbol[MINUS];
}

bool LCD_4_5_Digits::_is_integer_valid(int16_t value)
{
    if (value >= -19999 && value <= 19999)
        return true;
    return false;
}

void LCD_4_5_Digits::_removeLeadingZeros()
{
    for (uint8_t i = 1; i < 4; i++)
    {
        if (_value_to_set_lcd[i] == _number[0])
            _value_to_set_lcd[i] = 0b00000000;
        else
            break;
    }
}

void LCD_4_5_Digits::set_timer(uint16_t time)
{
    uint8_t seconds = time % 60;
    uint8_t minuts = time / 60;

    _time_lcd[0] = seconds % 10;
    _time_lcd[1] = (seconds / 10) % 10;
    _time_lcd[2] = minuts % 10;
    _time_lcd[3] = (minuts / 10) % 10;

    _set_colon_right(true);
    _show_time_on_display();
    // _set_colon_right(false);
}

void LCD_4_5_Digits::_set_colon_right(bool show)
{
    if (show)
        _value_to_set_lcd[0] |= _symbol[COLON_RIGHT];
    else
        _value_to_set_lcd[0] &= ~_symbol[COLON_RIGHT];
}

void LCD_4_5_Digits::_set_decimal_dot(bool show, uint8_t dot_position)
{
    if (show)
    {
        _value_to_set_lcd[4 - dot_position] |= _symbol[DOT];
        if (_value_to_set_lcd[3 - dot_position] == 0b00000000 && dot_position != 3)
        {
            _value_to_set_lcd[3 - dot_position] = _number[0];
        }
    }
    else
    {
        _value_to_set_lcd[4 - dot_position] &= ~_symbol[DOT];
    }

    _show_lcd();
}

void LCD_4_5_Digits::_show_time_on_display(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        _value_to_set_lcd[4 - i] = _number[_time_lcd[i]];
    }
    _show_lcd();
}

void LCD_4_5_Digits::_show_value_on_display(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        _value_to_set_lcd[4 - i] = _number[_value_lcd[i]];
    }

    if (_value_lcd[4] > 0)
    {
        _value_to_set_lcd[0] |= _symbol[NUMBER_1];
    }
    else
    {
        _value_to_set_lcd[0] &= ~_symbol[NUMBER_1];
        _removeLeadingZeros();
    }
}