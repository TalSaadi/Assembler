#pragma once
#include "DataTypes.h"

char * search_code(char * code);
int check_address_code(char * argument);
int address_data(machine_code_type * machine_code, address_mode mode);
address_mode analyze_arguments(char * code, char * arguments);