#include <bus/iic.h>

//#define IIC_DEBUG

#ifdef IIC_DEBUG
#define iic_dbg(fmt, ...)   pi_print(fmt, ##__VA_ARGS__)
#else
#define iic_dbg(fmt, ...)
#endif



pi_inline int iic_start(struct iic *iic)
{
    return iic->ops->start(iic);
}

pi_inline int iic_stop(struct iic *iic)
{
    return iic->ops->stop(iic);
}

pi_inline int iic_writeb(struct iic *iic, pi_uint8_t data, iic_ack_t *ack)
{
    return iic->ops->writeb(iic, data, ack);
}

pi_inline int iic_readb(struct iic *iic, pi_uint8_t *data, iic_ack_t ack)
{
    return iic->ops->readb(iic, data, ack);
}

int iic_init(struct iic* iic)
{
    if(iic->ops->init)
        return iic->ops->init(iic);
    return 0;
}

/**
  * @brief This function is used to write data to iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address.
  * @param data: data to be write.
  * @param len: length of data.
  * @retval 0 for success, otherwise -1.
  */
int iic_write(struct iic *iic, pi_uint8_t dev_addr, const pi_uint8_t *data, pi_size_t len)
{
    iic_ack_t ack;
    pi_assert(iic != KL_NULL);

    if(iic_start(iic) < 0)
        goto iic_error;
    if(iic_writeb(iic, (dev_addr<<1)|0x00, &ack)<0 || ack != IIC_ACK)
        goto iic_error;
    for(int i=0; i<len;i++)
    {
        if(iic_writeb(iic, *data ++, &ack) < 0 || ack != IIC_ACK)
            goto iic_error;
        iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
    }
    iic_stop(iic);
    return 0;
iic_error:
    iic_stop(iic);
    iic_dbg("iic write failed.");
    return -1;
}
/**
  * @brief This function is used to read data from iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address.
  * @param data: data to be read.
  * @param len: length of data.
  * @retval 0 for success, otherwise -1.
  */
int iic_read(struct iic *iic, pi_uint8_t dev_addr, pi_uint8_t *rd_data, pi_size_t len)
{
    iic_ack_t ack;
    pi_assert(iic != KL_NULL);

    if(iic_start(iic) < 0)
        goto iic_error;
    if(iic_writeb(iic, (dev_addr<<1)|0x01, &ack)<0 || ack != IIC_ACK)
        goto iic_error;
    for(int i=0; i<len;i++)
    {
        if(i==len-1)
        {
            if(iic_readb(iic, rd_data ++, IIC_NACK)<0)
                goto iic_error;
            iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
        }
        else
        {
            if(iic_readb(iic, rd_data ++, IIC_ACK)<0)
                goto iic_error;
            iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
        }
    }
    iic_stop(iic);
    return 0;
iic_error:
    iic_stop(iic);
    iic_dbg("iic read failed.\r\n");
    return -1;
}

/**
  * @brief This function is used to write and read data from iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address
  * @param address: slave device memory or register address.
  * @param flag: address option, 0 for 7 bit address, 1 for 10 bit address.
  * @param wr_data: data to be write.
  * @param wr_len: length of data to be write.
  * @retval 0 for success, otherwise -1.
  */
int iic_address_write(struct iic *iic, pi_uint8_t dev_addr, pi_uint16_t address, pi_uint8_t flag, pi_uint8_t *wr_data, pi_size_t wr_len)
{
    iic_ack_t ack;
    pi_assert(iic != KL_NULL);

    iic_dbg("iic_address_write(0x%x, 0x%x, %d, %d)", dev_addr, address, flag, wr_len);
    if(iic_start(iic) < 0)
    {
        iic_dbg("iic start failed.\r\n");
        goto iic_error;
    }
    if(iic_writeb(iic, (dev_addr<<1)|0x00, &ack)<0 || ack != IIC_ACK)
    {
        iic_dbg("iic write dev_addr failed.");
        goto iic_error;
    }
    iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
    if(flag == IIC_BYTE_ADDRESS)
    {
        if(iic_writeb(iic, (pi_uint8_t)address, &ack)<0 || ack != IIC_ACK)
        {
            iic_dbg("iic write address failed.\r\n");
            goto iic_error;
        }
        iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
    }
    else if(flag == IIC_WORD_ADDRESS)
    {
        if(iic_writeb(iic, (pi_uint8_t)address, &ack)<0 || ack != IIC_ACK)
            goto iic_error;
        if(iic_writeb(iic, (pi_uint8_t)((address>>8)&0xFF), &ack)<0 || ack != IIC_ACK)
            goto iic_error;
    }
    for(int i=0; i<wr_len;i++)
    {
        if(iic_writeb(iic, *wr_data ++, &ack) < 0 || ack != IIC_ACK)
        {
            iic_dbg("iic write data failed.\r\n");
            iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
            goto iic_error;
        }
        iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
    }
    iic_stop(iic);
    return 0;
iic_error:
    iic_stop(iic);
    iic_dbg("iic address write failed.\r\n");
    return -1;
}

