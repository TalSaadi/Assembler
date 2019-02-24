#pragma once
#include "DataTypes.h"

void encode_number(machine_code_type * machine_code, char * arg, int line);
void encode_reg(machine_code_type * machine_code, char * reg, int line);
void encode_2_regs(machine_code_type * machine_code, char * reg1, char * reg2);
void encode_sign(machine_code_type *machine_code, int line);