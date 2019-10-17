
/**
*  Cmd_Node.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cmd_Node.h"

static void _destroy(Cmd_Node* this)
{
    if (NULL != this) {
        free(this);
    }
}

Cmd_Node* CREATE_CMD_NODE(char* key, char* path, int index)
{
    Cmd_Node* this = malloc(sizeof(*this));
    this->destroy  = _destroy;

    this->key = key;
    this->cmd = path;
    printf("%s\n", this->cmd);
    this->index = index;

    return this;
}
