#ifndef HASH_T_H
#define HASH_T_H

#define PRIME_1 163
#define PRIME_2 151

typedef struct _Cmd_Node Cmd_Node;
typedef struct _Hash_t {
    void (*destroy)(struct _Hash_t* this);                /* Free allocated memory */
    Cmd_Node* (*search)(struct _Hash_t* this, char* key); /* Perform a hash search for the given key */
    void (*print_table)(struct _Hash_t* this);            /* Print Hashtable */
    void (*grow)(struct _Hash_t* this);                   /* Double the size of hashtable */
    void (*insert)(struct _Hash_t* this, Cmd_Node* item); /* Insert a new node into hashtable. */
    void (*delete)(struct _Hash_t* this, char* key);      /* Delete node of key */
    void (*load_cmds)(struct _Hash_t* this, char* path);

    Cmd_Node** table; /* Array of Logic Node Pointers */
    int size;         /* size of table */
    int count;        /* number of nodes hash into table */

} Hash_t;

Hash_t* CREATE_HASH_T(int size);

#endif /* HASH_T_H */
