#ifndef __PI_TIME_H__
#define __PI_TIME_H__
#include <common/pidef.h>

#ifdef __cplusplus
extern "C"{
#endif

#define pi_time_after(unknow, known)    ((long)(known)-(long)(unknow)<0)
#define pi_time_before(unknow, known)    ((long)(unknow)-(long)(known)<0)
#define pi_time_after_eq(unknow, known)  ((long)(unknow)-(long)(known)>=0)
#define pi_time_before_eq(unknow, known) ((long)(known)-(long)(unknow)>=0)
    
pi_tick_t pi_get_ticks(void);

pi_millis_t pi_get_millis(void);

void pi_delay_us(pi_usec_t us);

#define pi_sleep_us pi_delay_us

void pi_delay_ms(pi_millis_t millis);

void pi_sleep_ms(pi_millis_t millis);

void pi_sleep(float sec);

void pi_tick_periodic(void);
#ifdef __cplusplus
}
#endif
#endif