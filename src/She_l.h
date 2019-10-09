#ifndef SHE_L_H
#define SHE_L_H
#define PRINT 0
void print_title_screen();
typedef struct _She_l {
    void (*destroy)(struct _She_l* this); /* Free Allocated Memory */
    Cmd* (*get_user_input)(struct _She_l* this, Cmd* command);
    void (*run)(struct _She_l* this);
    char** argv;
    int argc;
    Cmd* current_comand;
} She_l;
She_l* CREATE_SHE_L();

#endif /* SHE_L_H */
