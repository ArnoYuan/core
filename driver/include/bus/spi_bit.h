/**
 * This is the difinition of spi
 *
 * Author: Arno
 *
 * Created 2018/09/19
 */
#ifndef __SPI_BIT_H__
#define __SPI_BIT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <common/pidef.h>
#include "spi.h"

struct spi_bit{
    struct spi spi;
    int delay_us;
    const struct spi_bit_operations *ops;
};

struct spi_bit_operations{
    int (*config)(spi_mode_t mode);
    int (*mosi_set)(int state);
    int (*miso_get)(void);
    int (*sck_set)(int state);
    void (*udelay)(int us);
};

int spi_bit_init(struct spi_bit *spi_bit, struct spi* spi);

#ifdef __cplusplus
}
#endif
#endif