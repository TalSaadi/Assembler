#pragma once

typedef struct sign_table_ptr {
	char * sign;
	int place;
	int isext;
	int isent;
	int isdata;
	int * references;
	int num_ref;
	struct sign_table_ptr * next;
}sign_table_ptr;


sign_table_ptr* create(char * flag, int place, int isext, int isent, int isdata, sign_table_ptr* next);
void append(sign_table_ptr *head, char * flag, int place, int isext, int isent, int isdata);
int search_sign(sign_table_ptr *head, char *sign);
int mark_entry(sign_table_ptr *head, char *sign);
int sign_place(sign_table_ptr *head, char *sign);
void shift_places(sign_table_ptr * head, int IC);
void mark_extern(sign_table_ptr * head, char * sign, int IC);
