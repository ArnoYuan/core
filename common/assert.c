#include <common/include.h>
#if CONFIG_PI_USING_ASSERT

__kernel_weak void pi_assert_failed(pi_uint8_t *file, pi_uint32_t line)
{
    pi_print("assert failed int file %s, at line %d\r\n", file, line);
    while(1);
}

#endif