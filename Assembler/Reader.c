#include <stdio.h>
#include <string.h>
#include "Reader.h"
#include "DataTypes.h"

#define MAX_LINE_SIZE 80
#define OPCODE_LENGTH 16

machine_code_type * process(char *line, int IC, int DC, sign_table_ptr * sign_head);
int is_empty(char *line);
int search_sign(sign_table_ptr *head, char *sign);
char * search_code(char * code);
address_mode analyze_arguments(char * code, char * arguments);
int check_address_code(char * argument);
sign_table_ptr* create(char * flag, int DC, int isext, int iscode, sign_table_ptr* next);
void append(sign_table_ptr *head, char * flag, int DC, int isext, int iscode);
int legal_sign(char *sign);
int legal_reg(char *reg);
void free_array(int **a, int m);


void read(char filename[])
{
	char line[MAX_LINE_SIZE];
	FILE *file;
	error * errors = NULL;
	error * current = errors;
	machine_code_type * machine_code;
	sign_table_ptr sign_head;
	sign_head.sign;
	int IC = 0, DC = 0;
	char Instructions[1000][13];
	char Data[1000][13];
	#pragma warning(suppress : 4996)
	file = fopen(strcat(filename, ".as"), "r");
	if (file) {
		while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
			machine_code = process(line, IC, DC, &sign_head);
			for (int i = 0; i < machine_code->num_lines; i++) {
				if (machine_code == NULL) {
					continue;
				}
				else if (machine_code->isdata) {
					#pragma warning(suppress : 4996)
					strcpy(Data[DC], machine_code->machine_code[i]);
					DC++;
				}
				else {
					#pragma warning(suppress : 4996)
					strcpy(Instructions[IC], machine_code->machine_code[i]);
					IC++;
				}
			}
			free_array(machine_code->machine_code, machine_code->num_lines);
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

machine_code_type * process(char *line, int IC, int DC, sign_table_ptr * sign_head) {	
	int found_flag = 0;
	char * flag = "";
	address_mode mode;
	machine_code_type machine_code;
	char ** remachine_code;
	int num_lines = 0;
	char machine_code_line[13] = "";
	if (is_empty(line)) {
		return &machine_code;
	}
	else if (*line == ';') {
		return &machine_code;
	}
	else {
		machine_code.machine_code = (char **)malloc((num_lines + 1) * sizeof(char*));
		if (machine_code.machine_code == NULL) {
			printf("Unable to allocate memory");
			exit();
		}
		#pragma warning(suppress : 4996)
		line = strtok(line, " ");
	    if (*(line + (strlen(line) - 1)) == ':') {
			if (search_sign(sign_head, line)) {
				printf("Sign already found");
				exit();
			}
			else if (!legal_sign(line)) {
				printf("Not a legal sign");
				exit();
			}
			else {
				found_flag = 1;
				flag = line;
			}
		}
		#pragma warning(suppress : 4996)
		line = strtok(NULL, " ");
		if (strcmp(line, ".data") == 0) {
			if (found_flag) {
				append(sign_head, flag, DC, 0, 0);
			}
			#pragma warning(suppress : 4996)
			line = strtok(NULL, ",");
			if (line == NULL) {
				printf("No data was found");
				exit();
			}
			while (line != NULL) {
				machine_code.machine_code[num_lines] = (char*)malloc(13 * sizeof(char));
				if (machine_code.machine_code[num_lines] == NULL) {
					printf("Unable to allocate memory");
					exit();
				}
				int num;
				if (atoi(line) < 0) {
					num = ~atoi(line) + 1;
				}
				else {
					num = atoi(line);
				}
				for (int i = 0; i < 12; num = num >> 1, i++) {
					machine_code.machine_code[num_lines][i] = (num & 1) + '0';
				}
				machine_code.machine_code[num_lines][12] = '\0';
				num_lines++;
				machine_code.machine_code = (char **)realloc(machine_code.machine_code, (num_lines + 1) * sizeof(*machine_code.machine_code));
				if (machine_code.machine_code == NULL) {
					printf("Unable to allocate memory");
					exit();
				}
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
					int num = *line;
					machine_code.machine_code[num_lines] = malloc(13 * sizeof(char));
					if (machine_code.machine_code[num_lines] == NULL) {
						printf("Unable to allocate memory");
						exit();
					}
					for (int i = 0; i < 12; num = num >> 1, i++) {
						machine_code.machine_code[num_lines][i] = (num & 1) + '0';
					}
					machine_code.machine_code[num_lines][12] = '\0';
					num_lines++;
					machine_code.machine_code = realloc(machine_code.machine_code, (num_lines + 1) * sizeof(char *));			
					if (machine_code.machine_code == NULL) {
						printf("Unable to allocate memory");
						exit();
					}
					line++;
				}
				machine_code.machine_code[num_lines] = malloc(13 * sizeof(char));
				if (machine_code.machine_code[num_lines] == NULL) {
					printf("Unable to allocate memory");
					exit();
				}
				#pragma warning(suppress : 4996)
				strcpy(machine_code.machine_code[num_lines], "000000000000");
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
					exit();
				}
				append(sign_head, flag, DC, 1, 0);
			}
		}
		else {
			if (found_flag) {
				append(sign_head, flag, IC, 0, 1);
			}
			#pragma warning(suppress : 4996)
			char * op = search_code(line);
			if (op == NULL) {
				printf("Code wasn't found");
				exit();
			}
			machine_code.machine_code[0] = malloc(13 * sizeof(char));
			if (machine_code.machine_code[0] == NULL) {
				printf("Unable to allocate memory");
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
			strcpy(machine_code.machine_code[0], machine_code_line);
			num_lines++;
			machine_code.isdata = 0;
		}
	}
	machine_code.num_lines = num_lines;
	return &machine_code;
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
	if (ptr->sign == NULL || strcmp(ptr->sign, sign) == 0) {
		return 0;
	}
	else {
		ptr = ptr->next;
	}
	while (ptr != NULL) {
		if (strcmp(ptr->sign, sign) == 0) {
			return 1;
		}
		ptr = ptr->next;
	}
	return 0;
}

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
		if (legal_reg(argument + 1)) {
			return "101";
		}
		else {
			printf("Not a legal register");
			exit();
		}
		
	}
	else if (isalpha(*argument)) {
		if (legal_sign(argument)) {
			return "011";
		}
		else {
			printf("Not a legal sign");
			exit();
		}
	}
	else {
		if (!isdigit(*(argument)) || *(argument) != '+' || *(argument) != '-') {
			printf("Not a valid argument");
			exit();
		}
		for (int i = 0; i < strlen(argument); i++) {
			if (!isdigit(*(argument + i))) {
				printf("Not a valid argument");
				exit();
			}
		}
		return "001";
	}
}

