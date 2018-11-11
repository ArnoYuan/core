#include <console/command.h>
#include <string.h>


static int help_handle(command_port_t port, const char* inputstring, char* outbuffer, pi_size_t bufsz, command_output_t output_callback);

static command_item_t command_head={
    .name = "help",
    .brief = "\r\nhelp:\r\n Lists all the registered commands\r\n\r\n",
    .handle = help_handle,
};

static int help_handle(command_port_t port, const char* inputstring, char* outbuffer, pi_size_t bufsz, command_output_t output_callback)
{
    command_item_t* command = &command_head;

    
    while(command != PI_NULL)
    {
        command_write_string(port, output_callback, command->brief);
        command = command->next;
    }

    return PI_ENO_OK;
}

int console_register_handle(command_item_t* handle)
{
    pi_assert(handle);

    static command_item_t* command_insert = &command_head;

    handle->next = PI_NULL;

    command_insert->next = handle;

    command_insert = handle;

    return PI_ENO_OK;
}

int command_process(command_port_t port, const char* const inputstring, char* outbuffer, pi_size_t bufsz, command_output_t output_callback)
{
    pi_assert(inputstring);
    pi_assert(outbuffer);
    pi_assert(output_callback);

    command_item_t* command = PI_NULL;
    int ret = PI_ENO_OK;
    const char* name;
    int length;

    for(command = &command_head; command != PI_NULL; command = command->next)
    {
        name = command->name;
        length = strlen(name);

        if((inputstring[length] == ' ') || (inputstring[length] == '\0'))
        {
            if(strncmp(inputstring, name, length) == 0)
            {
                break;
            }
        }
    }

    if(command != PI_NULL)
    {
        ret = command->handle(port, inputstring, outbuffer, bufsz, output_callback);
    }
    else
    {
        command_write_string(port, output_callback, "Command not recognised. Enter 'help' to view a list of available commands.\r\n\r\n");
        ret = PI_ENO_ERROR;
    }

    return ret;
}

const char* command_get_parameter(const char* inputstring, int index, pi_size_t* param_len)
{
    pi_assert(inputstring);
    pi_assert(param_len);

    *param_len = 0;
    int nparam = 0;
    const char* parameter = PI_NULL;

    while(nparam < index)
    {
        while((*inputstring != '\0') && (*inputstring != ' '))
            inputstring ++;

        while((*inputstring != '\0') && (*inputstring == ' '))
            inputstring ++;
        if(*inputstring != '\0')
        {
            nparam ++;
            if(nparam == index)
            {
                parameter = inputstring;
                while((*inputstring != '\0') && (*inputstring != ' '))
                {
                    (*param_len) ++;
                    inputstring ++;
                }

                if(*param_len == 0)
                    parameter = PI_NULL;

                break;
            }
        }
        else
        {
            break;
        }
    }

    return parameter;
}

pi_int8_t comand_get_number_of_parameter(const char* inputstring)
{
    pi_assert(inputstring);

    pi_int8_t count = 0;
    pi_base_t last_is_space = PI_FALSE;

    while(*inputstring != '\0')
    {
        if(*inputstring == ' ')
        {
            if(last_is_space != PI_TRUE)
            {
                count ++;
                last_is_space = PI_TRUE;
            }
        }
        else
        {
            last_is_space = PI_FALSE;
        }
        inputstring ++;
    }

    if(last_is_space == PI_TRUE)
        count --;

    return count;
}