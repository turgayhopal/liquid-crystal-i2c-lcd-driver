// Header: LCD I2C (Liquid Crystal) Cpp Driver Source File
// File Name: lcd_i2c.cpp
// Author: Turgay Hopal
// Date: 1.1.2024

#include "../inc/lcd_i2c.hpp"

LCD::LCD(lcd_config_t config)
{
    uint8_t tx_buffer[6]{0x00, 0x00, 0x00, 0x00, 0x00};

    this->config.i2c_device_path = config.i2c_device_path;
    this->config.slave_addr = config.slave_addr;
    this->config.lines = config.lines;
    this->config.columns = config.columns;

    this->lcd_status.display_screen_status = LCD_DISPLAY_ON;
    this->lcd_status.display_cursor_blink_status = LCD_DSIPLAY_BLINK_OFF;
    this->lcd_status.display_cursor_status = LCD_DISPLAY_CURSOR_OFF;
    this->lcd_status.entry_counter_status = LCD_ENTRY_MODE_INC;
    this->lcd_status.entry_shift_status = LCD_ENTRY_MODE_NO_SHIFT;
    this->lcd_status.shift_direction_status = LCD_SHIFT_LEFT;
    this->lcd_status.shift_mode_status = LCD_SHIFT_CURSOR;
    this->lcd_status.backlight_status = LCD_BACKLIGHT_ON;

    usleep(5000);

    tx_buffer[0] = (0x03 << 4) | LCD_E_BIT;
    tx_buffer[1] = (0x03 << 4) | LCD_E_BIT;
    tx_buffer[2] = (0x03 << 4);

    for (uint8_t i = 0; i < 3; ++i)
    {

        this->write_bytes_i2c(tx_buffer, 3);

        if (i == 2)
        {
            usleep(1000);
        }
        else
        {
            usleep(5000);
        }
    }

    tx_buffer[0] = (0x02 << 4) | LCD_E_BIT;
    tx_buffer[1] = (0x02 << 4) | LCD_E_BIT;
    tx_buffer[2] = (0x02 << 4);

    this->write_bytes_i2c(tx_buffer, 3);

    this->write_command(LCD_FUNCTION_SET, LCD_FUNCTION_5x8_DOTS | LCD_FUNCTION_2_LINE | LCD_FUNCTION_4_BIT_BUS);
    usleep(100);
    this->write_command(LCD_DISPLAY_CONTROL, LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR_OFF | LCD_DSIPLAY_BLINK_OFF);
    usleep(100);
    this->write_command(LCD_ENTRY_MODE_SET, LCD_ENTRY_MODE_INC | LCD_ENTRY_MODE_NO_SHIFT);
    usleep(100);
    this->write_command(LCD_DSIPLAY_SHIFT, LCD_SHIFT_CURSOR | LCD_SHIFT_LEFT);
    usleep(100);
    this->write_command(LCD_CLEAR_DISPLAY, LCD_NO_COMMAND);
    usleep(100);
    this->write_command(LCD_RETURN_HOME, LCD_NO_COMMAND);
    usleep(100);
}

