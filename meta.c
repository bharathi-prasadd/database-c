#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

MetaCommandResult do_meta_command(Table *table,InputBuffer *buffer)
{
    if (strcmp(buffer->buffer,".exit") == 0)
    {
        close_input_buffer(buffer);
        free(table);
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("%s: Unrecognized meta command\n", buffer->buffer);
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}
