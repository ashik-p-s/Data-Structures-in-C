#include "apc.h"

void subtraction(node *tail1, node *tail2, node **headR, node **tailR)
{
    *headR = NULL;
    *tailR = NULL;

    int borrow  = 0;
    while(tail1 != NULL || tail2 != NULL || borrow > 0 )
    {
        int diff = -borrow;//bcz in subtraction order matters borrow must be subtracted

        if(tail1 != NULL)
        {
            diff += tail1->data;// add top operand digit
            tail1 = tail1->prev;
        }
        if(tail2 != NULL)
        {
            diff -= tail2->data;// subtract 2nd operand digit.
            tail2 = tail2->prev;
        }
        if(diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }

        insert_first(headR, tailR, diff);
        
    }
    //printf("Subtraction Successfull\n");
    
}
