#include <stdio.h>
#include "Reader.h"

int main(int argc, char *argv[]) {
	int i;
	if (argc == 1) {
		printf("Error in file: %s, line number %d, No arguments found\n", globalFileName, globalLineNum);
		return -1;
	}
	else {
		for (i = 1; i < argc; i++) {
			read(*(argv + i));
		}
	}
	return 1;
}
