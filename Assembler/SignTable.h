#pragma once

typedef struct sign_table_ptr {
	char * sign;
	int place;
	int isext;
	int iscode;
	struct sign_table_ptr * next;
}sign_table_ptr;


sign_table_ptr* create(char * flag, int DC, int isext, int iscode, sign_table_ptr* next);
void append(sign_table_ptr *head, char * flag, int DC, int isext, int iscode);
int search_sign(sign_table_ptr *head, char *sign);