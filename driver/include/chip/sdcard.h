#ifndef __SDCARD_H__
#define __SDCARD_H__
#include <common/include.h>
#include <bus/pin.h>
#include <bus/spi.h>


struct sdcard{
    int cs_pin;
    int type;
    struct pin* pin;
    struct spi* spi;
    struct sddcard* (*open)(int index);
};

int sd_init(struct sdcard* sd);

int sd_deselect(struct sdcard* sd);

int sd_select(struct sdcard* sd);

int sd_get_cid(struct sdcard* sd, pi_uint8_t* cid);

int sd_get_csd(struct sdcard* sd, pi_uint8_t*csd);

int sd_get_sector_count(struct  sdcard* sd);

int sd_initialize(struct sdcard* sd);

int sd_read_disk(struct sdcard* sd, pi_uint32_t sector, pi_uint8_t* rd_data, pi_size_t nsec);

int sd_write_disk(struct sdcard* sd, pi_uint32_t sector, pi_uint8_t* wr_data, pi_size_t nsec);

#endif