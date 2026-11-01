#include "apc.h"


void addition(node *tail1, node *tail2, node **headR, node **tailR)
{
    //intializing result list pointers to a fresh start
    *headR = NULL;
    *tailR = NULL;

    int carry = 0;

    // loop must run until there is digits at either tail1 or tail2 or in carry.
    while(tail1 != NULL || tail2 != NULL || carry > 0)
    {
        // start the sum with carry from previous step.
        int sum = carry;

        //Add digit from list1 if available and move tail1 to left like in normal +.
        if(tail1 != NULL)
        {
            sum += tail1->data;
            tail1 = tail1->prev;
        }

         //Add digit from list2 if available and move tail2 to left like in normal +.
        if(tail2 != NULL)
        {
            sum += tail2->data;
            tail2 = tail2->prev;
        }

        // calculate the new carry and single digit to store in rsultant list.
        carry = sum/10;// stored in carry
        int digit = sum % 10;// stores in result node(15/10 = 1(carry) and 15%10 = 5(result))

        insert_first(headR,tailR,digit);
            
    }
}