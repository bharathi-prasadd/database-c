#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*------                          Enums and structs  ----------*/
typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    StatementType type;
} Statement;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

/*-------------         Function declarations       --------------*/

void display_prompt(void);
InputBuffer* new_input_buffer(void);
void close_input_buffer(InputBuffer* buffer);
void read_line(InputBuffer* buffer);
MetaCommandResult do_meta_command(InputBuffer *);
PrepareResult prepare_statement(char *, Statement *);
void execute_statement(Statement *statement);

int main()
{
    while(true)
    {
        display_prompt();
        InputBuffer* buffer = new_input_buffer();
        read_line(buffer);
        if (buffer->buffer[0] == '.')                               //if meta command execute it
        {
            switch(do_meta_command(buffer)) {
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
                    execute_statement(&statement);
                    break;
                case(PREPARE_UNRECOGNIZED_STATEMENT):
                {
                    printf("%s : Unrecognized keyword at the start\n",buffer->buffer);
                    break;
                }
            }
        }
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
MetaCommandResult do_meta_command(InputBuffer *buffer)
{
    if (strcmp(buffer->buffer,".exit") == 0)
    {
        close_input_buffer(buffer);
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("%s: Unrecognized meta command\n", buffer->buffer);
    }
}

/* branch over valid keywords and
   pass statements type
*/
PrepareResult prepare_statement(char *buffer, Statement *statement)
{
    /* strncmp is used instead of strcmp because
       keyword will be followed by arguments
    */
    if (strncmp(buffer, "insert", 6) == 0)
    {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    else if (strncmp(buffer, "select", 6) == 0)
    {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    else
        return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement)
{
    if (statement->type = STATEMENT_INSERT)
    {
        printf("Insertion\n");
    }
    else if (statement->type = STATEMENT_SELECT)
    {
        printf("Selection\n");
    }
    return;
}
