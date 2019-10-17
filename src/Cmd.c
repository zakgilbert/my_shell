
/**
*  Cmd.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Cmd.h"

#define SIZE 100

char* read_command(const char* str)
{
    FILE* in;
    char buff[100];
    char* result;

    in = popen(str, "r");
    fgets(buff, 100, in);

    buff[strlen(buff) - 1] = '\0';
    result                 = malloc(strlen(buff));
    return strcpy(result, buff);
}
char check_for_pipe(char* str)
{
    int i = 0;
    for (; i < strlen(str); i++) {
        if (str[i] == '|')
            return '|';
    }
    return ' ';
}
/* Return the number of spaces in input */
static int get_argc(char* input, char delim)
{
    int argc, i;

    i    = 0;
    argc = 0;

    for (; input[i]; i++) {
        if (input[i + 1] != '\n' && input[i] == delim)
            argc++;
    }
    return argc;
}

/* Remove Trailing whitespace and newline */
static int get_len(char* str, char delim)
{
    int len;

    len = strlen(str) - 1;
    while (str[len] == '\n' || str[len] == delim)
        str[len--] = '\0';

    return strlen(str);
}

/* Return parsed str as argv */
static char** parse(char* str, int len, int argc, char delim)
{
    char** argv;
    int k, j;

    k = len - 1;
    j = argc;

    argv = malloc(sizeof(char*) * (argc + 2));

    for (; k >= 0; k--) {
        if (str[k] == delim) {
            argv[j] = malloc(sizeof(char) * strlen(&str[k + 1]));
            argv[j] = strcpy(argv[j], &str[k + 1]);
            str[k]  = '\0';
            j--;
        }
    }
    if (delim == '|') {
        argv[j] = malloc(sizeof(char) * strlen(str));
        argv[j] = strcpy(argv[j], str);

    } else {
        argv[j] = malloc(sizeof(char) * (strlen(str)));
        argv[j] = strcpy(argv[j], str);
        printf("argv[%d]: %s\n", j, argv[j]);
    }
    if (NULL != argv[argc + 1])
        argv[argc + 1] = NULL;

    return argv;
}

/* Free allocated memory of Cmd object */
static void _destroy(Cmd* this)
{
    int i;
    if (NULL != this) {
        for (i = 0; i < this->argc + 1; i++)
            free(this->argv[i]);
        if (PRINT)
            printf("%p\n", this);
        free(this);
    }
}

/* Prompt the user */
static char** _prompt(Cmd* this)
{
    char line[SIZE];
    int argc;
    int line_l;
    int i;

    i = 0;
    printf("Shell@%s $ ", this->user_tag);
    fgets(line, SIZE, stdin);
    this->delim = check_for_pipe(line);
    argc        = get_argc(line, this->delim);
    line_l      = get_len(line, this->delim);
    this->argv  = parse(line, line_l, argc, this->delim);
    this->argc  = argc;
    return this->argv;
}

Cmd* CREATE_CMD(char* user_tag)
{
    Cmd* this      = malloc(sizeof(*this));
    this->destroy  = _destroy;
    this->prompt   = _prompt;
    this->user_tag = user_tag;
    this->delim    = ' ';

    if (PRINT)
        printf("%p\n", this);

    return this;
}
