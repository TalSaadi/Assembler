typedef struct error {
	char *desc;
	struct error * next;
}error;