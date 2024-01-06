#pragma once
#include <stdlib.h>
#include "gdhmap.h"
#define MAX_KEY_LENGTH 32
#define MAX_VAL_LENGTH 256

typedef struct conf_opt_t {
    char key[MAX_KEY_LENGTH];
    char val[MAX_VAL_LENGTH];
} conf_opt;

conf_opt* read_conf(char* filepath, size_t* n);
gdhmap* read_conf_map(char* filepath);