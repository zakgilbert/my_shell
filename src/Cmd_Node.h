#ifndef CMD_NODE_H
#define CMD_NODE_H

typedef struct _Cmd_Node 
{
    void (*destroy)(struct _Cmd_Node * this);  /* Free Allocated Memory */
    char * key;
    char *cmd;
    int index;
    
}Cmd_Node;
Cmd_Node * CREATE_CMD_NODE(char * key, char * path, int index);

#endif /* CMD_NODE_H */
