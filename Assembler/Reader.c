#include <stdio.h>
#include <string.h>
#include "Reader.h"
#include "DataTypes.h"

#define MAX_LINE_SIZE 80
#define OPCODE_LENGTH 16

int process(char *line);
int is_empty(char *line);
int search_sign(sign_table_ptr *head, char *sign);
char * search_code(opcode opcode_table[], char * code);
address_mode analyze_arguments(char * code, char * arguments);
int check_address_code(char * argument);

void read(char filename[])
{
	char line[MAX_LINE_SIZE];
	FILE *file;
	error * errors = NULL;
	error * current = errors;
	#pragma warning(suppress : 4996)
	file = fopen(strcat(filename, ".as"), "r");
	if (file) {
		while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
			process(line);
		}
		fclose(file);
		if (errors == NULL) {
			/* continue */
		}
		else {
			printf("Errors were found\n");
			return;
		}
	}
	else {
		printf("File not found\n");
	}
}

int process(char *line) {
	int IC = 0, DC = 0;
	char Instructions[1000][13];
	char Data[1000][13];
	opcode opcode_table[OPCODE_LENGTH] = { {"mov", "0000"}, {"cmp", "0001"}, {"add", "0010"}, {"sub", "0011"}, {"not", "0100"}, {"clr", "0101"}, {"lea", "0110"}, {"inc", "0111"}, {"dec", "1000"}, {"jmp", "1001"}, {"bne", "1010"}, {"red", "1011"}, {"prn", "1100"}, {"jsr", "1101"}, {"rts", "1110"}, {"stop", "1111"} };
	int found_flag = 0;
	char * flag = "";
	sign_table_ptr *sign_head = NULL;
	sign_table_ptr *sign_ptr = sign_head;
	address_mode mode;
	char machine_code_line[13] = "";
	if (is_empty(line)) {
		return 0;
	}
	else if (*line == ';') {
		return 0;
	}
	else {
		#pragma warning(suppress : 4996)
		line = strtok(line, " ");
	    if (*(line + (strlen(line) - 1)) == ':') {
			if (search_sign(sign_head, line)) {
				printf("Sign already found");
				exit();
			}
			found_flag = 1;
			flag = line;
		}
		#pragma warning(suppress : 4996)
		line = strtok(NULL, " ");
		if (strcmp(line, ".data") == 0) {
			if (found_flag) {
				sign_ptr = malloc(sizeof(sign_table_ptr));
				if (sign_ptr == NULL) {
					printf("Unable to allocate");
					exit();
				}
				sign_ptr->sign = flag;
				sign_ptr->place = DC;
				sign_ptr->next = malloc(sizeof(sign_table_ptr));
				sign_ptr = sign_ptr->next;
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, ",");
			if (line == NULL) {
				printf("No data was found");
				exit();
			}
			while (line != NULL) {
				int num;
				if (atoi(line) < 0) {
					num = ~atoi(line) + 1;
				}
				else {
					num = atoi(line);
				}
				for (int i = 0; i < 12; num = num >> 1, i++) {
					Data[DC][i] = (num & 1) + '0';
				}
				Data[DC][12] = '\0';
				DC++;
				#pragma warning(suppress : 4996)
				line = strtok(NULL, ",");
			}
		}
		else if (strcmp(line, ".string") == 0) {
			if (found_flag) {
				sign_ptr = malloc(sizeof(sign_table_ptr));
				if (sign_ptr == NULL) {
					printf("Unable to allocate");
					exit();
				}
				sign_ptr->sign = flag;
				sign_ptr->place = DC;
				sign_ptr->next = malloc(sizeof(sign_table_ptr));
				sign_ptr = sign_ptr->next;
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			if (*line == '"') {
				line++;
				while (*line != '"') {
					int num = *line;
					for (int i = 0; i < 12; num = num >> 1, i++) {
						Data[DC][i] = (num & 1) + '0';
					}
					Data[DC][12] = '\0';
					DC++;
					line++;
				}
				#pragma warning(suppress : 4996)
				strcpy(Data[DC], "000000000000");
				DC++;
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
					exit();
				}
				sign_ptr->sign = line;
				sign_ptr->isext = 1;	
				sign_ptr->next = malloc(sizeof(sign_table_ptr));
				sign_ptr = sign_ptr->next;
			}
		}
		else {
			if (found_flag) {
				sign_ptr = malloc(sizeof(sign_table_ptr));
				if (sign_ptr == NULL) {
					printf("Unable to allocate");
					exit();
				}
				sign_ptr->sign = flag;
				sign_ptr->iscode = 1;
				sign_ptr->place = IC;
				sign_ptr = sign_ptr->next;
			}
			#pragma warning(suppress : 4996)
			char * op = search_code(opcode_table, line);
			if (op == NULL) {
				printf("Code wasn't found");
				exit();
			}
			char * code = line;
			#pragma warning(suppress : 4996)
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
			strcpy(Data[DC], machine_code_line);
			DC++;
		}
	}
}

int is_empty(char *line) {
	while (*line != '\0') {
		if (!isspace((unsigned char)*line))
			return 0;
		line++;
	}
	return 1;
}

int search_sign(sign_table_ptr *head, char *sign) {
	sign_table_ptr *ptr = head;
	while (ptr != NULL) {
		if (head->sign == sign) {
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}

char * search_code(opcode opcode_table[], char * code) {
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
	if (strcmp(code, "mov") == 0 || strcmp(code, "add") || strcmp(code, "sub")) {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(NULL, " ");
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(NULL, ",");
		#pragma warning(suppress : 4996)
		if (strtok(NULL, ",") != NULL) {
			printf("Too much args");
			exit();
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit();
		}
		else {
			mode.first_mode = check_address_code(mode.first_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.second_mode, "001") == 0) {
				printf("Invalid mode");
				exit();
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
			exit();
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit();
		}
		else {
			mode.first_mode = check_address_code(mode.first_arg);
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
			exit();
		}
		if (mode.first_arg == NULL || mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit();
		}
		else {
			mode.first_mode = check_address_code(mode.first_arg);
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.first_mode, "011") != 0) {
				printf("Invalid mode");
			}
			else if (strcmp(mode.second_mode, "001") == 0) {
				printf("Invalid mode");
				exit();
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
			exit();
		}
		if (mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit();
		}
		else {
			mode.second_mode = check_address_code(mode.second_arg);
			if (strcmp(mode.second_mode, "001") == 0) {
				printf("Invalid arg");
				exit();
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
			exit();
		}
		if (mode.second_arg == NULL) {
			printf("Not enuogh args");
			exit();
		}
		else {
			mode.second_mode = check_address_code(mode.second_arg);
			return mode;
		}
	}
	else if (strcmp(code, "rts") == 0 || strcmp(code, "stop") == 0) {
		if (arguments != NULL || !is_empty(arguments)) {
			printf("Too much args");
			exit();
		}
	}
}

int check_address_code(char * argument) {
	if (*argument == '@') {
		return "101";
	}
	else if (isalpha(*argument)) {
		return "011";
	}
	else {
		for (int i = 0; i < strlen(argument); i++) {
			if (!isdigit(*(argument + i)) || *(argument + i) != '+' || *(argument + i) != '-') {
				printf("Not a valid argument");
				exit();
			}
		}
		return "001";
	}
}