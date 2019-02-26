#pragma once
#include "DataTypes.h"
#include "SignTable.h"

#define MEMORY_SIZE 1024
#define WORD_SIZE 13

int second_process(char Instructions[MEMORY_SIZE][WORD_SIZE], char *line, sign_table_ptr * sign_head, int IC);
