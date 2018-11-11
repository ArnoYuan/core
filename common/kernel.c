#include <common/include.h>

#if CONFIG_USING_RTOS==0
static pi_slist_t pi_co_thread_list = PI_SLIST_OBJECT_INIT(pi_co_thread_list);

void pi_co_thread_start(pi_co_thread_t* pthread, void* arg)
{
    pi_slist_insert(&pi_co_thread_list, &pthread->node);
}

void pi_co_thread_stop(pi_co_thread_t* pthread)
{
    pi_slist_remove(&pi_co_thread_list, &pthread->node);
}

static void pi_co_thread_dispatch(void)
{
    pi_co_thread_t* thread;

    pi_slist_fore_each_entry(thread, &pi_co_thread_list, node)
    {
        if(thread->entry != PI_NULL)
        {
            thread->entry(thread->arg);
        }
    }
}
#endif

void pi_kernel_start(void)
{
#if CONFIG_USING_RTOS==1
    osKernelStart();
#else
    for(;;)
    {
        pi_co_thread_dispatch();
    }
#endif
}

void pi_kernel_periodic(void)
{
#if CONFIG_USING_RTOS==1
    osSystickHandler();
#endif
    pi_tick_periodic();
}