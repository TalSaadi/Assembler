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


sign_table_ptr* create(char * flag, int place, int isext, int isent, int isdata, sign_table_ptr* next); /* Create new sign node */
void append(sign_table_ptr *head, char * flag, int place, int isext, int isent, int isdata); /* Append sign node to last one */
int search_sign(sign_table_ptr *head, char *sign); /* Search for sign in table */
int mark_entry(sign_table_ptr *head, char *sign); /* Mark sign as entry */
int sign_place(sign_table_ptr *head, char *sign); /* Find sign place */
void shift_places(sign_table_ptr * head, int IC); /* Shift all sign places by 100 and add IC to data */
void mark_extern(sign_table_ptr * head, char * sign, int IC); /* Mark sign as extern */
