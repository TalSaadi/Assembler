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

#define WORD_SIZE 13

machine_code_type first_process(char *line, int IC, int DC, sign_table_ptr * sign_head) {
	int i, k, num, found_flag = 0, num_lines = 0;
	char * op, *code, *flag = "";
	address_mode mode;
	machine_code_type machine_code;
	machine_code.machine_code = 0;
	char machine_code_line[WORD_SIZE] = "";
	if (is_empty(line)) {
		machine_code.machine_code = NULL;
		return machine_code;
	}
	else if (*line == ';') {
		machine_code.machine_code = NULL;
		return machine_code;
	}
	else {
		machine_code.machine_code = (char **)realloc(machine_code.machine_code, (num_lines + 1) * sizeof(*machine_code.machine_code));
		if (machine_code.machine_code == NULL) {
			printf("Unable to allocate memory");
			exit(0);
		}
		#pragma warning(suppress : 4996)
		line = strtok(line, " ");
		if (*(line + (strlen(line) - 1)) == ':') {
			if (search_sign(sign_head, line)) {
				printf("Sign already found");
				exit(0);
			}
			else if (!legal_sign(line)) {
				printf("Not a legal sign");
				exit(0);
			}
			else {
				found_flag = 1;
				flag = line;
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
		}
		if (strcmp(line, ".data") == 0) {
			if (found_flag) {
				append(sign_head, flag, DC, 0, 0);
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, ",");
			if (line == NULL) {
				printf("No data was found");
				exit(0);
			}
			while (line != NULL) {
				machine_code.machine_code = (char **)realloc(machine_code.machine_code, (num_lines + 1) * sizeof(*machine_code.machine_code));
				if (machine_code.machine_code == NULL) {
					printf("Unable to allocate memory");
					exit(0);
				}
				machine_code.machine_code[num_lines] = (char*)malloc(WORD_SIZE * sizeof(char));
				if (machine_code.machine_code[num_lines] == NULL) {
					printf("Unable to allocate memory");
					exit(0);
				}
				if (atoi(line) < 0) {
					num = ~atoi(line) + 1;
				}
				else {
					num = atoi(line);
				}
				for (i = 0; i < 12; num = num >> 1, i++) {
					machine_code.machine_code[num_lines][i] = (num & 1) + '0';
				}
				machine_code.machine_code[num_lines][12] = '\0';
				num_lines++;
				#pragma warning(suppress : 4996)
				line = strtok(NULL, ",");
			}
			machine_code.isdata = 1;
		}
		else if (strcmp(line, ".string") == 0) {
			if (found_flag) {
				append(sign_head, flag, DC, 0, 0);
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			if (*line == '"') {
				line++;
				while (*line != '"') {
					num = *line;
					machine_code.machine_code = realloc(machine_code.machine_code, (num_lines + 1) * sizeof(char *));
					if (machine_code.machine_code == NULL) {
						printf("Unable to allocate memory");
						exit(0);
					}
					machine_code.machine_code[num_lines] = malloc(WORD_SIZE * sizeof(char));
					if (machine_code.machine_code[num_lines] == NULL) {
						printf("Unable to allocate memory");
						exit(0);
					}
					for (k = 0; k < 12; num = num >> 1, k++) {
						machine_code.machine_code[num_lines][k] = (num & 1) + '0';
					}
					machine_code.machine_code[num_lines][12] = '\0';
					num_lines++;
					line++;
				}
				machine_code.machine_code = realloc(machine_code.machine_code, (num_lines + 1) * sizeof(char *));
				if (machine_code.machine_code == NULL) {
					printf("Unable to allocate memory");
					exit(0);
				}
				machine_code.machine_code[num_lines] = malloc(WORD_SIZE * sizeof(char));
				if (machine_code.machine_code[num_lines] == NULL) {
					printf("Unable to allocate memory");
					exit(0);
				}
				#pragma warning(suppress : 4996)
				strcpy(machine_code.machine_code[num_lines], "000000000000");
				num_lines++;
				machine_code.isdata = 1;
			}
			else {
				printf("Wrong argument");
			}
		}
		else if (strcmp(line, ".extern") == 0) {
			if (found_flag) {
				printf("Warning, found useless sign");
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			if (line != NULL) {
				if (search_sign(sign_head, line)) {
					printf("Sign already found");
					exit(0);
				}
				append(sign_head, flag, DC, 1, 0);
			}
		}
		else {
			if (found_flag) {
				append(sign_head, flag, IC, 0, 1);
			}
			#pragma warning(suppress : 4996)
			op = search_code(line);
			if (op == NULL) {
				printf("Code wasn't found");
				exit(0);
			}
			machine_code.machine_code[0] = malloc(WORD_SIZE * sizeof(char));
			if (machine_code.machine_code[0] == NULL) {
				printf("Unable to allocate memory");
				exit(0);
			}
			code = line;
			mode = analyze_arguments(code, line);
			#pragma warning(suppress : 4996)
			strcat(machine_code_line, mode.first_mode);
			#pragma warning(suppress : 4996)
			strcat(machine_code_line, op);
			#pragma warning(suppress : 4996)
			strcat(machine_code_line, mode.second_mode);
			#pragma warning(suppress : 4996)
			strcat(machine_code_line, "00");
			#pragma warning(suppress : 4996)
			strcpy(machine_code.machine_code[0], machine_code_line);
			num_lines++;
			num_lines += address_data(&machine_code, mode);
			machine_code.isdata = 0;
		}
	}
	machine_code.num_lines = num_lines;
	return machine_code;
}