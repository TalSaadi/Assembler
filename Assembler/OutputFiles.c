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
#define B64_SIZE 3
#define MEMORY_SIZE 1024

void ObjectFile(char Instructions[MEMORY_SIZE][WORD_SIZE], char Data[MEMORY_SIZE][WORD_SIZE], int IC, int DC, char * filename) {
	int i;
	FILE *fp;
	char *outputname;
	char *b64line = malloc(B64_SIZE * sizeof(char));
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

void EntriesFile(sign_table_ptr * sign_head, char * filename) {
	int found_ent = 0;
	FILE *fp;
	char *outputname;
	outputname = malloc(strlen(filename) * sizeof(char));
	#pragma warning(suppress : 4996)
	strcpy(outputname, filename);
	#pragma warning(suppress : 4996)
	fp = fopen(strcat(outputname, ".ent"), "w");
	sign_table_ptr *ptr = sign_head;
	if (ptr->sign == NULL) {
		fclose(fp);
	}
	else {
		while (ptr != NULL) {
			if (ptr->isent) {
				fprintf(fp, "%s %d\n", ptr->sign, ptr->place);
				found_ent = 1;
			}
			ptr = ptr->next;
		}
		fclose(fp);
	}
	if (!found_ent) {
		if (remove(outputname) == 0)
			printf("Deleted successfully\n");
		else {
			printf("Unable to delete the file\n");
			exit(0);
		}
	}
}

void ExternsFile(sign_table_ptr * sign_head, char *filename) {
	int i, found_ext = 0;
	FILE *fp;
	char *outputname;
	outputname = malloc(strlen(filename) * sizeof(char));
	#pragma warning(suppress : 4996)
	strcpy(outputname, filename);
	#pragma warning(suppress : 4996)
	fp = fopen(strcat(outputname, ".ext"), "w");
	sign_table_ptr *ptr = sign_head;
	if (ptr->sign == NULL) {
		fclose(fp);
	}
	else {
		while (ptr != NULL) {
			if (ptr->isext) {
				for (i = 0; i < ptr->num_ref; i++) {
					fprintf(fp, "%s %d\n", ptr->sign, *(ptr->references + i));
					found_ext = 1;
				}
			}
			ptr = ptr->next;
		}
		fclose(fp);
	}
	if (!found_ext) {
		if (remove(outputname) == 0)
			printf("Deleted successfully\n");
		else {
			printf("Unable to delete the file\n");
			exit(0);
		}
	}
}