#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "LinesProcessor.h"
#include "Reader.h"
#include "DataTypes.h"

int process(char *line) {
	int found_flag = 0;
	if (is_empty(line)) {
		return 0;
	}
	else {
		#pragma warning(suppress : 4996)
		line = strtok(line, " ");
		if (line == ";") {
			return 0;
		}
		else if ((line + (strlen(line) - 1)) == ":") {
			found_flag = 1;
		}
	}
}

int is_empty(char *line) {
	while (*line != '\0') {
		if (!isspace((unsigned char)*line))
			return 0;
		line++;
	}
	return 1;
}