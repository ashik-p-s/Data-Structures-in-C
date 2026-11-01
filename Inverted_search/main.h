#ifndef MAIN_H
#define MAIN_H

#include<stdio.h>
#include <string.h>
#include <stdlib.h>

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

typedef enum
{
    SUCCESS = 1,
    FAILURE = 0
}Status;

// Forward declarations so the compiler recognizes these types early
struct sub_node;
struct main_node;

typedef struct sub_node sub_node;
typedef struct main_node main_node;

typedef struct file_node
{
    char file_name[50];
    struct file_node *link;
}Flist;

typedef struct node
{
    int index;
    main_node *link;// point
}hash_t;

struct main_node
{
    char word[50];
    int file_count;
    sub_node *sub_link;
    main_node *link;
};

struct sub_node
{
    char file_name[50];
    int word_count;
    sub_node *link;
};

extern hash_t HT_array[27];

Status cla_validation(int argc,char *argv[],Flist **head);
Status create_database(Flist *head , hash_t array[]);
Status display(hash_t HT_array[]);
Status search(hash_t HT_array[]);
Status save(hash_t HT_array[]);
Status update(hash_t HT_array[]);

#endif