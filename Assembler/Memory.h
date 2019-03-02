#pragma once
#include "SignTable.h"

void free_array(char **a, int m); /* Free dynamically alocated array */
void free_sign_table(sign_table_ptr * head); /* Free alocated sign table */