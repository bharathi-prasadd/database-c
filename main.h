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

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
}ExecuteResult;

typedef struct {
    char *buffer;
    size_t buffer_length;
    ssize_t input_length;
}InputBuffer;

#define COLUMN_NAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct{
    uint32_t id;
    char name[COLUMN_NAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
}Row;

typedef struct{
    StatementType type;
    Row row_to_insert;
}Statement;


#define SIZE_OF_ATTRIBUTE(STRUCT, ATTR) sizeof(((STRUCT*)0)->ATTR)


#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100
extern const uint32_t  ROWS_PER_PAGE;
extern const uint32_t  TABLE_MAX_ROWS;

typedef struct {
    uint32_t n_rows;
    void *pointer[TABLE_MAX_PAGES];
}Table;

// Display a minimal promtp
void display_prompt(void);

// Functions associated with creating and freeing data structures
InputBuffer* new_input_buffer(void);
void close_input_buffer(InputBuffer* buffer);

Table* new_table(void);
void* get_row_slot(Table *table, uint32_t row_num);

/* Read a line using getline and store it
   into the input buffer
*/
void read_line(InputBuffer* buffer);

MetaCommandResult do_meta_command(Table *table,InputBuffer *);

PrepareResult prepare_statement(char *, Statement *);
ExecuteResult execute_statement(Table *table, Statement *statement);
ExecuteResult execute_insert(Table *table, Statement *statement);
ExecuteResult execute_select(Table *table, Statement *statement);

//Inserting and extracting rows from a page
void serialize_row(void *source,Row *row);
void deserialize_row(void *destination,Row *row);
#endif
