#ifndef __W25QXX_H__
#define __W25QXX_H__
#include <common/include.h>
#include <bus/spi.h>
#include <bus/pin.h>


struct w25qxx{
    int cs_pin;
    struct pin* pin;
    struct spi* spi;
};

int w25qxx_sector_erase(struct w25qxx* w25qxx, pi_uint32_t address);

int w25qxx_block32_erase(struct w25qxx* w25qxx, pi_uint32_t address);

int w25qxx_block64_erase(struct w25qxx* w25qxx, pi_uint32_t address);

int w25qxx_write(struct w25qxx* w25qxx, pi_uint32_t address, pi_uint8_t* wr_data, pi_size_t wr_len);

int w25qxx_read(struct w25qxx* w25qxx, pi_uint32_t address, pi_uint8_t*rd_data, pi_size_t rd_len);

int w25qxx_read_manufacturer_device_id(struct w25qxx* w25qxx);

#endif