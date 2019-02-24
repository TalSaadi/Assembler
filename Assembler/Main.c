#include <stdio.h>
#include "Reader.h"

int main(int argc, char *argv[]) {
	int i;
	if (argc == 1) {
		printf("No arguments found\n");
		return -1;
	}
	else {
		for (i = 1; i < argc; i++) {
			read(*(argv + i));
		}
	}
	return 1;
}
