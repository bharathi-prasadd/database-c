#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

void display_prompt(void);
InputBuffer* new_input_buffer(void);
void close_input_buffer(InputBuffer* buffer);
void read_line(InputBuffer* buffer);

int main()
{
    while(true)
    {
        display_prompt();
        InputBuffer* buffer = new_input_buffer();
        read_line(buffer);
        if (strcmp(buffer->buffer, ".exit") == 0)
        {
            close_input_buffer(buffer);
            exit(EXIT_SUCCESS);
        }
        else
        {
            printf("%s :Unrecognized command\n", buffer->buffer);
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
