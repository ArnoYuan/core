#include <common/include.h>
#include <bus/pin.h>

int pin_init(struct pin* pin)
{
    if(pin->ops->init)
        return pin->ops->init(pin);

    return 0;
}

int pin_set_direction(struct pin* pin, int pin_id, pin_dir_t dir)
{
    pi_assert(pin);
    pi_assert(pin->ops);
    pi_assert(pin->ops->set_direction);

    int ret;

    if((ret = pin->ops->set_direction(pin, pin_id, dir)) != 0)
        return ret;

    return PI_ENO_OK;
}

int pin_get_direction(struct pin* pin, int pin_id)
{
    pi_assert(pin);
    pi_assert(pin->ops);
    pi_assert(pin->ops->get_direction);

    pin_dir_t dir = PIN_DIR_INPUT;

    int ret;

    if((ret = pin->ops->get_direction(pin, pin_id, &dir)) != 0)
        return ret;

    return dir;
}

int pin_get_output(struct pin* pin, int pin_id)
{
    pi_assert(pin);
    pi_assert(pin->ops);
    pi_assert(pin->ops->get_output);

    pin_level_t level = PIN_LEVEL_LOW;

    int ret;

    if((ret = pin->ops->get_output(pin, pin_id, &level)) != 0)
        return ret;

    return level;
}

int pin_set_level(struct pin* pin, int pin_id, pin_level_t level)
{
    pi_assert(pin);
    pi_assert(pin->ops);
    pi_assert(pin->ops->set_level);

    int ret;

    if((ret = pin->ops->set_level(pin, pin_id, level)) != 0)
        return ret;

    return PI_ENO_OK;
}

int pin_get_level(struct pin* pin, int pin_id)
{
    pi_assert(pin);
    pi_assert(pin->ops);
    pi_assert(pin->ops->get_level);

    int ret;

    pin_level_t level = PIN_LEVEL_LOW;
    if((ret = pin->ops->get_level(pin, pin_id, &level)) != 0)
        return ret;

    return level;
}
