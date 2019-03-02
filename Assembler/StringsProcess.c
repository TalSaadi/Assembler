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

int is_empty(char *line) {
	while (*line != '\0') {
		if (!isspace((unsigned char)*line))
			return 0;
		line++;
	}
	return 1;
}

int legal_sign(char *sign) {
	if (legal_reg(sign)) {
		return 0;
	}
	else if (search_code(sign) != NULL) {
		return 0;
	}
	else if (strLen(sign) > 31) {
		return 0;
	}
	else if (strcmp(sign, ".data") == 0 || strcmp(sign, ".string") == 0 || strcmp(sign, ".extern") == 0 || strcmp(sign, ".entry") == 0) {
		return 0;
	}
	else if (strcmp(sign, ".data:") == 0 || strcmp(sign, ".string:") == 0 || strcmp(sign, ".extern:") == 0 || strcmp(sign, ".entry:") == 0) {
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

void clear_args(char * arg) {
	/* To keep track of non-space character count */
	int count = 0;

	/*** Traverse the given string. If current character
	is not space, then place it at index 'count++' ***/
	for (int i = 0; arg[i]; i++)
		if (arg[i] != ' ' && arg[i] != '\t' && arg[i] != ',' && arg[i] != '\n' && arg[i] != ':')
			arg[count++] = arg[i]; /* here count is incremented */
	arg[count] = '\0';
}

int strLen(char *s)
{
	int length;
	char *p = s;
	while (*p != ':' && *p != '\0')
	{
		p++;  /* increase the address until the end */
	}
	length = p - s;
	return length; /* Subtract the two addresses, end - start */
}