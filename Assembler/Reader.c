#include <stdio.h>
#include <string.h>
#include "Reader.h"
#include "DataTypes.h"

#define MAX_LINE_SIZE 80
#define OPCODE_LENGTH 16

int process(char *line);
int is_empty(char *line);
int search_sign(sign_table_ptr *head, char *sign);
int search_code(opcode opcode_table[], char * code);
address_mode analyze_arguments(char * code, char * arguments);
int check_address_code(char * argument);

void read(char filename[])
{
	int IC = 0, DC = 0;
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
	opcode opcode_table[OPCODE_LENGTH] = { {"mov", "0000"}, {"cmp", "0001"}, {"add", "0010"}, {"sub", "0011"}, {"not", "0100"}, {"clr", "0101"}, {"lea", "0110"}, {"inc", "0111"}, {"dec", "1000"}, {"jmp", "1001"}, {"bne", "1010"}, {"red", "1011"}, {"prn", "1100"}, {"jsr", "1101"}, {"rts", "1110"}, {"stop", "1111"} };
	int IC = 0, DC = 0;
	int found_flag = 0;
	char * flag = "";
	sign_table_ptr *sign_head = NULL;
	sign_head = malloc(sizeof(sign_table_ptr));
	data_image *data_head = NULL;
	data_head = malloc(sizeof(data_image));
	code_image *code_head = NULL;
	code_head = malloc(sizeof(data_image));
	if (sign_head == NULL || data_head == NULL || code_head == NULL) {
		return 1;
	}
	sign_table_ptr *sign_ptr = sign_head;
	data_image *data_ptr = data_head;
	code_image *code_ptr = code_head;
	address_mode mode;
	char machine_code[1000][13];
	int machine_code_free_line = 0;
	char * machine_code_line;
	if (is_empty(line)) {
		return 0;
	}
	else if (*line == ";") {
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
		if (line == ".data") {
			if (found_flag) {
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
				data_ptr->data = line;
				data_ptr->next = malloc(sizeof(data_image));
				data_ptr = data_ptr->next;
				DC++;
				#pragma warning(suppress : 4996)
				line = strtok(NULL, ",");
			}
		}
		else if (line == ".string") {
			if (found_flag) {
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
					data_ptr->data = *line * 10;
					data_ptr->next = malloc(sizeof(data_image));
					data_ptr = data_ptr->next;
					DC++;
					line++;
				}
			}
			else {
				printf("Wrong argument");
			}
		}
		else if (line == ".extern") {
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
				sign_ptr->sign = flag;
				sign_ptr->iscode = 1;
				sign_ptr->place = IC;
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			int op = search_code(opcode_table, line);
			if (op == NULL) {
				printf("Code wasn't found");
				exit();
			}
			char * code = line;
			#pragma warning(suppress : 4996)
			line = strtok(NULL, " ");
			mode = analyze_arguments(code, line);
			#pragma warning(suppress : 4996)
			machine_code_line = strcat(mode.first_mode, strcat(code, strcat(mode.second_mode, "00")));
			#pragma warning(suppress : 4996)
			strcpy(machine_code[machine_code_free_line], machine_code_line);
			machine_code_free_line++;
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

int search_code(opcode opcode_table[], char * code) {
	int i;
	for (i = 0; i < OPCODE_LENGTH; i++) {
		if (opcode_table[i].code = code) {
			return opcode_table[i].code;
		}
	}
	return NULL;
}

address_mode analyze_arguments(char * code, char * arguments) {
	address_mode mode;
	if (code == "mov" || code == "add" || code == "sub") {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(arguments, ",");
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
			if (mode.second_mode == 0b001) {
				printf("Invalid mode");
				exit();
			}
			else {
				return mode;
			}
		}
	}
	else if (code == "cmp") {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(arguments, ",");
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
	else if (code == "lea") {
		#pragma warning(suppress : 4996)
		mode.first_arg = strtok(arguments, ",");
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
			if (mode.first_mode != 0b011) {
				printf("Invalid mode");
			}
			else if (mode.second_mode == 0b001) {
				printf("Invalid mode");
				exit();
			}
			else {
				return mode;
			}
		}
	}
	else if (code == "not" || code == "clr" || code == "inc" || code == "dec" || code == "jmp" || code == "bne" || code == "red" || code == "jsr") {
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(arguments, ",");
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
			if (mode.second_mode == 0b001) {
				printf("Invalid arg");
				exit();
			}
			return mode;
		}
	}
	else if (code == "prn") {
		#pragma warning(suppress : 4996)
		mode.second_arg = strtok(arguments, ",");
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
	else if (code == "rts" || code == "stop") {
		if (arguments != NULL || !is_empty(arguments)) {
			printf("Too much args");
			exit();
		}
	}
}

int check_address_code(char * argument) {
	if (*argument == "@") {
		return 0b101;
	}
	else if (isalpha(*argument)) {
		return 0b011;
	}
	else {
		for (int i = 0; i < strlen(argument); i++) {
			if (!isdigit(*(argument + i)) || *(argument + i) != "+" || *(argument + i) != "-") {
				printf("Not a valid argument");
				exit();
			}
		}
		return 0b001;
	}
}