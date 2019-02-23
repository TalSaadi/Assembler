typedef struct error {
	char *desc;
	struct error * next;
}error;

typedef struct sign_table_ptr {
	char * sign;
	int place;
	int isext;
	int iscode;
	struct sign_table_ptr * next;
}sign_table_ptr;

typedef struct opcode {
	char * code;
	char * opcode;
}opcode;

typedef struct address_mode {
	char * first_arg;
	char * second_arg;
	char * first_mode;
	char * second_mode;
}address_mode;

typedef struct machine_code_type {
	char** machine_code;
	int isdata;
	int num_lines;
}machine_code_type;