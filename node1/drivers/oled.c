#include "oled.h"
#include "SPI.h"
#include <avr/io.h>

#define OLED_CS  SS1                 // lav = kommando, høy = data
#define IO_CS  SS2                 // displayet sitter på SS2
#define OLED_DC PB1

static uint8_t current_line = 0;
static uint8_t current_col  = 0;

void oled_command(uint8_t cmd)
{
    PORTB &= ~(1<<OLED_DC);
    SPI_Transmit(cmd, SS1);
}

void oled_data(uint8_t data)
{
    PORTB |= (1<<OLED_DC);
    SPI_Transmit(data, SS1);
}

void oled_clear(void)
{
    for (uint16_t i = 0; i < 1024; i++) {
        OLED_SRAM[i] = 0x00;
    }
}

void oled_init(void)
{
    oled_command(CMD_SEGMENT_REMAP_127);          
    oled_command(CMD_COM_SCAN_DIRECTION_REMAP); 

    oled_clear();                    // nullstill SRAM-bufferen
    oled_update();                   // send nullene til displayet
    oled_command(CMD_INVERSE_DISPLAY);
    oled_command(CMD_DISPLAY_ON);    // skru på til slutt
}

void oled_goto_line(uint8_t line)
{
    current_line = line & 0x07; //page nummer, max 7
    oled_command(0xB0 | current_line); //Sett page, page nummer
}

void oled_goto_column(uint8_t col)
{
    current_col = col & 0x7F; // 0-127
    oled_command(0x00 | (current_col & 0x0F)); //sette nedre fire bit
    oled_command(0x10 | (current_col >> 4)); // 
}

void oled_pos(uint8_t line, uint8_t col)
{
    oled_goto_line(line);
    oled_goto_column(col);
}

void oled_clear_line(uint8_t line)   // tømmer en page i bufferen
{
    if (line >= 8) return;
    for (uint8_t col = 0; col < 128; col++) {
        OLED_SRAM[line * 128 + col] = 0x00;
    }
}

void oled_update(void)
{
    for (uint8_t page = 0; page < 8; page++) {
        oled_pos(page, 0);
        for (uint8_t col = 0; col < 128; col++) {
            oled_data(OLED_SRAM[page * 128 + col]);
        }
    }
}

void oled_buf_write(uint8_t page, uint8_t col, uint8_t data)
{
    if (page >= 8 || col >= 128) return;
    OLED_SRAM[page * 128 + col] = data;
}

void oled_buf_set_pixel(uint8_t x, uint8_t y)
{
    if (x >= 128 || y >= 64) return;
    OLED_SRAM[(y / 8) * 128 + x] |= (1 << (y % 8));
}

void oled_print_char(uint8_t page, uint8_t col, char c)
{
    if (c < 32 || c > 126) c = '?';              // ukjent tegn
    uint8_t idx = c - 32;

    for (uint8_t i = 0; i < 8; i++) {
        oled_buf_write(page, col + i, pgm_read_byte(&font8[idx][i]));
    }
}

void oled_print(uint8_t page, uint8_t col, const char *s)
{
    while (*s && col <= 120) {                   // 16 tegn per linje
        oled_print_char(page, col, *s++);
        col += 8;
    }
}

void oled_home(void){
    oled_print(1, 16, "1.Normal mode");
    oled_print(2, 16, "2.Easy mode");
    oled_print(3, 16, "3.Diff mode");
    oled_print(4, 16, "4.Expert mode");
    oled_update();
}


static Direction  dir  = NEUTRAL;
static Game game = NORMAL;

void oled_update_select(Game old) {
    oled_print(old + 1, 0, "  ");   // fjern gammel pil
    oled_print(game + 1, 0, "=>");  // tegn ny pil
    oled_update();
}

void check_position(void) {
    uint8_t values[ADC_NUM_CHANNELS];
    adc_max156_convert_and_read(values);

    Game old = game;

    if (values[2] > 75) {
        dir = UP;
        if (NORMAL < game) {
            game -= 1;
        } else {
            game = EXPERT;
        }
    } else if (values[2] < 25) {
        dir = DOWN;
        if (game < EXPERT) {
            game += 1;
        } else {
            game = NORMAL;
        }
    } else {
        dir = NEUTRAL;
    }

    if (game != old) {
        oled_update_select(old);
    }
}

Direction get_dir(void) {
    return dir;
}

Game get_game(void) {
    return game;
}


uint8_t menu_select(void) {
    oled_clear();
    oled_home();
    oled_print(get_game() + 1, 0, "->");
    oled_update();

    button_pressed = 0;

    while (!button_pressed) {
        check_position();
        _delay_ms(200);
    }

    button_pressed = 0;
    return get_game();
}