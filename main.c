#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "main.h"



int main()
{
    Table *table = new_table();
    while(true)
    {
        display_prompt();
        InputBuffer* buffer = new_input_buffer();
        read_line(buffer);
        if (buffer->buffer[0] == '.')                               //if meta command execute it
        {
            switch(do_meta_command(table, buffer)) {
                case(META_COMMAND_SUCCESS):
                    break;
                case(META_COMMAND_UNRECOGNIZED_COMMAND):
                {
                    printf("%s : Unrecognized meta command\n",buffer->buffer);
                    break;
                }
            }
        }
        else {
            Statement statement;
            switch (prepare_statement(buffer->buffer, &statement)) {
                case(PREPARE_SUCCESS):
                {
                    execute_statement(table, &statement);
                    break;
                }
                case(PREPARE_SYNTAX_ERROR):
                {
                    printf("%s: Invalid syntax\n",buffer->buffer);
                    break;
                }
                    case(PREPARE_UNRECOGNIZED_STATEMENT):
                {
                    printf("%s : Unrecognized keyword at the start\n",buffer->buffer);
                    break;
                }
            }
        }
        close_input_buffer(buffer);
    }
}

void display_prompt(void)
{
    printf(".db >");
}

/* Allocate memory for a new buffer and initialise all values*/
InputBuffer* new_input_buffer(void)
{
    InputBuffer *buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    *buffer = (InputBuffer){.buffer = NULL, .buffer_length = 0, .input_length = 0};
    return buffer;
}

/* Read one line of input and store it in buffer->buffer
   after truncating the newline character
*/
void read_line(InputBuffer *buffer)
{
    buffer->input_length = getline(&(buffer->buffer), &(buffer->buffer_length), stdin) - 1; //-1 to account for the \n that will be truncated
    if ( buffer->input_length < 0 )
    {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }
    buffer->buffer[buffer->input_length] = 0; //truncate \n at the end
}


void close_input_buffer(InputBuffer* buffer)
{
    free(buffer->buffer);
    free(buffer);
}


/* branch over different meta commands and
   execute if it is a valid meta command
*/
/* branch over valid keywords and
   pass statements type
*/




Table* new_table(void)
{
    Table *table = (Table*)malloc(sizeof(Table));
    table->n_rows = 0;
    for (int i = 0; i < TABLE_MAX_PAGES; i++)
    {
        table->pointer[i] = NULL;
    }
    return table;
}

void free_table(Table *table)
{
    for (int i = 0; table->pointer[i] && i < TABLE_MAX_PAGES; i++)
    {
        free(table->pointer[i]);
    }
    free(table);
}
