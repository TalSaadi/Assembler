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
#include "Encoding.h"
#include "StringsProcess.h"

#define OPCODE_LENGTH 16

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
	address_mode mode;
	#pragma warning(suppress : 4996)
	if (strcmp(code, "mov") == 0 || strcmp(code, "add") == 0 || strcmp(code, "sub") == 0) {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, ",");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Too much args");
			exit(0);
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit(0);
		}
		else {
			clear_args(mode.first_arg);
			mode.first_mode = check_address_code(mode.first_arg);
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.second_mode, "001") == 0) {
				printf("Invalid mode");
				exit(0);
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
			printf("Too much args");
			exit(0);
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit(0);
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
			printf("Too much args");
			exit(0);
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit(0);
		}
		else {
			clear_args(mode.first_arg);
			mode.first_mode = check_address_code(mode.first_arg);
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.first_mode, "011") != 0) {
				printf("Invalid mode");
			}
			else if (strcmp(mode.second_mode, "001") == 0) {
				printf("Invalid mode");
				exit(0);
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
			printf("Too much args");
			exit(0);
		}
		if (mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit(0);
		}
		else {
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.second_mode, "001") == 0) {
				printf("Invalid arg");
				exit(0);
			}
			return mode;
		}
	}
	else if (strcmp(code, "prn") == 0) {
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Too much args");
			exit(0);
		}
		if (mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit(0);
		}
		else {
			clear_args(mode.second_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			return mode;
		}
	}
	else if (strcmp(code, "rts") == 0 || strcmp(code, "stop") == 0) {
		if (arguments != NULL || !is_empty(arguments)) {
			printf("Too much args");
			exit(0);
		}
	}
}

int check_address_code(char * argument) {
	int i;
	if (*argument == '@') {
		if (legal_reg(argument + 1)) {
			return "101";
		}
		else {
			printf("Not a legal register");
			exit(0);
		}

	}
	else if (isalpha(*argument)) {
		if (legal_sign(argument)) {
			return "011";
		}
		else {
			printf("Not a legal sign");
			exit(0);
		}
	}
	else {
		if (!isdigit(*(argument)) && *(argument) != '+' && *(argument) != '-') {
			printf("Not a valid argument");
			exit(0);
		}
		for (i = 0; i < strlen(argument); i++) {
			if (!isdigit(*(argument + i))) {
				printf("Not a valid argument");
				exit(0);
			}
		}
		return "001";
	}
}

int address_data(machine_code_type * machine_code, address_mode mode) {
	if (mode.first_arg == NULL) {
		if (mode.second_mode == NULL) {
			return 0;
		}
		else if (strcmp(mode.second_mode, "001") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_number(machine_code, mode.second_arg, 1);
			return 1;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_sign(machine_code, 1);
			return 1;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_reg(machine_code, mode.second_arg, 1);
			return 1;
		}
	}
	else if (strcmp(mode.first_arg, "001") == 0) {
		machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
		if (machine_code->machine_code == NULL) {
			printf("Unable to allocate memory");
			exit(0);
		}
		encode_number(machine_code, mode.first_arg, 1);
		if (mode.second_mode == NULL) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "001") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_number(machine_code, mode.second_arg, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_sign(machine_code, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_reg(machine_code, mode.second_arg, 2);
			return 2;
		}
	}
	else if (strcmp(mode.first_mode, "011") == 0) {
		machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
		if (machine_code->machine_code == NULL) {
			printf("Unable to allocate memory");
			exit(0);
		}
		encode_sign(machine_code, 1);
		if (mode.second_mode == NULL) {
			return 1;
		}
		else if (strcmp(mode.second_mode, "001") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_number(machine_code, mode.second_arg, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "011") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_sign(machine_code, 2);
			return 2;
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_reg(machine_code, mode.second_arg, 2);
			return 2;
		}
	}
	else if (strcmp(mode.first_mode, "101") == 0) {
		if (mode.second_mode == NULL) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_reg(machine_code, mode.first_arg, 1);
		}
		else if (strcmp(mode.second_mode, "101") == 0) {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_2_regs(machine_code, mode.first_arg, mode.second_arg, 1);
			return 1;
		}
		else {
			machine_code->machine_code = (char **)realloc(machine_code->machine_code, 2 * sizeof(*machine_code->machine_code));
			if (machine_code->machine_code == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			encode_reg(machine_code, mode.first_arg, 1);
			if (strcmp(mode.second_mode, "001") == 0) {
				machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
				if (machine_code->machine_code == NULL) {
					printf("Unable to allocate memory");
					exit(0);
				}
				encode_number(machine_code, mode.second_arg, 2);
				return 2;
			}
			else if (strcmp(mode.second_mode, "011") == 0) {
				machine_code->machine_code = (char **)realloc(machine_code->machine_code, 3 * sizeof(*machine_code->machine_code));
				if (machine_code->machine_code == NULL) {
					printf("Unable to allocate memory");
					exit(0);
				}
				encode_sign(machine_code, 2);
				return 2;
			}
		}
	}
}