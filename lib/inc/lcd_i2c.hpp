// Header: LCD I2C (Liquid Crystal) Cpp Driver Header File
// File Name: lcd_i2c.hpp
// Author: Turgay Hopal
// Date: 1.1.2024

#include <iostream>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// INSRUCTIONS

constexpr uint8_t LCD_NO_COMMAND = 0x00;
constexpr uint8_t LCD_CLEAR_DISPLAY = 0x01;
constexpr uint8_t LCD_RETURN_HOME = 0x02;
constexpr uint8_t LCD_ENTRY_MODE_SET = 0x04;
constexpr uint8_t LCD_DISPLAY_CONTROL = 0x08;
constexpr uint8_t LCD_DSIPLAY_SHIFT = 0x10;
constexpr uint8_t LCD_FUNCTION_SET = 0x20;
constexpr uint8_t LCD_SET_CGRAM_ADDRESS = 0x40;
constexpr uint8_t LCD_SET_DDRAM_ADDRESS = 0x80;

// ENTRY MODES

constexpr uint8_t LCD_ENTRY_MODE_DEC = 0x00;
constexpr uint8_t LCD_ENTRY_MODE_NO_SHIFT = 0x00;
constexpr uint8_t LCD_ENTRY_MODE_SHIFT = 0x01;
constexpr uint8_t LCD_ENTRY_MODE_INC = 0x02;

// DISPLAY ON OFF CONTROL

constexpr uint8_t LCD_DISPLAY_OFF = 0x00;
constexpr uint8_t LCD_DISPLAY_CURSOR_OFF = 0x00;
constexpr uint8_t LCD_DSIPLAY_BLINK_OFF = 0x00;
constexpr uint8_t LCD_DISPLAY_BLINK_ON = 0x01;
constexpr uint8_t LCD_DISPLAY_CURSOR_ON = 0x02;
constexpr uint8_t LCD_DISPLAY_ON = 0x04;

// CURSOR DISPLAY SHIFT

constexpr uint8_t LCD_SHIFT_CURSOR = 0x00;
constexpr uint8_t LCD_SHIFT_LEFT = 0x00;
constexpr uint8_t LCD_SHIFT_RIGHT = 0x04;
constexpr uint8_t LCD_SHIFT_DISPLAY = 0x08;

// FUNCTION SET

constexpr uint8_t LCD_FUNCTION_5x8_DOTS = 0x00;
constexpr uint8_t LCD_FUNCTION_1_LINE = 0x00;
constexpr uint8_t LCD_FUNCTION_4_BIT_BUS = 0x00;
constexpr uint8_t LCD_FUNCTION_5x11_DOTS = 0x04;
constexpr uint8_t LCD_FUNCTION_2_LINE = 0x08;
constexpr uint8_t LCD_FUNCTION_8_BIT_BUS = 0x10;

constexpr uint8_t LCD_RS_BIT = 0x01;
constexpr uint8_t LCD_RW_BIT = 0x02;
constexpr uint8_t LCD_E_BIT = 0x04;

constexpr uint8_t LCD_BACKLIGHT_ON = 0x08;
constexpr uint8_t LCD_BACKLIGHT_OFF = 0x00;

typedef struct
{
    const char *i2c_device_path;
    uint8_t slave_addr;
    uint8_t lines;
    uint8_t columns;
} lcd_config_t;

typedef struct {
	uint8_t display_screen_status;
	uint8_t display_cursor_status;
	uint8_t display_cursor_blink_status;
	uint8_t entry_counter_status;
	uint8_t entry_shift_status;
	uint8_t shift_mode_status;
	uint8_t shift_direction_status;
	uint8_t backlight_status;
} lcd_status_t;

class LCD
{
private:
    void write_bytes_i2c(uint8_t *data, uint8_t size);
    void write_command(uint8_t reg, uint8_t command);
    void write_data(uint8_t data);
public:
    
    lcd_config_t config;
    lcd_status_t lcd_status;
    LCD(lcd_config_t config);

    void write_char(uint8_t data);
    void print_string(const char *data, uint8_t length);
    void set_cursor_position(const char column, uint8_t line);

    void display_on(void);
    void display_off(void);
    void cursor_on(void);
    void cursor_off(void);
    void blink_on(void);
    void blink_off(void);
    void cursor_shift_right(void);
    void cursor_shift_left(void);
    void display_shift_right(void);
    void display_shift_left(void);
    void backlight_on(void);
    void backlight_off(void);

    void clear_display(void);
    void return_home(void);
};
