#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Reader.h"
#include "DataTypes.h"
#include "FirstProcess.h"
#include "Memory.h"
#include "ArgsEncoder.h"
#include "SignTable.h"
#include "Analasys.h"
#include "StringsProcess.h"

#define OPCODE_LENGTH 16
#define MEMORY_SIZE 1024
#define WORD_SIZE 13
#define NULL_TER_BIT 12
#define ARE1 11
#define ARE2 10

char * search_code(char * code) {
	opcode opcode_table[OPCODE_LENGTH] = { {"mov", "0000"}, {"cmp", "0001"}, {"add", "0010"}, {"sub", "0011"}, {"not", "0100"}, {"clr", "0101"}, {"lea", "0110"}, {"inc", "0111"}, {"dec", "1000"}, {"jmp", "1001"}, {"bne", "1010"}, {"red", "1011"}, {"prn", "1100"}, {"jsr", "1101"}, {"rts", "1110"}, {"stop", "1111"} };
	int i;
	for (i = 0; i < OPCODE_LENGTH; i++) {
		if (strcmp(opcode_table[i].code, code) == 0) {
			return opcode_table[i].opcode;
		}
	}
	return NULL;
}

address_mode analyze_arguments(char * code, char * arguments) {
	/*** This function checks which command was operated and
		 checks arguments to see if there is right amound of
		 operands and if operands are valid ***/
	char *args;
	address_mode mode;
	#pragma warning(suppress : 4996)
	if (strcmp(code, "mov") == 0 || strcmp(code, "add") == 0 || strcmp(code, "sub") == 0) {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, ",");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Error in file: %s, line number %d, Too much args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Error in file: %s, line number %d, Not enuogh args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		else {
			clear_args(mode.first_arg);
			mode.first_mode = check_address_code(mode.first_arg);
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.second_mode, "001") == 0) {
				printf("Error in file: %s, line number %d, Invalid mode\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			else {
				return mode;
			}
		}
	}
	else if (strcmp(code, "cmp") == 0) {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, ",");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Error in file: %s, line number %d, Too much args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Error in file: %s, line number %d, Not enuogh args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		else {
			clear_args(mode.first_arg);
			mode.first_mode = check_address_code(mode.first_arg);
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			return mode;
		}
	}
	else if (strcmp(code, "lea") == 0) {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, ",");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Error in file: %s, line number %d, Too much args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Error in file: %s, line number %d, Not enuogh args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		else {
			clear_args(mode.first_arg);
			mode.first_mode = check_address_code(mode.first_arg);
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.first_mode, "011") != 0) {
				printf("Error in file: %s, line number %d, Invalid mode\n", globalFileName, globalLineNum);
			}
			else if (strcmp(mode.second_mode, "001") == 0) {
				printf("Error in file: %s, line number %d, Invalid mode\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			else {
				return mode;
			}
		}
	}
	else if (strcmp(code, "not") == 0 || strcmp(code, "clr") == 0 || strcmp(code, "inc") == 0 || strcmp(code, "dec") == 0 || strcmp(code, "jmp") == 0 || strcmp(code, "bne") == 0 || strcmp(code, "red") == 0 || strcmp(code, "jsr") == 0) {
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Error in file: %s, line number %d, Too much args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		if (mode.second_arg == NULL) {
			printf("Error in file: %s, line number %d, Not enuogh args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		else {
			mode.first_mode = "000";
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.second_mode, "001") == 0) {
				printf("Error in file: %s, line number %d, Invalid arg\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			return mode;
		}
	}
	else if (strcmp(code, "prn") == 0) {
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Error in file: %s, line number %d, Too much args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		if (mode.second_arg == NULL) {
			printf("Error in file: %s, line number %d, Not enuogh args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		else {
			mode.first_mode = "000";
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			return mode;
		}
	}
	else if (strcmp(code, "rts") == 0 || strcmp(code, "stop") == 0) {
		#pragma warning(suppress : 4996)
		args = strtok(NULL, " ");
		if (args == NULL || is_empty(args)) {
			mode.first_mode = "000";
			mode.second_mode = "000";
			return mode;
		}
		else {
			printf("Error in file: %s, line number %d, Too much args\n", globalFileName, globalLineNum);
			error_found = 1;
		}
	}
}

int check_address_code(char * argument) {
	/* This function gets an operand, checks if it valid and determines his address code */
	int i;
	if (*argument == '@') { /* Register */
		if (legal_reg(argument + 1)) {
			return "101";
		}
		else {
			printf("Error in file: %s, line number %d, Not a legal register\n", globalFileName, globalLineNum);
			error_found = 1;
		}

	}
	else if (isalpha(*argument)) { /* Sign */
		if (legal_sign(argument)) {
			return "011";
		}
		else {
			printf("Error in file: %s, line number %d, Not a legal sign\n", globalFileName, globalLineNum);
			error_found = 1;
		}
	}
	else { /* Number */
		if (!isdigit(*(argument)) && *(argument) != '+' && *(argument) != '-') {
			printf("Error in file: %s, line number %d, Not a valid argument\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		for (i = 1; i < strlen(argument); i++) {
			if (!isdigit(*(argument + i))) {
				printf("Error in file: %s, line number %d, Not a valid argument\n", globalFileName, globalLineNum);
				error_found = 1;
			}
		}
		return "001";
	}
}

int address_data(machine_code_type * machine_code, address_mode mode) {
	/* This funcion encode the data to machine code with the address codes that were found */
	if (strcmp(mode.first_mode, "000") == 0) { /* Empty operand */
		if (strcmp(mode.second_mode, "000") == 0) {
			return 0;
		}
		else if (strcmp(mode.second_mode, "001") == 0) { /* Mode 1*/
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_number(machine_code, mode.second_arg, 1);
			return 1;
		}
		else if (strcmp(mode.second_mode, "011") == 0) { /* Mode 3*/
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_sign(machine_code, 1);
			return 1;
		}
		else if (strcmp(mode.second_mode, "101") == 0) { /* Mode 5*/
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_reg(machine_code, mode.second_arg, 1);
			return 1;
		}
	}
	else if (strcmp(mode.first_mode, "001") == 0) {
		machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
		if (machine_code->machine_code == NULL) {
			printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		encode_number(machine_code, mode.first_arg, 1);
		if (strcmp(mode.second_mode, "000") == 0) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "001") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_number(machine_code, mode.second_arg, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_sign(machine_code, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_reg(machine_code, mode.second_arg, 2);
			return 2;
		}
	}
	else if (strcmp(mode.first_mode, "011") == 0) {
		machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
		if (machine_code->machine_code == NULL) {
			printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		encode_sign(machine_code, 1);
		if (strcmp(mode.second_mode, "000") == 0) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "001") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_number(machine_code, mode.second_arg, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_sign(machine_code, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_reg(machine_code, mode.second_arg, 2);
			return 2;
		}
	}
	else if (strcmp(mode.first_mode, "101") == 0) {
		if (strcmp(mode.second_mode, "000") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_reg(machine_code, mode.first_arg, 1);
			return 1;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_2_regs(machine_code, mode.first_arg, mode.second_arg, 1);
			return 1;
		}
		else {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			encode_reg(machine_code, mode.first_arg, 1);
			if (strcmp(mode.second_mode, "001") == 0) {
				machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
				if (machine_code->machine_code == NULL) {
					printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
					error_found = 1;
				}
				encode_number(machine_code, mode.second_arg, 2);
				return 2;
			}
			else if (strcmp(mode.second_mode, "011") == 0) {
				machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
				if (machine_code->machine_code == NULL) {
					printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
					error_found = 1;
				}
				encode_sign(machine_code, 2);
				return 2;
			}
		}
	}
}

int address_data_second_process(char Instructions[MEMORY_SIZE][WORD_SIZE], address_mode mode, sign_table_ptr * sign_head, int IC) {
	/* This function encodes data in the second process (Signs) */
	int sign_index, i;
	if (strcmp(mode.first_mode, "000") == 0) {
		if (strcmp(mode.second_mode, "000") == 0) {
			return 0;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			sign_index = sign_place(sign_head, mode.second_arg);
			if (sign_index == 0) { /* All extern signs are with place 0 */
				Instructions[IC][ARE2] = '0';
				Instructions[IC][ARE1] = '1';
				mark_extern(sign_head, mode.second_arg, IC);
			}
			else {
				Instructions[IC][ARE2] = '1';
				Instructions[IC][ARE1] = '0';
			}
			for (i = 9; i >= 0; sign_index = sign_index >> 1, i--) {
				Instructions[IC][i] = (sign_index & 1) + '0';
			}
			Instructions[IC][NULL_TER_BIT] = '\0';
			return 1;
		}
		else {
			return 1;
		}
	}
	else if (strcmp(mode.first_mode, "001") == 0) {
		IC++;
		if (strcmp(mode.second_mode, "000") == 0) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			sign_index = sign_place(sign_head, mode.second_arg);
			if (sign_index == 0) {
				Instructions[IC][ARE2] = '0';
				Instructions[IC][ARE1] = '1';
				mark_extern(sign_head, mode.second_arg, IC);
			}
			else {
				Instructions[IC][ARE2] = '1';
				Instructions[IC][ARE1] = '0';
			}
			for (i = 9; i >= 0; sign_index = sign_index >> 1, i--) {
				Instructions[IC][i] = (sign_index & 1) + '0';
			}
			Instructions[IC][NULL_TER_BIT] = '\0';
			return 1;
		}
		else {
			return 2;
		}
	}
	else if (strcmp(mode.first_mode, "011") == 0) {
		sign_index = sign_place(sign_head, mode.first_arg);
		if (sign_index == 0) {
			Instructions[IC][ARE2] = '0';
			Instructions[IC][ARE1] = '1';
			mark_extern(sign_head, mode.first_arg, IC);
		}
		else {
			Instructions[IC][ARE2] = '1';
			Instructions[IC][ARE1] = '0';
		}
		for (i = 9; i >= 0; sign_index = sign_index >> 1, i--) {
			Instructions[IC][i] = (sign_index & 1) + '0';
		}
		Instructions[IC][NULL_TER_BIT] = '\0';
		IC++;
		if (strcmp(mode.second_mode, "000") == 0) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			sign_index = sign_place(sign_head, mode.second_arg);
			if (sign_index == 0) {
				Instructions[IC][ARE2] = '0';
				Instructions[IC][ARE1] = '1';
				mark_extern(sign_head, mode.second_arg, IC);
			}
			else {
				Instructions[IC][ARE2] = '1';
				Instructions[IC][ARE1] = '0';
			}
			for (i = 9; i >= 0; sign_index = sign_index >> 1, i--) {
				Instructions[IC][i] = (sign_index & 1) + '0';
			}
			Instructions[IC][NULL_TER_BIT] = '\0';
			return 2;
		}
		else {
			return 2;
		}
	}
	else if (strcmp(mode.first_mode, "101") == 0) {
		IC++;
		if (strcmp(mode.second_mode, "000") == 0) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			return 1;
		}
		else {
			if (strcmp(mode.second_mode, "001") == 0) {
				return 2;
			}
			else if (strcmp(mode.second_mode, "011") == 0) {
				sign_index = sign_place(sign_head, mode.second_arg);
				if (sign_index == 0) {
					Instructions[IC][ARE2] = '0';
					Instructions[IC][ARE1] = '1';
					mark_extern(sign_head, mode.second_arg, IC);
				}
				else {
					Instructions[IC][ARE2] = '1';
					Instructions[IC][ARE1] = '0';
				}
				for (i = 9; i >= 0; sign_index = sign_index >> 1, i--) {
					Instructions[IC][i] = (sign_index & 1) + '0';
				}
				Instructions[IC][NULL_TER_BIT] = '\0';
				return 2;
			}
		}
	}
}
