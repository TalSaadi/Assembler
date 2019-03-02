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
#include "OutputFiles.h"
#include "SecondProcess.h"

#define MAX_LINE_SIZE 82
#define MEMORY_SIZE 1024
#define WORD_SIZE 13

int globalLineNum = 0; /* current file line number for errors */
char * globalFileName; /* current file name for errors */
int error_found = 0; /* check if errors were found */

void read(char filename[])
{
	char * asfilename;
	char line[MAX_LINE_SIZE];
	FILE *file;
	machine_code_type machine_code;
	int i, IC = 0, DC = 0;
	char Instructions[MEMORY_SIZE][WORD_SIZE];
	char Data[MEMORY_SIZE][WORD_SIZE];
	sign_table_ptr sign_head;
	sign_head.sign = NULL;
	globalFileName = malloc(strlen(filename) * sizeof(char));
	#pragma warning(suppress : 4996)
	strcpy(globalFileName, filename);
	asfilename = malloc(strlen(filename) * sizeof(char));
	#pragma warning(suppress : 4996)
	strcpy(asfilename, filename);
	#pragma warning(suppress : 4996)
	file = fopen(strcat(asfilename, ".as"), "r");
	/* First Process */
	if (file) {
		while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
			globalLineNum++;
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
	}
	else {
		printf("File not found\n");
		return;
	}
	if (error_found) { /* Stop if errors were found in first process */
		exit(0);
	}
	shift_places(&sign_head, IC); /* Shift all signs places by 100 and add IC to data */
	int Second_IC = 0;
	#pragma warning(suppress : 4996)
	file = fopen(asfilename, "r");
	/* Second Process */
	if (file) {
		while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
			Second_IC = second_process(Instructions, line, &sign_head, Second_IC);
		}

	}
	if (error_found) { /* Stop if errors were found in second process */
		exit(0);
	}
	ObjectFile(Instructions, Data, IC, DC, filename); /* Create object file */
	EntriesFile(&sign_head, filename); /* create entries file */
	ExternsFile(&sign_head, filename); /* create externs file */
	free_sign_table(&sign_head);
}