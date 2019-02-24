#pragma once

typedef struct sign_table_ptr {
	char * sign;
	int place;
	int isext;
	int isent;
	struct sign_table_ptr * next;
}sign_table_ptr;


sign_table_ptr* create(char * flag, int DC, int isext, int isent, sign_table_ptr* next);
void append(sign_table_ptr *head, char * flag, int DC, int isext, int isent);
int search_sign(sign_table_ptr *head, char *sign);
int mark_entry(sign_table_ptr *head, char *sign);
int sign_plce(sign_table_ptr *head, char *sign);