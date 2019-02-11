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

typedef struct data_image {
	int data;
	struct data_image * next;
}data_image;

typedef struct code_image {
	int code;
	struct code_image * next;
}code_image;

typedef struct opcode {
	char * code;
	int opcode;
}opcode;

typedef struct address_mode {
	char * first_arg;
	char * second_arg;
	int first_mode;
	int second_mode;
}address_mode;