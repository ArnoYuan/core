#include <common/include.h>
#include <bus/serial.h>


static int _serial_poll_write(struct serial *serial, const pi_uint8_t* wr_data, pi_size_t wr_len)
{
    const struct serial_operations *ops = serial->ops;

    int length = wr_len;

    while(length)
    {
        if(ops->putc(serial, *wr_data ++) < 0)
            break;
        length --;
    }

    return wr_len - length;
}

static int _serial_poll_read(struct serial *serial, pi_uint8_t *rd_data, pi_size_t rd_len)
{
    const struct serial_operations *ops = serial->ops;

    int ch = -1;
    int length = rd_len;

    while(length)
    {
        ch = ops->getc(serial);
        if(ch < 0)
            break;
        *rd_data ++  = ch;
        length --;
    }
    return rd_len - length;
}

static int _serial_irq_write(struct serial *serial, const pi_uint8_t* wr_data, pi_size_t wr_len)
{
    pi_assert(serial->serial_tx);

    struct serial_fifo* tx_fifo = (struct serial_fifo*)serial->serial_tx;
    int length = wr_len;

    while(length)
    {
        if(tx_fifo->put_index != tx_fifo->get_index)
        {
            tx_fifo->buffer[tx_fifo->put_index] = *wr_data ++;
            tx_fifo->put_index ++;
            if(tx_fifo->put_index >= tx_fifo->bufsz)
                tx_fifo->put_index = 0;
        }
        length --;
    }
    return wr_len - length;
}

static int _serial_irq_read(struct serial *serial, pi_uint8_t *rd_data, pi_size_t rd_len)
{
    pi_assert(serial->serial_rx);

    struct serial_fifo *rx_fifo = serial->serial_rx;
    int length = rd_len;
    while(length)
    {
        int ch;
        if(rx_fifo->get_index != rx_fifo->put_index)
        {
            ch = rx_fifo->buffer[rx_fifo->get_index];
            rx_fifo->get_index ++;
            if(rx_fifo->get_index >= rx_fifo->bufsz)
                rx_fifo->get_index = 0;
        }
        else
            break;
        *rd_data ++ = ch & 0xFF;
        length --;
    }
    return rd_len - length;
}

static int _serial_dma_write(struct serial *serial, const pi_uint8_t* wr_data, pi_size_t wr_len)
{

    return 0;
}

static int _serial_dma_read(struct serial *serial, pi_uint8_t *rd_data, pi_size_t rd_len)
{

    return 0;
}


int serial_write(struct serial* serial, const pi_uint8_t* wr_data, pi_size_t wr_len)
{
    pi_assert(serial);
    pi_assert(serial->ops);
    pi_assert(wr_data);
    pi_assert(wr_len > 0);

    if(serial->flags & SERIAL_FLAG_IRQ_TX)
        return _serial_irq_write(serial, wr_data, wr_len);
    else if(serial->flags & SERIAL_FLAG_DMA_TX)
        return _serial_dma_write(serial, wr_data, wr_len);
    else
        return _serial_poll_write(serial, wr_data, wr_len);
}

int serial_read(struct serial* serial, pi_uint8_t* rd_data, pi_size_t rd_len)
{
    pi_assert(serial);
    pi_assert(serial->ops);
    pi_assert(rd_data);
    pi_assert(rd_len > 0);

    if(serial->flags & SERIAL_FLAG_IRQ_RX)
        return _serial_irq_read(serial, rd_data, rd_len);
    else if(serial->flags & SERIAL_FLAG_DMA_RX)
        return _serial_dma_read(serial, rd_data, rd_len);
    else
        return _serial_poll_read(serial, rd_data, rd_len);
}


int serial_init(struct serial* serial)
{
    int ret;
    pi_assert(serial);
    pi_assert(serial->ops);

    if(serial->ops->init)
    {
        if((ret = serial->ops->init(serial)) < 0)
            return ret;
    }

    return 0;
}

void serial_hw_isr(struct serial* serial, pi_uint8_t event)
{
    pi_assert(serial);
    pi_assert(serial->ops);

    const struct serial_operations *ops = serial->ops;
    switch(event & 0xFF)
    {
        case SERIAL_EVENT_IRQ_RX:
        {
            int ch = -1;
            struct serial_fifo *rx_fifo = (struct serial_fifo*)serial->serial_rx;
            while(1)
            {
                ch = ops->getc(serial);
                if(ch == -1)
                    break;
                rx_fifo->buffer[rx_fifo->put_index]=ch;
                rx_fifo->put_index += 1;
                if(rx_fifo->put_index>=rx_fifo->bufsz)
                    rx_fifo->put_index = 0;
                if(rx_fifo->put_index == rx_fifo->get_index)
                {
                    rx_fifo->get_index += 1;
                    if(rx_fifo->put_index >= rx_fifo->bufsz)
                    {
                        rx_fifo->get_index = 0;
                    }
                }
            }
            break;
        }
        case SERIAL_EVENT_IRQ_TX:
        {
            int ch;
            struct serial_fifo *tx_fifo = (struct serial_fifo*)serial->serial_tx;
            if(tx_fifo->get_index!=tx_fifo->put_index)
            {
                ch = tx_fifo->buffer[tx_fifo->get_index];
                if(ops->putc(serial,ch)<0)
                    break;
                tx_fifo->get_index +=1;
                if(tx_fifo->get_index>=tx_fifo->bufsz)
                    tx_fifo->get_index = 0;
            }
            break;
        }
        case SERIAL_EVENT_DMA_RX:
        {

            break;
        }
        case SERIAL_EVENT_DMA_TX:
        {

            break;
        }
    }
}
