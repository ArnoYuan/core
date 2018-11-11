#include <common/include.h>

extern void driver_init(void);
extern void application_init(void);


void startup(void)
{
    driver_init();

    application_init();
}

int main(void)
{
    startup();

    pi_kernel_start();

    return 0;
}