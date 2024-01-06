#include <ctype.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include <stdbool.h>
#include "strlib.h"

#define LINE_LENGTH (MAX_KEY_LENGTH + MAX_VAL_LENGTH + 8) 
#define INIT_LINES 512

/// @brief Read a .conf Configuration file at filePath. conf_arr returns the array pointer. n is the size of the array
/// Comments are only treated as comments if # is first non-space character in line
/// @param filepath the Path to the configuration file
/// @param n Pointer to the counter of the array (should be 0 at calling)
/// @return Returns the array pointer
conf_opt* read_conf(char* filepath, size_t* n) {
    FILE *fp;

    //Initially set length to 512 and realloc if there are more lines than that
    conf_opt* optarr = calloc(INIT_LINES, sizeof(conf_opt));

    if((fp = fopen(filepath, "r")) == NULL) {
        char buffer[320]; //256 is max path length add 64 for easy memory alignment
        sprintf(buffer,"File at %s could not be found", filepath);
        perror(buffer);
        return NULL;
    }

    char* linebuf = calloc(1, LINE_LENGTH);
    size_t i = 0;
    *n = INIT_LINES;
    size_t len = 0;
    int read = 0;
    while((read = getline(&linebuf, &len, fp)) != -1) {
        char* buf = calloc(1, LINE_LENGTH);
        //Remove leading and trailing whitespaces
        remove_whitespaces(linebuf, buf);
        free(linebuf);
        linebuf = buf;

        //Ignore Comments
        if(linebuf[0] == '#') continue;

        //Allocate memory for conf_opt struct and read values
        conf_opt* conf_p = calloc(1,sizeof(conf_opt));

        if(conf_p == NULL) {
            perror("Not able to allocate enough memory for lines");
        } 

        //split string into key and value
        char keybuff[MAX_KEY_LENGTH], valbuff[MAX_VAL_LENGTH];
        split_at_char(linebuf, keybuff, valbuff, '=');
        remove_whitespaces(valbuff, conf_p->val);
        remove_whitespaces(keybuff, conf_p->key);
        remove_newline(conf_p->val);

        //put key in lowercase
        int key_len = strlen(conf_p->key);
        for (size_t i = 0; i < key_len; i++) {
            conf_p->key[i] = tolower(conf_p->key[i]);
        }

        //Check if array is still big enough 
        if(i >= *n) {
            optarr = reallocarray(optarr, 2*(*n), sizeof(conf_opt));
            *n *= 2;

            if(optarr == NULL) {
                perror("No able to allocate enough memory for lines");
                return NULL;
            }
        }

        optarr[i] = *conf_p;
        i++;
    }
    free(linebuf);
    //free unused array space and return pointer
    optarr = reallocarray(optarr, i, sizeof(conf_opt));
    *n = i;
    return optarr;
}

gdhmap* read_conf_map(char* filepath) {
    //To avoid constant resizing we execute read_conf one time and then just create a map with correct size
    size_t n = 0;
    conf_opt* optarr = read_conf(filepath, &n);

    //create map
    gdhmap* map = gdhmap_create(2*n);
    //2*n = size to avoid long probing sequences
    for(size_t i = 0; i < n; i++) {
        gdhmap_put(map, optarr[i].key, optarr[i].val, MAX_VAL_LENGTH);
    }

    free(optarr);
    return map;
}