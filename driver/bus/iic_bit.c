#include <common/include.h>
#include <bus/iic_bit.h>

#define scl_set(ops, state)	(ops->scl_set(state))
#define sda_set(ops, state)	(ops->sda_set(state))
#define sda_get(ops)		(ops->sda_get())

#define SCL_H(ops)			scl_set(ops, 1)
#define SCL_L(ops)			scl_set(ops, 0)
#define SDA_H(ops)			sda_set(ops, 1)
#define SDA_L(ops)			sda_set(ops, 0)
#define SDA_STATE(ops)		sda_get(ops)

pi_inline void iic_udelay(const struct iic_bit_operations *ops, int us)
{
	if(ops->udelay)
		ops->udelay(us);
}

pi_inline void iic_enable_output(const struct iic_bit_operations* ops)
{
	if(ops->enable_output)
		ops->enable_output();
}

pi_inline void iic_enable_input(const struct iic_bit_operations* ops)
{
	if(ops->enable_input)
		ops->enable_input();
}

pi_inline struct iic_bit *iic_to_iic_bit(struct iic *iic)
{
	return iic->private_data;
}

int iic_bit_start(struct iic *iic)
{
	struct iic_bit *iic_bit = iic_to_iic_bit(iic);
	iic_enable_output(iic_bit->ops);
	SDA_H(iic_bit->ops);
	SCL_H(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	SDA_L(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	SCL_L(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	return 0;
}

int iic_bit_stop(struct iic *iic)
{
	struct iic_bit *iic_bit = iic_to_iic_bit(iic);
	iic_enable_output(iic_bit->ops);
	SDA_L(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	SCL_H(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	SDA_H(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	return 0;
}
int iic_bit_writeb(struct iic *iic, pi_uint8_t data, iic_ack_t *ack)
{
	struct iic_bit *iic_bit = iic_to_iic_bit(iic);
	pi_uint8_t temp = 0;

	iic_enable_output(iic_bit->ops);

	for(int i=0;i<8;i++)
	{
		temp = data&0x80;
		data <<= 1;
		if(temp)
			SDA_H(iic_bit->ops);
		else
			SDA_L(iic_bit->ops);
		iic_udelay(iic_bit->ops, iic_bit->delay_us);
		SCL_H(iic_bit->ops);
		iic_udelay(iic_bit->ops, iic_bit->delay_us);
		SCL_L(iic_bit->ops);
		iic_udelay(iic_bit->ops, iic_bit->delay_us);
	}

	SDA_H(iic_bit->ops);

	iic_enable_input(iic_bit->ops);

	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	SCL_H(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	if(SDA_STATE(iic_bit->ops))
		*ack = IIC_NACK;
	else
		*ack = IIC_ACK;

	SCL_L(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);

	return 0;
}
int iic_bit_readb(struct iic *iic, pi_uint8_t *data, iic_ack_t ack)
{
	struct iic_bit *iic_bit = iic_to_iic_bit(iic);

	int temp = 0;

	SDA_H(iic_bit->ops);

	iic_enable_input(iic_bit->ops);

	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	for(int i=0;i<8;i++)
	{
		temp <<= 1;
		SCL_H(iic_bit->ops);
		iic_udelay(iic_bit->ops, iic_bit->delay_us);
		if(SDA_STATE(iic_bit->ops))
			temp |= 0x01;
		SCL_L(iic_bit->ops);
		iic_udelay(iic_bit->ops, iic_bit->delay_us);
	}

	iic_enable_output(iic_bit->ops);

	if(ack == IIC_ACK)
		SDA_L(iic_bit->ops);
	else
		SDA_H(iic_bit->ops);
	SCL_H(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	SCL_L(iic_bit->ops);
	iic_udelay(iic_bit->ops, iic_bit->delay_us);
	*data = temp;

	return 0;
}

const struct iic_operations iic_ops={
	.start = iic_bit_start,
	.stop = iic_bit_stop,
	.writeb = iic_bit_writeb,
	.readb = iic_bit_readb,
};

int iic_bit_init(struct iic_bit* iic_bit, struct iic* iic)
{
	pi_assert(iic_bit != PI_NULL);
	pi_assert(iic_bit->ops != PI_NULL);
	iic->private_data = iic_bit;
	iic->ops = &iic_ops;

	return iic_init(iic);
}


