#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

const uint32_t  ID_SIZE = SIZE_OF_ATTRIBUTE(Row, id);
const uint32_t  NAME_SIZE = SIZE_OF_ATTRIBUTE(Row, name);
const uint32_t  EMAIL_SIZE = SIZE_OF_ATTRIBUTE(Row, email);
const uint32_t  ROW_SIZE = ID_SIZE + NAME_SIZE + EMAIL_SIZE;

const uint32_t  ID_OFFSET = 0;
const uint32_t  NAME_OFFSET = ID_SIZE + ID_OFFSET;
const uint32_t  EMAIL_OFFSET = NAME_SIZE + NAME_OFFSET;

const uint32_t  ROWS_PER_PAGE = PAGE_SIZE/(ROW_SIZE);
const uint32_t  TABLE_MAX_ROWS = ROWS_PER_PAGE*TABLE_MAX_PAGES;

PrepareResult prepare_statement(char *buffer, Statement *statement)
{
    /* strncmp is used instead of strcmp because
       keyword will be followed by arguments
    */
    if (strncmp(buffer, "insert", 6) == 0)
    {
        statement->type = STATEMENT_INSERT;
        int arg_count, null_of;
        arg_count = sscanf(buffer,"insert %u %" STR_(COLUMN_NAME_SIZE) "s %" STR_(COLUMN_EMAIL_SIZE) "s%n", &(statement->row_to_insert.id),
                                                                                    statement->row_to_insert.name,
                                                                                    statement->row_to_insert.email,
                                                                                    &null_of);
        if (arg_count != 3 || null_of == 0 || buffer[null_of] != '\0' )
            return PREPARE_SYNTAX_ERROR;
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

ExecuteResult execute_statement(Table *table, Statement *statement)
{
    switch(statement->type) {
        case(STATEMENT_SELECT):
            return execute_select(table, statement);
        case(STATEMENT_INSERT):
            return execute_insert(table, statement);
        default:
            return EXECUTE_SUCCESS;
    }
}


ExecuteResult execute_insert(Table *table,Statement *statement)
{
    if (table->n_rows == TABLE_MAX_ROWS)
    {
        printf("Insert failed:Table full\n");
        return EXECUTE_TABLE_FULL;
    }
    else
    {
        serialize_row(get_row_slot(table, table->n_rows),&(statement->row_to_insert));
        table->n_rows += 1;
        return EXECUTE_SUCCESS;
    }
}

ExecuteResult execute_select(Table *table, Statement *statement)
{
    Row *row = (Row*)malloc(ROW_SIZE);
    for (uint32_t i = 0; i < table->n_rows; i++)
    {
        deserialize_row(get_row_slot(table, i), (row));
        printf("%d |%s | %s\n", row->id, row->name, row->email);
        //printf("%3d |%.32s | %.255s\n", row.id, row.name, row.email);
    }
    free(row);
    return EXECUTE_SUCCESS;
}

void* get_row_slot(Table *table, uint32_t row_num)
{
    int page_num = row_num/ROWS_PER_PAGE;
    void *page = table->pointer[page_num];
    if (page == NULL)
        page = table->pointer[page_num] = malloc(PAGE_SIZE);
    int offset = row_num%ROWS_PER_PAGE;
    return page+ ROW_SIZE * offset;
}

void serialize_row(void *destination,Row *row)
{
    memcpy(destination + ID_OFFSET, &(row->id), ID_SIZE);
    memcpy(destination + NAME_OFFSET, &(row->name), NAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(row->email), EMAIL_SIZE);
}

void deserialize_row(void *destination,Row *row)
{
    memcpy(&(row->id), destination + ID_OFFSET,  ID_SIZE);
    memcpy(&(row->name), destination + NAME_OFFSET, NAME_SIZE);
    memcpy(&(row->email), destination + EMAIL_OFFSET, EMAIL_SIZE);
}
