#ifndef __COMMAND_H__
#define __COMMAND_H__
#include <common/include.h>

typedef void* command_port_t;

typedef int (*command_output_t)(command_port_t port, const char* wr_data, pi_size_t wr_len);

typedef int (*command_handle_t)(command_port_t port, const char* inputstring, char* outbuffer, pi_size_t bufsz, command_output_t output_callback);

typedef struct command_item{
    struct command_item* next;
    const char* const name;
    const char* const brief;
    command_handle_t handle;
}command_item_t;

#define command_write_string(port, callback, message)     (callback(port, message, strlen(message)))

int console_register_handle(command_item_t* handle);

int command_process(command_port_t port, const char* const inputstring, char* outbuffer, pi_size_t bufsz, command_output_t output_callback);

const char* console_get_parameter(const char* inputstring, int index, pi_size_t* param_len);

pi_int8_t comand_get_number_of_parameter(const char* inputstring);
#endif