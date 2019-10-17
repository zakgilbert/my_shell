
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include "Cmd_Node.h"
#include "Cmd.h"
#include "Hash_t.h"

static int check_size(int size, int count)
{
    if (count >= size)
        return 1;
    return 0;
}

/**
 * Creates hash code from a char *, prime number, and the size of the hash table.
 */
static int hash(char* str, int prime, int size)
{
    long hash = 0;
    int i;
    const int len = strlen(str);
    for (i = 0; i < len; i++) {
        hash += (long)pow(prime, len - (i + 1)) * str[i];
        hash = hash % size;
    }
    return (int)hash;
}

/**
 * Creates hash code by passing two different prime numbers into <int hash(char * , int, int)
 */
static int hash_code(char* str, const int num, const int attempt)
{
    int hash_a = hash(str, PRIME_1, num);
    int hash_b = hash(str, PRIME_2, num);
    return (hash_a + (attempt * (hash_b + 1))) % num;
}

/**
 * Deallocates all memory that of which was allocated at the hashtables creation.
 */
static void _destroy(Hash_t* this)
{
    Cmd_Node* temp  = NULL;
    Cmd_Node* dummy = NULL;
    if (NULL != this) {
        for (size_t i = 0; i < this->size; i++) {
            temp = this->table[i];
            if (NULL != temp) {
                this->table[i] = dummy;
                temp->destroy(temp);
            }
        }
        free(this->table);
        free(this);
    }
}

/**
 * Performs a hash search for the given key
 */
static Cmd_Node* _search(Hash_t* this, char* key)
{
    int index      = hash_code(key, this->size, 0);
    Cmd_Node* item = this->table[index];
    int i          = 1;
    while (NULL != item) {
        if (strcmp(item->key, key) == 0)
            return item;
        index = hash_code(key, this->size, i);
        i++;
    }
    return NULL;
}

/**
 * Doubles the size of the hashtable
static void _grow(Hash_t* this)
{
    Cmd_Node** temp;
    Cmd_Node *dummy, *temp_n;
    int old_size;

    temp        = this->table;
    dummy       = NULL;
    temp_n      = NULL;
    old_size    = this->size;
    this->size  = old_size * 2;
    this->table = realloc(this->table, (sizeof(Cmd_Node*)) * (this->size));
    this->count = 0;

    for (int i = 0; i < old_size; i++) {
        temp_n = temp[i];
        if (NULL != temp_n) {
            this->insert(this, CREATE_LOGIC_NODE(temp_n->key, temp_n->obj, temp_n->funct, NULL));
            this->table[i] = dummy;
            temp_n->destroy(temp_n);
        }
    }
    free(temp);
}
 */
static int get_num_cmds(char* input)
{
    int argc, i;

    i    = 0;
    argc = 0;

    for (; input[i]; i++) {
        if (input[i + 1] == '\n')
            argc++;
    }
    return argc;
}

static int get_len_segment(char* str)
{
    int len;

    len = strlen(str) - 1;
    while (str[len] == '\n')
        str[len--] = '\0';

    return strlen(str);
}
static void parse(Hash_t* this, char* str, int len, int argc, char* path)
{
    int k, j;
    char buff[50];

    k = len - 1;
    j = argc;

    for (; k >= 0; k--) {
        if (str[k] == '\n') {
            printf("adding: %s\n", &str[k + 1]);
            this->insert(this, CREATE_CMD_NODE(&str[k + 1], strcat(strcpy(buff, path), &str[k + 1]), 1));
            str[k] = '\0';
            j--;
        }
    }
}
static void _load_cmds(Hash_t* this, char* path)
{
    FILE *in, out;
    char buff[50000];
    char* add_ls;
    size_t num;
    int k, j, argc, len;

    add_ls = malloc(50);

    add_ls = strcat(strcpy(add_ls, "ls "), path);

    if ((in = popen(add_ls, "r")) == NULL) {
        printf("Error popen()\n");
        exit(1);
    }
    while ((num = fread(buff, 1, sizeof(buff) - 1, in)) > 0) {
    }
    argc = get_num_cmds(buff);
    parse(this, buff, strlen(buff), argc, path);

    pclose(in);
}

/**
 * Inserts a new node into hashtable.
 */
static void _insert(Hash_t* this, Cmd_Node* item)
{
    int index;
    Cmd_Node* cur_item;
    int i;

    index    = hash_code(item->key, this->size, 0);
    cur_item = this->table[index];
    i        = 1;

    if (check_size(this->size, this->count) == 1)
        this->grow(this);
    while (NULL != cur_item) {
        index    = hash_code(item->key, this->size, i);
        cur_item = this->table[index];
        i++;
    }
    this->table[index]        = item;
    this->table[index]->index = index;
    printf("%s", this->table[index]->key);
    printf("%*s at index %d\n", 10, "Added", index);
    this->count++;
}

/**
 * Deletes node of key
 */
static void _delete(Hash_t* this, char* key)
{
    Cmd_Node* item  = this->search(this, key);
    Cmd_Node* dummy = NULL;
    if (item) {
        this->table[item->index] = dummy;
        item->destroy(item);
        this->count--;
    } else
        printf("The node with a key value %s was not found\n", key);
}

Hash_t* CREATE_HASH_T(int size)
{
    Hash_t* this = malloc(sizeof(*this));

    this->destroy = _destroy;
    this->search  = _search;
    /**
        this->grow        = _grow;
*/
    this->insert    = _insert;
    this->delete    = _delete;
    this->load_cmds = _load_cmds;

    this->size  = size;
    this->table = malloc(sizeof(Cmd_Node*) * this->size);
    this->count = 0;

    for (int i = 0; i < this->size; i++)
        this->table[i] = NULL;

    return this;
}
