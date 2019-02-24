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

void append(sign_table_ptr* head, char * flag, int place, int isext, int iscode)
{
	/* go to the last node */
	sign_table_ptr *cursor = head;
	sign_table_ptr* new_node;
	size_t length;
	if (head->sign == NULL) {
		length = strlen(flag);
		head->sign = (char*)malloc(length + 1);
		if (head->sign == NULL) {
			printf("Unable to allocate memory");
			exit(0);
		}
#pragma warning(suppress : 4996)
		strcpy(head->sign, flag);
		head->place = place;
		head->isext = isext;
		head->iscode = iscode;
		head->next = NULL;
		return;
	}
	while (cursor->next != NULL)
		cursor = cursor->next;

	/* create a new node */
	new_node = create(flag, place, isext, iscode, NULL);
	cursor->next = new_node;
}

sign_table_ptr* create(char * flag, int DC, int isext, int iscode, sign_table_ptr* next)
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
		printf("Unable to allocate memory");
		exit(0);
	}
#pragma warning(suppress : 4996)
	strcpy(new_node->sign, flag);
	new_node->place = DC;
	new_node->isext = isext;
	new_node->iscode = iscode;
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