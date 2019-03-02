#pragma once
#include "DataTypes.h"
#include "SignTable.h"

#define MEMORY_SIZE 1024
#define WORD_SIZE 13

char * search_code(char * code); /*Search opcode*/
int check_address_code(char * argument); /*Analyze address code of argument*/
int address_data(machine_code_type * machine_code, address_mode mode); /*Build machine code lines*/
address_mode analyze_arguments(char * code, char * arguments); /*Analyze and check arguments of commands*/
int address_data_second_process(char Instructions[MEMORY_SIZE][WORD_SIZE], address_mode mode, sign_table_ptr * sign_head, int IC); /*Analyze address code in the second process (Also signs)*/
