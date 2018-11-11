#include <common/include.h>

void *pi_mem_alloc(pi_size_t size)
{
#if CONFIG_PI_USING_RTOS==1
    return pvPortMalloc(size);
#else
    return PI_NULL;
#endif
}

void *pi_mem_realloc(void *ptr,pi_size_t size)
{
    void *new = PI_NULL;
#if CONFIG_PI_USING_RTOS==1
    new = pvPortMalloc(size);
    if(ptr)
    {
        memcpy(new, ptr, size);
        vPortFree(ptr);
    }
#endif
    return new;
}

void pi_mem_free(void *ptr)
{
#if CONFIG_PI_USING_RTOS==1
    vPortFree(ptr);
#endif
}