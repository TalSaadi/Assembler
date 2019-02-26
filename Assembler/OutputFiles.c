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
#define MEMORY_SIZE 1024

void ObjectFile(char Instructions[MEMORY_SIZE][WORD_SIZE], char Data[MEMORY_SIZE][WORD_SIZE], int IC, int DC, char * filename) {
	int i;
	FILE *fp;
	char *outputname;
	char *b64line = malloc(3 * sizeof(char));
	outputname = malloc(strlen(filename) * sizeof(char));
	#pragma warning(suppress : 4996)
	strcpy(outputname, filename);
	#pragma warning(suppress : 4996)
	fp = fopen(strcat(outputname, ".ob"), "w");
	fprintf(fp, "%d %d\n", IC, DC);
	for (i = 0; i < IC; i++) {
		b64line = b64_encode(Instructions[i]);
		fprintf(fp, "%c%c\n", b64line[0], b64line[1]);
	}
	for (i = 0; i < DC; i++) {
		b64line = b64_encode(Data[i]);
		fprintf(fp, "%c%c\n", b64line[0], b64line[1]);
	}
	fclose(fp);
}