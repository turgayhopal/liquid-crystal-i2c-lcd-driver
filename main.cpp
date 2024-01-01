#include <iostream>
#include "lib/inc/lcd_i2c.hpp"
#include <cstring>

int main(int argc, char const *argv[])
{
    lcd_config_t config = {"/dev/i2c-1", 0x3F, 2, 16};
    LCD lcd(config);

    int counter = 0;
    char data[16];
    
    while (1) {
        lcd.clear_display();
        sprintf(data, "C : %d", counter);
        lcd.print_string((uint8_t *) data, std::strlen(data));
        usleep(100);
        counter++;
    }

    return 0;
}
