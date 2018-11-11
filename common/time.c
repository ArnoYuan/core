#include <common/include.h>

pi_volatile pi_tick_t pi_ticks = 0;

pi_tick_t pi_get_ticks(void)
{
    return pi_ticks;
}

pi_millis_t pi_get_millis(void)
{
    return pi_ticks * 1000 / KERNEL_HZ;
}

void pi_delay_us(pi_usec_t us)
{
    int count_1us = (CPU_CLOCK_HZ / 1000000);

    for(int i = 0; i < us; i ++)
        for(int j = 0; j < count_1us; j ++);
}

void pi_delay_ms(pi_millis_t millis)
{
    unsigned long timeout = pi_ticks + millis * KERNEL_HZ / 1000;

    while(pi_time_before(pi_ticks, timeout));
}

void pi_sleep_ms(pi_millis_t millis)
{
#if CONFIG_PI_USING_RTOS==1
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if(xTaskGetSchedulerState()==taskSCHEDULER_NOT_STARTED)
    {
        pi_delay_ms(millis);
    }
    else
    {
        osDelay(millis);
    }
#else
    osDelay(millis);
#endif
#else
    pi_delay_ms(millis);
#endif
}

void pi_sleep(float sec)
{
    pi_usec_t usec = (pi_usec_t)(sec * 1000000) % 1000;
    pi_sleep_ms((pi_millis_t)(sec * 1000));
    pi_sleep_us(usec);
}

void pi_tick_periodic(void)
{
    pi_ticks ++;
}
