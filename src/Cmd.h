#ifndef CMD_H
#define CMD_H
#define PRINT 0
typedef struct _Hash_t Hash_t;
char* read_command(const char* str);
typedef struct _Cmd {
    void (*destroy)(struct _Cmd* this); /* Free Allocated Memory */
    char** (*prompt)(struct _Cmd* this);
    int argc;
    char** argv;
    char* user_tag;
    char delim;
    Hash_t* tab;
} Cmd;
Cmd* CREATE_CMD(char* user_tag, Hash_t* tab);

#endif /* CMD_H */
