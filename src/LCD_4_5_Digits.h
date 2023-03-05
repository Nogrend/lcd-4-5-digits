#ifndef LCD_4_5_Digits_h
#define LCD_4_5_Digits_h

#include <Arduino.h>

// defines
#define MINUS 0
#define COLON_LEFT 1
#define COLON_MIDDLE 2
#define COLON_RIGHT 3
#define NUMBER_1 4
#define LOBAT 5
#define DOT 6

class LCD_4_5_Digits
{
private:
    uint8_t _latch_pin;
    uint8_t _clock_pin;
    uint8_t _data_pin;

    uint8_t _number[10] = {
        0b00111111, // "0"
        0b00000110, // "1"
        0b01011011, // "2"
        0b01001111, // "3"
        0b01100110, // "4"
        0b01101101, // "5"
        0b01111101, // "6"
        0b00000111, // "7"
        0b01111111, // "8"
        0b01101111  // "9"
    };

    uint8_t _symbol[7] = {
        0b00000001, // "-"
        0b00000010, // ":" (left)
        0b00000100, // ":" (middle)
        0b00001000, // ":" (right)
        0b00010000, // "1"
        0b01000000, // "LOBAT"
        0b10000000  // "."
    };

    uint8_t _value_lcd[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t _time_lcd[4] = {0x00, 0x00, 0x00, 0x00};

    uint8_t _value_to_set_lcd[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint16_t _abs_value;

    uint16_t _round_multiplier[5] = {1, 10, 100, 1000, 10000};

    void _show_lcd(void);
    void _show_time_on_display(void);
    void _show_value_on_display(void);
    void _show_float_on_display(void);
    void _set_overflow(void);
    void _set_minus_sign_if_negative(int16_t value);
    bool _is_integer_valid(int16_t value);
    void _removeLeadingZeros();
    void _set_colon_right(bool show = true);
    void _set_decimal_dot(uint8_t dot_position = 1);

public:
    LCD_4_5_Digits(uint8_t latch, uint8_t clock, uint8_t data);
    ~LCD_4_5_Digits();
    void init(void);
    void clear(void);
    void all_on(void);
    void set_integer(int16_t value = 0);
    void set_float(float value = 0.0, uint8_t decimals = 1);
    void set_timer(uint16_t time = 0);
};

#endif