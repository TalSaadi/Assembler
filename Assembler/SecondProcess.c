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

int second_process(char Instructions[MEMORY_SIZE][WORD_SIZE], char *line, sign_table_ptr * sign_head, int IC) {
	char * sign, * code;
	address_mode mode;
	if (is_empty(line)) {
		return IC;
	}
	line = clear_line(line);
	if (*line == ';') {
		return IC;
	}
	#pragma warning(suppress : 4996)
	line = strtok(line, " ");
	if (*(line + (strlen(line) - 1)) == ':') {
		#pragma warning(suppress : 4996)
		line = strtok(NULL, " ");
	}
	if ((strcmp(line, ".data") == 0) || (strcmp(line, ".string") == 0) || (strcmp(line, ".extern") == 0)) {
		return IC;
	}
	else if (strcmp(line, ".entry") == 0) {
		#pragma warning(suppress : 4996)
		sign = strtok(NULL, " ");
		clear_args(sign);
		if (sign == NULL) {
			printf("Not enough args\n");
			exit(0);
		}
		else if (!legal_sign(sign)) {
			printf("Not a legal sign\n");
			exit(0);
		}
		if (!mark_entry(sign_head, sign)) {
			printf("Sign wasn't found\n");
			exit(0);
		}
		else {
			return IC;
		}
	}
	else {
		clear_args(line);
		code = line;
		mode = analyze_arguments(code, line);
		IC++;
		IC += address_data_second_process((char*)Instructions, mode, sign_head, IC);
	}
	return IC;
}