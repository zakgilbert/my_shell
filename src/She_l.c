#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Cmd.h"
#include "She_l.h"

static void _destroy(She_l* this)
{
    if (NULL != this) {
        if (PRINT)
            printf("%p\n", this);
        free(this);
    }
}

Cmd* _get_user_input(She_l* this, Cmd* command)
{
    this->current_comand = command;
    this->current_comand->prompt(this->current_comand);
    return command;
}

void _run(She_l* this)
{
    int status;
    FILE *in, *out;
    char buffer[100];

    if ((strcmp(this->current_comand->argv[0], "cd")) == 0) {
        chdir(this->current_comand->argv[1]);
        return;
    }

    /* Pipe it */
    if (this->current_comand->delim == '|') {
        out = popen(this->current_comand->argv[1], "w");
        in  = popen(this->current_comand->argv[0], "r");

        while (fgets(buffer, 100, in) != NULL)
            fprintf(out, "%s", buffer);
        pclose(out);
        pclose(in);
        return;
    }
    if ((waitpid(fork(), &status, 0)) == -1) {
        execvp(this->current_comand->argv[0], this->current_comand->argv);
        exit(0);
    }
}

She_l* CREATE_SHE_L()
{
    She_l* this          = malloc(sizeof(*this));
    this->destroy        = _destroy;
    this->get_user_input = _get_user_input;
    this->run            = _run;

    return this;
}
