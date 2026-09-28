#include <avr/io.h>
#include <stdint.h>
#include "fonts.h"
#include "input_pos.h"
#include <util/delay.h>
#define OLED_SRAM_BASE   0x1400
#define OLED_PAGE_SIZE   128                        // bytes per page (128 kolonner)
#define OLED_SRAM        ((volatile uint8_t *)OLED_SRAM_BASE)


typedef enum Direction {
    DOWN = 0, UP, RIGHT, LEFT, NEUTRAL
} Direction;

typedef enum Game {
    NORMAL = 0, EASY, DIFF, EXPERT
} Game;

/* ---------- Funksjoner mot displayet (SPI) ---------- */
void oled_command(uint8_t cmd);
void oled_data(uint8_t data);
void oled_init(void);
void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t col);
void oled_pos(uint8_t line, uint8_t col);
void oled_update(void);                 // kopierer SRAM-bufferen til displayet

/* ---------- Funksjoner mot bufferen (SRAM) ---------- */
void oled_clear(void);
void oled_clear_line(uint8_t line);
void oled_buf_write(uint8_t page, uint8_t col, uint8_t data);
void oled_buf_set_pixel(uint8_t x, uint8_t y);
void oled_buf_clear_pixel(uint8_t x, uint8_t y);
void oled_font(char c);
void oled_print_char(uint8_t page, uint8_t col, char c);
void oled_print(uint8_t page, uint8_t col, const char *s);
void oled_home(void);
void check_position(void);
Direction get_dir(void);
Game get_game(void);
void oled_update_select(Game old);
uint8_t menu_select(void);


#define PAGE0  0x1400
#define PAGE1  0x1480
#define PAGE2  0x1500
#define PAGE3  0x1580
#define PAGE4  0x1600
#define PAGE5  0x1680
#define PAGE6  0x1700
#define PAGE7  0x1780


/* OLED command defines */
#define CMD_SEGMENT_REMAP_0              0xA0 
#define CMD_SEGMENT_REMAP_127            0xA1 
#define CMD_COM_PINS                     0xDA 
#define COM_PINS_RESET                   0x12 
#define CMD_COM_SCAN_DIRECTION_NORMAL    0xC0 
#define CMD_COM_SCAN_DIRECTION_REMAP     0xC8 
#define CMD_MULTIPLEX_RATIO              0xA8 
#define CMD_DISPLAY_CLOCK_DIVIDE_RATIO   0xD5 
#define DISPLAY_CLOCK_DIVIDE_RESET       0x80 
#define CMD_CONTRAST_CONTROL             0x81
#define CONTRAST_CONTROL_RESET           0x7F
#define CMD_PRE_CHARGE_PERIOD            0xD9
#define PRE_CHARGE_PERIOD_RESET          0x22
#define CMD_MEMORY_ADDRESSING_MODE       0x20
#define MEMORY_ADDRESSING_MODE_RESET     0x02
#define CMD_VCOMH_DESELECT_LEVEL         0xDB
#define VCOMH_DESELECT_LEVEL_RESET       0x20
#define CMD_NOP                          0xE3
#define CMD_IREF_SELECTION               0xAD
#define IREF_EXTERNAL                    0x00
#define IREF_INTERNAL                    0x10
#define CMD_ENTIRE_DISPLAY_ON            0xA4
#define CMD_ENTIRE_DISPLAY_ON_IGNORE_RAM 0xA5
#define CMD_NORMAL_DISPLAY               0xA6
#define CMD_INVERSE_DISPLAY              0xA7
#define CMD_DISPLAY_OFF                  0xAE
#define CMD_DISPLAY_ON                   0xAF