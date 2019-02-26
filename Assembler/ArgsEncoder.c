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
#define LETTER 6

const char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void encode_number(machine_code_type * machine_code, char * arg, int line) {
	int num, i;
	machine_code->machine_code[line] = malloc(WORD_SIZE * sizeof(char));
	if (machine_code->machine_code[line] == NULL) {
		printf("Unable to allocate memory");
		exit(0);
	}
	if (atoi(arg) < 0) {
		num = ~atoi(arg) + 1;
	}
	else {
		num = atoi(arg);
	}
	machine_code->machine_code[line][0] = '0';
	machine_code->machine_code[line][1] = '0';
	for (i = 2; i < 12; num = num >> 1, i++) {
		machine_code->machine_code[line][i] = (num & 1) + '0';
	}
	machine_code->machine_code[line][12] = '\0';
}

void encode_reg(machine_code_type * machine_code, char * reg, int line) {
	int num, i;
	machine_code->machine_code[line] = malloc(WORD_SIZE * sizeof(char));
	if (machine_code->machine_code[line] == NULL) {
		printf("Unable to allocate memory");
		exit(0);
	}
	machine_code->machine_code[line][0] = '0';
	machine_code->machine_code[line][1] = '0';
	num = reg[2] - '0';
	for (i = 2; i < 7; i++) {
		machine_code->machine_code[line][i] = '0';
	}
	for (i = 11; i >= 7; num = num >> 1, i--) {
		machine_code->machine_code[line][i] = (num & 1) + '0';
	}
	machine_code->machine_code[line][12] = '\0';
}

void encode_2_regs(machine_code_type * machine_code, char * reg1, char * reg2) {
	int num, i;
	machine_code->machine_code[1] = malloc(WORD_SIZE * sizeof(char));
	if (machine_code->machine_code[1] == NULL) {
		printf("Unable to allocate memory");
		exit(0);
	}
	machine_code->machine_code[1][0] = '0';
	machine_code->machine_code[1][1] = '0';
	num = reg1[2] - '0';
	for (i = 2; i < 7; num = num >> 1, i++) {
		machine_code->machine_code[1][i] = (num & 1) + '0';
	}
	num = reg2[2] - '0';
	for (i = 11; i >= 7; num = num >> 1, i--) {
		machine_code->machine_code[1][i] = (num & 1) + '0';
	}
	machine_code->machine_code[1][12] = '\0';
}

void encode_sign(machine_code_type *machine_code, int line) {
	int i;
	machine_code->machine_code[line] = malloc(WORD_SIZE * sizeof(char));
	if (machine_code->machine_code[line] == NULL) {
		printf("Unable to allocate memory");
		exit(0);
	}
	for (i = 0; i < 12; i++) {
		machine_code->machine_code[line][i] = '?';
	}
	machine_code->machine_code[line][12] = '\0';
}

char *b64_encode(char *line) {
	char firstletter[LETTER], b64[3];	
	char *secondletter;
	int num1, num2, firstbin, secondbin;

	memset(firstletter, '\0', sizeof(firstletter));
	#pragma warning(suppress : 4996)
	strncpy(firstletter, line, LETTER);
	secondletter = line + LETTER;
	num1 = atoi(firstletter);
	num2 = atoi(secondletter);
	firstbin = binaryToDecimal(num1);
	secondbin = binaryToDecimal(num2);
	b64[0] = b64chars[firstbin];
	b64[1] = b64chars[secondbin];
	b64[2] = '\0';
	#pragma warning(suppress : 4996)
	return b64;
}

int binaryToDecimal(int n)
{
	int num = n;
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0 
	int base = 1;

	int temp = num;
	while (temp)
	{
		int last_digit = temp % 10;
		temp = temp / 10;
		dec_value += last_digit * base;
		base = base * 2;
	}

	return dec_value;
}