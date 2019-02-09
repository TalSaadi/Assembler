#include <stdio.h>
#include <string.h>
#include "Reader.h"
#include "DataTypes.h"

#define MAX_LINE_SIZE 80

void read(char filename[])
{
	int IC = 0, DC = 0;
	char * line;
	FILE *file;
	error * errors = NULL;
	error * current = errors;
	#pragma warning(suppress : 4996)
	file = fopen(strcat(filename, ".as"), "r");
	if (file) {
		while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
			process()
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