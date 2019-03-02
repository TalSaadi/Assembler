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

#define WORD_SIZE 13
#define NULL_TER_BIT 12

machine_code_type first_process(char *line, int IC, int DC, sign_table_ptr * sign_head) {
	/*** This function gets a line, process it and building the machine code lines 
	     Based on command and arguments recived ***/
	int i, k, num, found_flag = 0, num_lines = 0;
	char * op, *code, *flag = "";
	address_mode mode;
	machine_code_type machine_code;
	machine_code.machine_code = 0;
	char machine_code_line[WORD_SIZE] = "";
	if (is_empty(line)) { /* Blank chars line */
		machine_code.machine_code = NULL;
		return machine_code;
	}
	line = clear_line(line);
	if (*line == ';') { /* Comment line */
		machine_code.machine_code = NULL;
		return machine_code;
	}
	else {
		machine_code.machine_code = (char **)realloc(machine_code.machine_code, (num_lines + 1) * sizeof(*machine_code.machine_code));
		if (machine_code.machine_code == NULL) {
			printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
			error_found = 1;
		}
		#pragma warning(suppress : 4996)
		line = strtok(line, " ");
		if (*(line + (strlen(line) - 1)) == ':') { /* Search for sign */
			clear_args(line);
			if (search_sign(sign_head, line)) { /* Check if sign already exists*/
				printf("Error in file: %s, line number %d, Sign already found\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			else if (!legal_sign(line)) {
				printf("Error in file: %s, line number %d, Not a legal sign\n", globalFileName, globalLineNum);
				error_found = 1;
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
				append(sign_head, flag, DC, 0, 0, 1); /* Add sign to table with place DC to mark data start */
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, ",");
			if (line == NULL) {
				printf("Error in file: %s, line number %d, No data was found\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			while (line != NULL) {
				machine_code.machine_code = (char **)realloc(machine_code.machine_code, (num_lines + 1) * sizeof(*machine_code.machine_code));
				if (machine_code.machine_code == NULL) {
					printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
					error_found = 1;
				}
				machine_code.machine_code[num_lines] = (char*)malloc(WORD_SIZE * sizeof(char));
				if (machine_code.machine_code[num_lines] == NULL) {
					printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
					error_found = 1;
				}
				num = atoi(line); /* Encode data to machine code */
				for (i = 11; i >= 0; num = num >> 1, i--) {
					machine_code.machine_code[num_lines][i] = (num & 1) + '0';
				}
				machine_code.machine_code[num_lines][NULL_TER_BIT] = '\0';
				num_lines++;
				#pragma warning(suppress : 4996)
				line = strtok(NULL, ",");
			}
			machine_code.isdata = 1;
		}
		else if (strcmp(line, ".string") == 0) {
			if (found_flag) {
				append(sign_head, flag, DC, 0, 0, 1);
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			clear_args(line);
			if ((*line == '"') && (*(line + (strlen(line) - 1)) == '"')) {
				line++;
				/* Encode ascii chars of string to machine code */
				while (*line != '"') {
					num = *line;
					machine_code.machine_code = realloc(machine_code.machine_code, (num_lines + 1) * sizeof(char *));
					if (machine_code.machine_code == NULL) {
						printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
						error_found = 1;
					}
					machine_code.machine_code[num_lines] = malloc(WORD_SIZE * sizeof(char));
					if (machine_code.machine_code[num_lines] == NULL) {
						printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
						error_found = 1;
					}
					for (k = 11; k >= 0; num = num >> 1, k--) {
						machine_code.machine_code[num_lines][k] = (num & 1) + '0';
					}
					machine_code.machine_code[num_lines][NULL_TER_BIT] = '\0';
					num_lines++;
					line++;
				}
				machine_code.machine_code = realloc(machine_code.machine_code, (num_lines + 1) * sizeof(char *));
				if (machine_code.machine_code == NULL) {
					printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
					error_found = 1;
				}
				machine_code.machine_code[num_lines] = malloc(WORD_SIZE * sizeof(char));
				if (machine_code.machine_code[num_lines] == NULL) {
					printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
					error_found = 1;
				}
				#pragma warning(suppress : 4996)
				strcpy(machine_code.machine_code[num_lines], "000000000000");
				num_lines++;
				machine_code.isdata = 1;
			}
			else {
				printf("Error in file: %s, line number %d, Wrong argument\n", globalFileName, globalLineNum);
				error_found = 1;
			}
		}
		else if (strcmp(line, ".extern") == 0) {
			if (found_flag) {
				printf("Error in file: %s, line number %d, Warning, found useless sign\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			append(sign_head, line, -1, 1, 0, 0); /* Add extern sign to table with no place (-1)*/
			#pragma warning(suppress : 4996)
			line = strtok(NULL, ",");
			if (line != NULL) {
				printf("Error in file: %s, line number %d, Too much arguments\n", globalFileName, globalLineNum);
				error_found = 1;
			}
		}
		else if (strcmp(line, ".entry") == 0) {
			machine_code.machine_code = NULL;
			return machine_code;
		}
		else {
			if (found_flag) {
				append(sign_head, flag, IC, 0, 0, 0);
			}
			#pragma warning(suppress : 4996)
			clear_args(line);
			op = search_code(line); /* Find opcode of command */
			if (op == NULL) {
				printf("Error in file: %s, line number %d, Code wasn't found\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			machine_code.machine_code[0] = malloc(WORD_SIZE * sizeof(char));
			if (machine_code.machine_code[0] == NULL) {
				printf("Error in file: %s, line number %d, Unable to allocate memory\n", globalFileName, globalLineNum);
				error_found = 1;
			}
			code = line;
			/* Build first line of machine code*/
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
			/* Build arguments lines after command */
			num_lines += address_data(&machine_code, mode);
			machine_code.isdata = 0;
		}
	}
	machine_code.num_lines = num_lines;
	return machine_code;
}