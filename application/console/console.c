#include <string.h>
#include <console/console.h>
#include <console/command.h>


static const char* const console_welcome_message = "PI command server.\r\nType Help to view a list of registered commands.\r\n\r\n";
static const char* newline = "\r\n";

#define ASCII_DEL       (0x7F)

int console_write(command_port_t port, const char* wr_data, pi_size_t wr_len)
{
    console_t* console = port;

    return pi_stdio_write(console->stdio, wr_data, wr_len);
}

static void console_thread(void const* arg)
{
    pi_print("console thread running...\r\n");
    console_t* console = (console_t*)arg;
    int ret;
    
    char* rxbuf = console->rxbuf.buffer;

    console_write(console, console_welcome_message, strlen(console_welcome_message));

    for(;;)
    {

        int ch = pi_stdio_getc(console->stdio);

        if(ch <= 0)
        {
            pi_sleep_ms(10);
            continue;
        }
        pi_stdio_putc(console->stdio, ch);

        if(ch == '\n' || ch == '\r')
        {
            if(console->rxpos == 0)
                continue;
            console_write(console, newline, strlen(newline));

            ret = command_process(console, (const char*)rxbuf, console->txbuf.buffer, console->rxbuf.bufsz, console_write);
            memset(rxbuf, '\0', console->rxpos);
            console->rxpos = 0;
        }
        else
        {
            if(ch == '\r')
            {

            }
            else if(ch == '\b' || ch == ASCII_DEL)
            {
                if(console->rxpos > 0)
                {
                    console->rxpos --;
                    rxbuf[console->rxpos] = '\0';
                }
            }
            else if(ch >= ' ' && ch <= '~')
            {
                if(console->rxpos < console->rxbuf.bufsz)
                {
                    rxbuf[console->rxpos] = ch;
                    console->rxpos ++;
                }
            }
        }
    }
}

int console_init(console_t* console, pi_size_t stacksz)
{
    pi_assert(console);

    console->rxpos = 0;
    osThreadDef(cli, console_thread, osPriorityNormal, 0, stacksz);
    osThreadCreate(osThread(cli), console);
    return PI_ENO_OK;
}