#include <string.h>
#include <bus/iic.h>
#include <chip/at24cxx.h>

struct at24cxx_info{
    at24cxx_chip_t chip_type;
    pi_uint8_t page_size;
    pi_uint16_t page_num;
    pi_uint8_t addr_size;
    pi_uint8_t dev_mask;
    pi_uint16_t mdelay;
};

static const struct at24cxx_info at24cxx_info[]={
    {.chip_type = AT24C01, .page_size = 8, .page_num = 16, .addr_size = 1, .dev_mask = 0, .mdelay = 7},
    {.chip_type = AT24C02, .page_size = 8, .page_num = 32, .addr_size = 1, .dev_mask = 0, .mdelay = 12},
    {.chip_type = AT24C04, .page_size = 16, .page_num = 32, .addr_size = 1, .dev_mask = 0x01, .mdelay = 15},
    {.chip_type = AT24C08, .page_size = 16, .page_num = 64, .addr_size = 1, .dev_mask = 0x03, .mdelay = 15},
    {.chip_type = AT24C16, .page_size = 16, .page_num = 128, .addr_size = 1, .dev_mask = 0x07, .mdelay = 15},
    {.chip_type = AT24C32, .page_size = 32, .page_num = 128, .addr_size = 2, .dev_mask = 0, .mdelay = 15},
    {.chip_type = AT24C64, .page_size = 32, .page_num = 256, .addr_size = 2, .dev_mask = 0, .mdelay = 15},
    {.chip_type = AT24C128, .page_size = 64, .page_num = 256, .addr_size = 2, .dev_mask = 0, .mdelay = 15},
    {.chip_type = AT24C256, .page_size = 64, .page_num = 512, .addr_size = 2, .dev_mask = 0, .mdelay = 15},
    {.chip_type = AT24C512, .page_size = 128, .page_num = 512, .addr_size = 2, .dev_mask = 0, .mdelay = 15},
};


static struct at24cxx_info* _get_chip_info(at24cxx_chip_t chip_type)
{
    pi_assert(at24cxx_info);

    for(int i = 0; i < sizeof(at24cxx_info) / sizeof(at24cxx_info[0]); i ++)
    {
        if(at24cxx_info[i].chip_type == chip_type)
            return (struct at24cxx_info*)(at24cxx_info + i);
    }  

    return PI_NULL;
}

int at24cxx_write(struct at24cxx* at24cxx, pi_uint16_t address, pi_uint8_t* wr_data, pi_size_t len)
{
    pi_assert(at24cxx);
    pi_assert(address >= 0);
    pi_assert(wr_data);
    pi_assert(len > 0);

    struct at24cxx_info* chip_info = _get_chip_info(at24cxx->chip_type);

    pi_assert(chip_info);
    pi_assert(address + len <= chip_info->page_size * chip_info->page_num);

    int length = len;
    int wr_len = 0;

    int addr_flag = IIC_BYTE_ADDRESS;
    if(chip_info->addr_size == 2)
        addr_flag = IIC_WORD_ADDRESS;

    while(length > 0)
    {
        pi_uint8_t dev_addr = at24cxx->dev_addr | ((address >> 8) & chip_info->dev_mask);

        if((address & (chip_info->page_size - 1)) + length > chip_info->page_size)
            wr_len = chip_info->page_size - (address & (chip_info->page_size - 1));
        else
            wr_len = length;

        if(iic_address_write(at24cxx->bus, dev_addr, address, addr_flag, wr_data, wr_len) < 0)
            break;
        length -= wr_len;
        wr_data += wr_len;
        address += wr_len;
        pi_sleep_ms(chip_info->mdelay);
    }

    return (len - length);
}

int at24cxx_read(struct at24cxx* at24cxx, pi_uint16_t address, pi_uint8_t* rd_data, pi_size_t len)
{
    pi_assert(at24cxx);
    pi_assert(address >= 0);
    pi_assert(rd_data);
    pi_assert(len > 0);

    struct at24cxx_info* chip_info = _get_chip_info(at24cxx->chip_type);

    pi_assert(chip_info);
    pi_assert(address + len <= chip_info->page_size * chip_info->page_num);


    int length = len;
    int rd_len = 0;

    int addr_flag = IIC_BYTE_ADDRESS;
    if(chip_info->addr_size == 2)
        addr_flag = IIC_WORD_ADDRESS;

    while(length > 0)
    {
        pi_uint8_t dev_addr = at24cxx->dev_addr | ((address >> 8) & chip_info->dev_mask);

        if((address & (chip_info->page_size - 1)) + length > chip_info->page_size)
            rd_len = chip_info->page_size - (address & (chip_info->page_size - 1));
        else
            rd_len = length;
        if(iic_address_read(at24cxx->bus, dev_addr, address, addr_flag, rd_data, rd_len) < 0)
            break;
        length -= rd_len;
        rd_data += rd_len;
        address += rd_len;
    }

    return (len - length);
}
