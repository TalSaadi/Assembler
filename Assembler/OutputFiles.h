#pragma once

#define MEMORY_SIZE 1024
#define WORD_SIZE 13

void ObjectFile(char Instructions[MEMORY_SIZE][WORD_SIZE], char Data[MEMORY_SIZE][WORD_SIZE], int IC, int DC, char * filename);
void EntriesFile(sign_table_ptr * sign_head, char * filename);
void ExternsFile(sign_table_ptr * sign_head, char *filename);
