#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

char** argparse(char* str, char** argv, int* argc) {
    size_t arg_size = 16;
    argv = calloc(arg_size, sizeof(char*));
    size_t i = 0;
    size_t argv_i = 0;
    size_t argstr_i = 0;
    size_t argstrlen = 0;
    uint arg_empty = 1;
    uint in_quotes = 0;
    uint escaped = 0;
    //in_quotes = 1 -> "" quote
    //in_quotes = 2 -> '' quote
    while(str[i] != 0) {
        if(arg_empty && argstrlen == 0) {
            argstrlen = 64;
            argv[argv_i] = malloc(argstrlen);
        }
        if(argstr_i >= argstrlen) {
            argstrlen *= 2;
            argv[argv_i] = realloc(argv[argv_i], argstrlen);
        }
        if(argv_i >= arg_size) {
            arg_size *= 2;
            argv = reallocarray(argv, arg_size, sizeof(char*));
        }
        if(in_quotes) {
            if((str[i] == 34 || str[i] == 39) && !escaped) {
                //check if it's a valid end to the quote
                if((in_quotes == 1 && str[i] == 34) || (in_quotes == 2 && str[i] == 39)) {
                    in_quotes = 0;
                }
            } else {
                //treeat it like a normal character 
                argv[argv_i][argstr_i] = str[i];
                arg_empty = 0;
                argstr_i++;
            }
        }
        else if((str[i] == 34 || str[i] == 39) && !escaped) {
            //34 is the ASCII value for "
            //39 is the ASCII value for '
            //-> enter quote mode
            in_quotes = (str[i] == 34) ? 1 : 2;
        }
        else if(str[i] == ' ' && !arg_empty && !escaped) {
            //End of arg has been reached -> finish arg and move to next
            argv[argv_i][argstr_i] = 0;
            argv_i++; 
            argstr_i = 0;
            argstrlen = 0;
            arg_empty = 1;
        } 
        else if(str[i] == ' ') {
            i++;
            continue;
        }
        else {
            //Handle escaping sequences
            if(str[i] == '\\') {
                escaped = 1;
            } else {
                //Normal Character 
                argv[argv_i][argstr_i] = str[i];
                arg_empty = 0;
                argstr_i++; 
                escaped = 0;
            }
        }
        //Space -> move to next arg unless arg is still empty
        i++;
    }
    //terminate the last arg string with a 0
    argv[argv_i][argstr_i] = 0;

    //set argc to length
    *argc = argv_i+1;
    return argv;
}

int main(int argc, char* argv[]) {
    int c = 0;
    //free(argparse("argparse 123 456", NULL, &c));
    char ** arrp = argparse("argparse   a \"1\" '2' a=2 -a", NULL, &c);
    for(size_t i = 0; i < c; i++) {
        printf("%d: %s\n", (int)i, arrp[i]);
    }
}