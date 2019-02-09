#include <stdio.h>
#include "Reader.h"

int main(int argc, char *argv[]) {
	int i = 1;
	if (argc == 1) {
		printf("No arguments found\n");
		return -1;
	}
	else {
		for (; i <= argc; i++) {
			read(*(argv + i));
		}
	}
	
}