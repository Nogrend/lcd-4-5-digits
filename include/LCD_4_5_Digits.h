#ifndef Button_Handler_h
#define Button_Handler_h

#include <Arduino.h>

// defines

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

    uint8_t _tth = 0, _th = 0, _h = 0, _t = 0, _u = 0; // ten thousands, thousands,hundreds,tens,units

    uint8_t _value_to_set_lcd[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    uint16_t _abs_value;

    void _set_lcd(void);
    void _set_overflow(void);
    void _set_minus_sign_if_negative(int16_t value);
    bool _is_value_valid(int16_t value);
    void _remove_unnecessary_zeros(void);

public:
    LCD_4_5_Digits(uint8_t latch, uint8_t clock, uint8_t data);
    ~LCD_4_5_Digits();
    void init(void);
    void clear(void);
    void all_on(void);
    void set_value(int16_t input_value = 0);
};

#endif