void append(sign_table_ptr* head, char * flag, int place, int isext, int iscode)
{
	/* go to the last node */
	sign_table_ptr *cursor = head;
	if (head->sign == NULL) {
		/*head = (sign_table_ptr*)malloc(sizeof(sign_table_ptr));
		if (head == NULL) {
			printf("Unable to allocate");
			exit();
		}
		else {*/
		head->sign = (char*)malloc(sizeof(flag));
		if (head->sign == NULL) {
			printf("Unable to allocate memory");
			exit();
		}
		#pragma warning(suppress : 4996)
		strcpy(head->sign, flag);
		head->place = place;
		head->isext = isext;
		head->iscode = iscode;
		head->next = NULL;
		return;
		/*}*/
	}
	while (cursor->next != NULL)
		cursor = cursor->next;

	/* create a new node */
	sign_table_ptr* new_node = create(flag, place, isext, iscode, NULL);
	cursor->next = new_node;
}

sign_table_ptr* create(char * flag, int DC, int isext, int iscode, sign_table_ptr* next)
{
	sign_table_ptr* new_node = (sign_table_ptr*)malloc(sizeof(sign_table_ptr));
	if (new_node == NULL)
	{
		printf("Error creating a new node.\n");
		exit(0);
	}
	new_node->sign = (char *)malloc(sizeof(flag));
	if (new_node->sign == NULL) {
		printf("Unable to allocate memory");
		exit();
	}
	#pragma warning(suppress : 4996)
	strcpy(new_node->sign, flag);
	new_node->place = DC;
	new_node->isext = isext;
	new_node->iscode = iscode;
	new_node->next = next;

	return new_node;
}

int legal_sign(char *sign) {
	if (legal_reg(sign)) {
		return 0;
	}
	else if (search_code(sign) != NULL) {
		return 0;
	}
	else {
		return 1;
	}
}

int legal_reg(char *reg) {
	if (*reg == 'r') {
		if (isdigit(*(reg + 1)) && *(reg + 1) - '0' <= 7) {
			return 1;
		}
	}
	return 0;
}

void free_array(int **a, int m) {
	int i;
	for (i = 0; i < m; ++i) {
		free(a[i]);
	}
	free(a);
}