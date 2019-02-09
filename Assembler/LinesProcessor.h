#include <stdio.h>
#include <string.h>


int process(char * line) {
	line = strtok(line, " ");
	while (line != NULL) {
		printf("%s\n", p);
		p = strtok(NULL, " ");
	}
}
