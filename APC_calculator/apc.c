#include "apc.h"
#include<ctype.h>
#include<string.h>
// Function definitions
int cla_validation(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("Invalid input\n");
        return FAILURE;
    }
    if(strcmp(argv[2],"+") != 0 &&
       strcmp(argv[2],"-") != 0 &&
       strcmp(argv[2],"*") != 0 &&
       strcmp(argv[2],"x") != 0 &&
       strcmp(argv[2],"X") != 0 &&
       strcmp(argv[2],"/") != 0 &&
       strcmp(argv[2],"%") != 0 )
    {
        printf("Invalid operator\n");
        return FAILURE;
    }

    int i = 0;

    if(argv[1][0] == '-' || argv[1][0] == '+')
    {
       i = 1;
    }

    if(argv[1][i] == '\0')// if the first operand is just  "-" like no digits case
    {
        printf("Invalid first operand\n");
        return FAILURE;
    }

    while(argv[1][i] != '\0')
    {  
        if(!isdigit((unsigned char)argv[1][i]))
        {
            printf("Invalid first operand\n");
            return FAILURE;
        }
        i++;
    }
    i=0;
    if(argv[3][0] == '-' || argv[3][0] == '+')
    {
       i = 1;
    }

    if(argv[3][i] == '\0')// if the second operand is just  "-" like no digits case
    {
        printf("Invalid Second Operand\n");
        return FAILURE;
    }
    
    while(argv[3][i] != '\0')
    {  
        if(!isdigit((unsigned char)argv[3][i]))
        {
            printf("Invalid Second Operand\n");
            return FAILURE;
        }
        i++;
    }
    printf("Input Validation Successfull!!!\n");
    return SUCCESS;
    
}
int insert_last(node **head,node **tail,int digit)
{
    if(head == NULL || tail == NULL)
    {
        return FAILURE;
    }

    node *new1 = malloc(sizeof(node));
    if(new1 == NULL)
    {
        return FAILURE;
    }
    new1->data = digit;
    new1->prev = *tail;
    new1->next = NULL;

    if(*head == NULL)
    {
        *head = new1;
        *tail = new1;
        return SUCCESS;
    }

    (*tail)->next = new1;
    *tail = new1;
    return SUCCESS; 
}

void remove_pre_zeros(node **head)
{
    // loop only if head exist and heads data is 0 and list contain more nodes
    while(*head != NULL && (*head)->data == 0 && (*head)->next != NULL)
    {
        node *temp = *head;
        *head = (*head)->next;
        (*head)->prev = NULL;
        free(temp);
    }
}

void create_list(char *opr, node **head, node **tail)
{
    if(opr == NULL || head == NULL || tail == NULL)
    {
        return ;
    }

    // Ensure pointers start fresh
    *head = NULL;
    *tail = NULL;

    int i = 0;
    if(opr[0] == '-' || opr[0] == '+')
    {
        i++;
    }

    while(opr[i] != '\0')
    {
        
        int digit = opr[i] - '0';
        insert_last(head,tail,digit);
        i++;
    }
}

int compare(node *head1, node *head2)
{
    node *temp1 = head1;
    node *temp2 = head2;
    int len1=0,len2 = 0;
    while(temp1 != NULL)
    {
        len1++;
        temp1 = temp1->next;
    }

    while(temp2 != NULL)
    {
        len2++;
        temp2 = temp2->next;
    }

    if(len1 > len2)// longer length means larger magnitude
    {
        return 1;// ist operand is larger which is fine
    }
    else if(len1 < len2)
    {
        return -1;// 2nd operand is larger which we return -1
    }

    //if length are equal then compare each digit
    temp1 = head1;
    temp2 = head2;
    while(temp1 != NULL && temp2 != NULL)
    {
        if(temp1->data > temp2->data)
        {
            return 1;
        }
        else if(temp1->data < temp2->data)
        {
            return -1;
        }

        temp1= temp1->next;
        temp2 = temp2->next;
    }

    return 0;// if we reach here then numbers are equal.
}

int insert_first(node **head, node **tail, int data)
{
    node *new = malloc(sizeof(node));
    if(new == NULL)
    {
        return FAILURE;
    }
    new->data = data;
    new->prev = NULL;
    new->next = *head;
    if(*head == NULL)
    {
        *head = new;
        *tail = new;
        return SUCCESS;
    }
    
    (*head)->prev = new;
    *head = new;
    return SUCCESS;
    
}

// helper function to free the list
void free_list(node **head , node **tail)
{
    node *temp = *head;
    while(temp != NULL)
    {
        node *temp1 = temp;
        temp = temp->next;
        free(temp1);
    }
    *head = NULL;
    *tail = NULL;
}

// helper fucntion to copy a list
void copy_list(node *src_head, node **dest_head, node **dest_tail)
{
    *dest_head = NULL;
    *dest_tail = NULL;
    node *temp = src_head;
    while(temp != NULL)
    {
        insert_last(dest_head,dest_tail,temp->data);
        temp = temp->next;
    }
}

void print_list(node *head)
{
    node *temp = head;
    while (temp != NULL)
    {
        printf("%d", temp->data);
        temp = temp->next;
    }
    printf("\n");
}