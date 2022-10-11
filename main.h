#ifndef MAIN_H
#define MAIN_H

#include <unistd.h>
#include <stdint.h>

#define STR(X) #X
#define STR_(X) STR(X)



typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
}MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT
}PrepareResult;

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

#define COLUMN_NAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct {
    uint32_t id;
    char name[COLUMN_NAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row;

typedef struct {
    StatementType type;
    Row row_to_insert;
} Statement;


#define SIZE_OF_ATTRIBUTE(STRUCT, ATTR) sizeof(((STRUCT*)0)->ATTR)
#define ID_SIZE SIZE_OF_ATTRIBUTE(Row, id)
#define NAME_SIZE SIZE_OF_ATTRIBUTE(Row, name)
#define EMAIL_SIZE SIZE_OF_ATTRIBUTE(Row, email)
#define ID_OFFSET 0
#define NAME_OFFSET ID_SIZE + ID_OFFSET
#define EMAIL_OFFSET NAME_SIZE + NAME_OFFSET

// Display a minimal promtp
void display_prompt(void);

/* Allocate memory for a new input buffer and
   initialize it
*/
InputBuffer* new_input_buffer(void);

/* Free memory allocated to the input buffer */
void close_input_buffer(InputBuffer* buffer);

/* Read a line using getline and store it
   into the input buffer
*/
void read_line(InputBuffer* buffer);
MetaCommandResult do_meta_command(InputBuffer *);
PrepareResult prepare_statement(char *, Statement *);
void execute_statement(Statement *statement);

#endif

