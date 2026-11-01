#include "apc.h"
#include<stdlib.h>

void multiplication(node *tail1, node *tail2, node **headR, node **tailR)
{
    *headR = NULL;
    *tailR = NULL;

    // if either number is null means  result is empty/zero
    if(tail1 == NULL || tail2 == NULL)
    {
        return;
    }
    
    node *t2 = tail2;
    int shift = 0;// to track places values like for 0 for ones ,1 for tens etc

    node *head_t1 = NULL;
    node *tail_t1 = NULL;
    node *head_t2 = NULL;
    node *tail_t2 = NULL;

    int count = 0;// to count the multiplication round

    while(t2 != NULL)// traversing operand 2 from right to left 
    {
        head_t2 = NULL;
        tail_t2 = NULL;

        int carry = 0;
        node *t1 = tail1;
        // inner loop for multiplying entire operand1 with one digit of operand2
        while(t1 != NULL || carry > 0)
        {
            int product = carry;

            if(t1 != NULL)
            {
                product += (t1->data * t2->data);
                t1 = t1->prev;
            }

            carry = product/10;
            int digit = product % 10;

            // store the first intermediate in temp2
            insert_first(&head_t2,&tail_t2,digit);

        }

        // place value padding
        for(int i = 0;i < shift;i++)
        {
            insert_last(&head_t2,&tail_t2,0);
        }

        //first pass means during first multiplication round the the addition freeing and copying 
        //need not be done just store resutl in temp1 
        if(count == 0)
        {
            // first multiplication result goes straight to temp1
            head_t1 = head_t2;
            tail_t1 = tail_t2;
            head_t2 = NULL;
            tail_t2 = NULL;
        }
        else
        {
            // add temp1 and temp2 and store it in resultlist(*headR, *tailR)
            addition(tail_t1,tail_t2,headR,tailR);

            //delete both temp lists(temp1 and temp2)
            free_list(&head_t1,&tail_t1);
            free_list(&head_t2,&tail_t2);

            // copy resultant list to temp1
            copy_list(*headR,&head_t1,&tail_t1);

            // clear out resultant list for next round
            free_list(headR, tailR);
        }
        count++;
        shift++;
        t2 = t2->prev;
    }

    // after the loop finishes, temp1 hold the final accumulated result 
    *headR = head_t1;
    *tailR = tail_t1; 
}
