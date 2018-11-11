#ifndef __AT24CXX_H__
#define __AT24CXX_H__
#include <common/pidef.h>
#include <bus/iic.h>

typedef enum{
    AT24C01,
    AT24C02,
    AT24C04,
    AT24C08,
    AT24C16,
    AT24C32,
    AT24C64,
    AT24C128,
    AT24C256,
    AT24C512,
}at24cxx_chip_t;

struct at24cxx{
    const pi_uint16_t dev_addr;
    const at24cxx_chip_t chip_type;
    struct iic* bus;
};

int at24cxx_write(struct at24cxx* at24cxx, pi_uint16_t address, pi_uint8_t* wr_data, pi_size_t len);

int at24cxx_read(struct at24cxx* at24cxx, pi_uint16_t address, pi_uint8_t* rd_data, pi_size_t len);

#endif