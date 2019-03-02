#pragma once
#include "DataTypes.h"

void encode_number(machine_code_type * machine_code, char * arg, int line); /* Number to machine code */
void encode_reg(machine_code_type * machine_code, char * reg, int line); /* One register to machine code */
void encode_2_regs(machine_code_type * machine_code, char * reg1, char * reg2); /* Two registers to one line machine code*/
void encode_sign(machine_code_type *machine_code, int line); /* Sign to machine code */
char *b64_encode(char *line); /* Binary to Base 64 */
int binaryToDecimal(int n); /* Binary to Decimal */
