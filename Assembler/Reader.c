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

#define MAX_LINE_SIZE 80
#define MEMORY_SIZE 1024
#define WORD_SIZE 13


void read(char filename[])
{
	char line[MAX_LINE_SIZE];
	FILE *file;
	error * errors = NULL;
	error * current = errors;
	machine_code_type machine_code;
	int i, IC = 0, DC = 0;
	char Instructions[MEMORY_SIZE][WORD_SIZE];
	char Data[MEMORY_SIZE][WORD_SIZE];
	sign_table_ptr sign_head;
	sign_head.sign = NULL;
	#pragma warning(suppress : 4996)
	file = fopen(strcat(filename, ".as"), "r");
	if (file) {
		while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
			machine_code = first_process(line, IC, DC, &sign_head);
			if (machine_code.machine_code == NULL) {
				continue;
			}
			for (i = 0; i < machine_code.num_lines; i++) {
				if (machine_code.isdata) {
					#pragma warning(suppress : 4996)
					strcpy(Data[DC], machine_code.machine_code[i]);
					DC++;
				}
				else {
					#pragma warning(suppress : 4996)
					strcpy(Instructions[IC], machine_code.machine_code[i]);
					IC++;
				}
			}
			free_array(machine_code.machine_code, machine_code.num_lines);
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
	free_sign_table(&sign_head);
}