/**
  * @brief This function is used to write and read data from iic bus.
  * @param iic: iic bus pointer.
  * @param dev_addr: iic device address
  * @param address: slave device memory or register address.
  * @param flag: address option, 0 for 7 bit address, 1 for 10 bit address.
  * @param wr_data: data to be write.
  * @param wr_len: length of data to be read.
  * @retval 0 for success, otherwise -1.
  */
int iic_address_read(struct iic *iic, pi_uint8_t dev_addr, pi_uint16_t address, pi_uint8_t flag, pi_uint8_t *rd_data, pi_size_t rd_len)
{
    iic_ack_t ack;
    pi_assert(iic != KL_NULL);

    iic_dbg("iic_address_read(0x%x, 0x%x, %d, %d)\r\n", dev_addr, address, flag, rd_len);
    iic_dbg("iic start...\r\n");
    if(iic_start(iic) < 0)
    {
        iic_dbg("iic address read start failed.\r\n");
        goto iic_error;
    }
    iic_dbg("iic write dev_addr:0x%x\r\n", dev_addr);
    if(iic_writeb(iic, (dev_addr<<1)|0x00, &ack)<0 || ack != IIC_ACK)
    {
        iic_dbg("iic write dev_addr 0x%02x failed, ack=%d.", dev_addr, ack);
        goto iic_error;
    }
    iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
    iic_dbg("iic write address:0x%x\r\n", address);
    if(flag == IIC_BYTE_ADDRESS)
    {
        if(iic_writeb(iic, (pi_uint8_t)address, &ack)<0 || ack != IIC_ACK)
        {
            iic_dbg("iic write address 0x%02x failed, ack=%d.\r\n", address, ack);
            goto iic_error;
        }
        iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
    }
    else if(flag == IIC_WORD_ADDRESS)
    {
        if(iic_writeb(iic, address, &ack)<0 || ack != IIC_ACK)
            goto iic_error;
        if(iic_writeb(iic, (address>>8)&0xFF, &ack)<0 || ack != IIC_ACK)
            goto iic_error;
    }
    iic_dbg("iic restart ...\r\n");
    if(iic_start(iic) < 0)
    {
        iic_dbg("iic restart failed.\r\n");
        goto iic_error;
    }
    iic_dbg("iic write dev_addr:0x%x\r\n", dev_addr);
    if(iic_writeb(iic, (dev_addr<<1)|0x01, &ack)<0 || ack != IIC_ACK)
    {
        iic_dbg("iic rewrite dev_addr 0x%02x failed, ack=%d.\r\n", dev_addr, ack);
        goto iic_error;
    }
    iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");

    for(int i=0; i<rd_len;i++)
    {
        if(i == rd_len-1)
        {
            iic_dbg("iic readb...\r\n");
            if(iic_readb(iic, rd_data ++, IIC_NACK)<0)
                goto iic_error;
            iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
        }
        else
        {
            if(iic_readb(iic, rd_data ++, IIC_ACK)<0)
                goto iic_error;
            iic_dbg("%s\r\n", (ack==IIC_ACK)?"ACK":"NACK");
        }
    }
    iic_stop(iic);
    return 0;
iic_error:
    iic_stop(iic);
    iic_dbg("iic address read failed.\r\n");
    return -1;
}
