#ifndef __ILI93XX_H__
#define __ILI93XX_H__
#include <common/include.h>


struct ili93xx_operations{
    void (*write_reg)(pi_uint16_t value);
    void (*write_data)(pi_uint16_t value);
    pi_uint16_t (*read_data)(void);
    void (*backlight_control)(int state);
};

struct ili93xx{
    int dir;
    pi_uint16_t width;
    pi_uint16_t height;
    pi_uint16_t setxcmd;
    pi_uint16_t setycmd;
    pi_uint16_t wramcmd;
    const struct ili93xx_operations* ops;
};


void ili93xx_write_reg(struct ili93xx* ili93xx, pi_uint16_t value);

void ili93xx_write_data(struct ili93xx* ili93xx, pi_uint16_t value);

pi_uint16_t ili93xx_read_data(struct ili93xx* ili93xx);

void ili93xx_write_register(struct ili93xx* ili93xx, pi_uint16_t reg, pi_uint16_t value);

pi_uint16_t ili93xx_read_register(struct ili93xx* ili93xx, pi_uint16_t reg);

void ili93xx_writeram_prepare(struct ili93xx* ili93xx);

void ili93xx_set_cursor(struct ili93xx* ili93xx, pi_uint16_t x, pi_uint16_t y);

void ili93xx_fill(struct ili93xx* ili93xx, pi_uint16_t x1, pi_uint16_t y1, pi_uint16_t x2, pi_uint16_t y2, pi_uint16_t color);

void ili93xx_set_dir(struct ili93xx* ili93xx, pi_uint8_t dir);

void ili93xx_init(struct ili93xx* ili93xx);
#endif