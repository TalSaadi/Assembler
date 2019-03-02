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

#define START 100

void append(sign_table_ptr* head, char * flag, int place, int isext, int isent, int isdata)
{
	clear_args(flag);
	/* go to the last node */
	sign_table_ptr *cursor = head;
	sign_table_ptr* new_node;
	size_t length;
	if (head->sign == NULL) {
		length = strlen(flag);
		head->sign = (char*)malloc(length + 1);
		if (head->sign == NULL) {
			printf("Unable to allocate memory\n");
			exit(0);
		}
		#pragma warning(suppress : 4996)
		strcpy(head->sign, flag);
		head->place = place;
		head->isext = isext;
		head->isent = isent;
		head->isdata = isdata;
		head->references = NULL;
		head->num_ref = 0;
		head->next = NULL;
		return;
	}
	while (cursor->next != NULL)
		cursor = cursor->next;

	/* create a new node */
	new_node = create(flag, place, isext, isent, isdata, NULL);
	cursor->next = new_node;
}

sign_table_ptr* create(char * flag, int place, int isext, int isent, int isdata, sign_table_ptr* next)
{
	sign_table_ptr* new_node = (sign_table_ptr*)malloc(sizeof(sign_table_ptr));
	size_t length;
	if (new_node == NULL)
	{
		printf("Error creating a new node.\n");
		exit(0);
	}
	length = strlen(flag);
	new_node->sign = (char *)malloc(length + 1);
	if (new_node->sign == NULL) {
		printf("Unable to allocate memory\n");
		exit(0);
	}
	#pragma warning(suppress : 4996)
	strcpy(new_node->sign, flag);
	new_node->place = place;
	new_node->isext = isext;
	new_node->isent = isent;
	new_node->isdata = isdata;
	new_node->references = NULL;
	new_node->num_ref = 0;
	new_node->next = next;

	return new_node;
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

int mark_entry(sign_table_ptr *head, char *sign) {
	sign_table_ptr *ptr = head;
	if (ptr->sign == NULL) {
		return 0;
	}
	else {
		while (ptr != NULL) {
			if (strcmp(ptr->sign, sign) == 0) {
				ptr->isent = 1;
				return 1;
			}
			ptr = ptr->next;
		}
	}
	return 0;
}

int sign_place(sign_table_ptr *head, char *sign) {
	sign_table_ptr *ptr = head;
	if (ptr->sign == NULL) {
		return 0;
	}
	else {
		while (ptr != NULL) {
			if (strcmp(ptr->sign, sign) == 0) {
				return ptr->place;
			}
			ptr = ptr->next;
		}
	}
	return 0;
}

void shift_places(sign_table_ptr * head, int IC) {
	sign_table_ptr *ptr = head;
	if (ptr->sign == NULL) {
		return;
	}
	else {
		while (ptr != NULL) {
			if (ptr->place != -1) {
				if (ptr->isdata) {
					ptr->place += (IC + START);
				}
				else {
					ptr->place += START;
				}
			}
			else {
				ptr->place = 0;
			}
			ptr = ptr->next;
		}
	}
}

void mark_extern(sign_table_ptr * head, char * sign, int IC) {
	sign_table_ptr *ptr = head;
	if (ptr->sign == NULL) {
		return;
	}
	else {
		while (ptr != NULL) {
			if (strcmp(ptr->sign, sign) == 0) {
				ptr->references = (int *)realloc(ptr->references, ++ptr->num_ref * sizeof(int));
				*(ptr->references + (ptr->num_ref - 1)) = IC + START;
				return;
			}
			ptr = ptr->next;
		}
	}
}
