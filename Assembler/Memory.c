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

void free_array(char **a, int m) {
	int i;
	for (i = 0; i < m; ++i) {
		free(a[i]);
	}
	free(a);
}

void free_sign_table(sign_table_ptr * head) {
	sign_table_ptr * curr;
	while ((curr = head) != NULL) {
		head = head->next;
		free(curr->sign);
	}
}