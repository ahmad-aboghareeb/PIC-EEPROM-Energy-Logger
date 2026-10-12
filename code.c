#include <16F877A.h>
#use delay(crystal=16000000)

#define LCD_RS PIN_D0
#define LCD_RW PIN_D1
#define LCD_E  PIN_D2
#define LCD_D4 PIN_D4
#define LCD_D5 PIN_D5
#define LCD_D6 PIN_D6
#define LCD_D7 PIN_D7

void lcd_pulse_enable() {
    output_high(LCD_E);
    delay_us(2);
    output_low(LCD_E);
    delay_us(50);
}

void lcd_send_nibble(int8 nibble) {
    output_low(LCD_D4);
    output_low(LCD_D5);
    output_low(LCD_D6);
    output_low(LCD_D7);
    if(bit_test(nibble, 0)) output_high(LCD_D4); 
    if(bit_test(nibble, 1)) output_high(LCD_D5); 
    if(bit_test(nibble, 2)) output_high(LCD_D6); 
    if(bit_test(nibble, 3)) output_high(LCD_D7); 
    lcd_pulse_enable();
}

void lcd_send_byte(int8 address, int8 data) {
    if(address) output_high(LCD_RS); else output_low(LCD_RS);
    output_low(LCD_RW); 
    lcd_send_nibble(data >> 4);
    lcd_send_nibble(data & 0x0F);
    if(address == 0) delay_ms(2); 
}

void lcd_init_custom() {
    output_low(LCD_RS);
    output_low(LCD_RW);
    output_low(LCD_E);
    delay_ms(50); 
    lcd_send_nibble(0x03);
    delay_ms(5);
    lcd_send_nibble(0x03);
    delay_us(150);
    lcd_send_nibble(0x03);
    delay_us(150);
    lcd_send_nibble(0x02);
    delay_us(150); 
    lcd_send_byte(0, 0x28); 
    lcd_send_byte(0, 0x0C); 
    lcd_send_byte(0, 0x01); 
    lcd_send_byte(0, 0x06); 
}

void lcd_print(char* str) {
    while(*str) lcd_send_byte(1, *str++);
}

int32 total_energy = 0;
short save_flag = 0;

#INT_EXT
void ext_isr() {
    save_flag = 1; 
}

void save_energy() {
    write_eeprom(0, make8(total_energy, 0));
    write_eeprom(1, make8(total_energy, 1));
    write_eeprom(2, make8(total_energy, 2));
    write_eeprom(3, make8(total_energy, 3));
}

void load_energy() {
    total_energy = make32(read_eeprom(3), read_eeprom(2), read_eeprom(1), read_eeprom(0));
    
    if (total_energy == 0xFFFFFFFF) {
        total_energy = 0;
    }
}

void main() {
    char buffer[32];

    delay_ms(100);
    lcd_init_custom();
    
    load_energy(); 

    ext_int_edge(H_TO_L);
    enable_interrupts(INT_EXT);
    enable_interrupts(GLOBAL);

    while(TRUE) {
        if (save_flag) {
            save_energy();
            lcd_send_byte(0, 0x01);
            sprintf(buffer, "EMERGENCY SAVE!");
            lcd_print(buffer);
            delay_ms(1500);
            save_flag = 0;
        } else {
            total_energy++; 
            
            lcd_send_byte(0, 0x80);
            sprintf(buffer, "Energy Logger:");
            lcd_print(buffer);
            
            lcd_send_byte(0, 0xC0);
            sprintf(buffer, "%Lu kWh      ", total_energy); 
            lcd_print(buffer);
            
            delay_ms(250); 
        }
    }
}