void LCD::write_command(uint8_t reg, uint8_t command)
{
    uint8_t tx_buffer[6]{0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t data = command | reg;
    uint8_t HIGH = (data & 0xF0);
    uint8_t LOW = ((data << 4) & 0xF0);

    tx_buffer[0] = HIGH | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[1] = HIGH | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[2] = HIGH | lcd_status.backlight_status;

    tx_buffer[3] = LOW | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[4] = LOW | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[5] = LOW | lcd_status.backlight_status;

    this->write_bytes_i2c(tx_buffer, 6);
}

void LCD::write_data(uint8_t data)
{

    uint8_t tx_buffer[6]{0x00, 0x00, 0x00, 0x00, 0x00};

    uint8_t HIGH = (data & 0xF0);
    uint8_t LOW = ((data << 4) & 0xF0);

    tx_buffer[0] = HIGH | LCD_RS_BIT | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[1] = HIGH | LCD_RS_BIT | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[2] = HIGH | LCD_RS_BIT | lcd_status.backlight_status;

    tx_buffer[3] = LOW | LCD_RS_BIT | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[4] = LOW | LCD_RS_BIT | LCD_E_BIT | lcd_status.backlight_status;
    tx_buffer[5] = LOW | LCD_RS_BIT | lcd_status.backlight_status;

    this->write_bytes_i2c(tx_buffer, 6);
}

void LCD::write_bytes_i2c(uint8_t *data, uint8_t size)
{
    int file;

    if ((file = open(this->config.i2c_device_path, O_RDWR)) < 0)
    {
    }
    else
    {
        if (ioctl(file, I2C_SLAVE, this->config.slave_addr) < 0)
        {
        }
        else
        {
            if (write(file, data, size) == size)
            {
            }
        }
    }

    close(file);
}
void LCD::write_char(const char data)
{
    this->write_data(data);
}
void LCD::print_string(const char *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; ++i)
    {
        this->write_data(data[i]);
    }
}
void LCD::set_cursor_position(uint8_t column, uint8_t line)
{
    static const uint8_t lineOffsets[4] = {0x00, 0x40, 0x14, 0x54};

    uint8_t command = (column + lineOffsets[line]);

    this->write_command(LCD_SET_DDRAM_ADDRESS, command);
}
void LCD::load_custom_char(uint8_t cell, uint8_t *char_map)
{
    if (cell > 7) {
        return;
    }

    uint8_t lcdCommand = LCD_SET_CGRAM_ADDRESS | (cell << 3);

    this->write_command((uint8_t)0x00, lcdCommand);

    for (uint8_t i = 0; i < 8; ++i) {
        this->write_command(LCD_RS_BIT, char_map[i]);
    }
}
void LCD::show_cg_ram(uint8_t addr)
{
    this->write_command(LCD_RS_BIT, addr);
}
void LCD::display_on(void)
{
    lcd_status.display_screen_status |= LCD_DISPLAY_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::display_off(void)
{
    lcd_status.display_screen_status &= ~LCD_DISPLAY_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::cursor_on(void)
{
    lcd_status.display_cursor_status |= LCD_DISPLAY_CURSOR_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::cursor_off(void)
{
    lcd_status.display_cursor_status &= ~LCD_DISPLAY_CURSOR_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::blink_on(void)
{
    lcd_status.display_cursor_blink_status |= LCD_DISPLAY_BLINK_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::blink_off(void)
{
    lcd_status.display_cursor_blink_status &= ~LCD_DISPLAY_BLINK_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::backlight_on(void)
{
    lcd_status.backlight_status = LCD_BACKLIGHT_ON;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::backlight_off(void)
{
    lcd_status.backlight_status = LCD_BACKLIGHT_OFF;
	uint8_t command = lcd_status.display_screen_status | lcd_status.display_cursor_status | lcd_status.display_cursor_blink_status;
	this->write_command(LCD_DISPLAY_CONTROL, command);
    usleep(1000);
}
void LCD::cursor_shift_right(void)
{
    lcd_status.shift_direction_status |= LCD_SHIFT_RIGHT;
	lcd_status.shift_mode_status &= ~LCD_SHIFT_DISPLAY;
	uint8_t command = lcd_status.shift_direction_status | lcd_status.shift_mode_status;
	this->write_command(LCD_DSIPLAY_SHIFT, command);
    usleep(1000);
}
void LCD::cursor_shift_left(void)
{
    lcd_status.shift_direction_status &= ~LCD_SHIFT_RIGHT;
	lcd_status.shift_mode_status &= ~LCD_SHIFT_DISPLAY;
	uint8_t command = lcd_status.shift_direction_status | lcd_status.shift_mode_status;
	this->write_command(LCD_DSIPLAY_SHIFT, command);
    usleep(1000);
}
void LCD::display_shift_right(void)
{
    lcd_status.shift_direction_status |= LCD_SHIFT_RIGHT;
	lcd_status.shift_mode_status |= LCD_SHIFT_DISPLAY;
	uint8_t command = lcd_status.shift_direction_status | lcd_status.shift_mode_status;
	this->write_command(LCD_DSIPLAY_SHIFT, command);
    usleep(1000);
}
void LCD::display_shift_left(void)
{
    lcd_status.shift_direction_status &= ~LCD_SHIFT_RIGHT;
	lcd_status.shift_mode_status |= LCD_SHIFT_DISPLAY;
	uint8_t command = lcd_status.shift_direction_status | lcd_status.shift_mode_status;
	this->write_command(LCD_DSIPLAY_SHIFT, command);
    usleep(1000);
}

void LCD::clear_display(void)
{
    this->write_command(LCD_RETURN_HOME, LCD_NO_COMMAND);
    usleep(10000);
    this->write_command(LCD_CLEAR_DISPLAY, LCD_NO_COMMAND);
    usleep(10000);
}
void LCD::return_home(void)
{
    this->write_command(LCD_CLEAR_DISPLAY, LCD_NO_COMMAND);
    usleep(10000);
}
