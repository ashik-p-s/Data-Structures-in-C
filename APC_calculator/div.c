#include "apc.h"

void division(node *head1, node *head2, node **headR, node **tailR)
{
    *headR = NULL;
    *tailR = NULL;

    // if either num is NULL means empty result
    if(head1 == NULL || head2 == NULL)
    {
        return ;
    }

    // FIND DIVISOR TAIL ONCE HERE:
    node *tail2 = head2;
    while(tail2->next != NULL)
    {
        tail2 = tail2->next;
    }

    // temporary list to hold out remainder as we pull digits from left to right
    node *rem_head = NULL;
    node *rem_tail = NULL;

    node *temp = head1;

    while(temp != NULL)
    {
        //Bring down the next digit from temp->data
        // and append it to running remainder list.
        insert_last(&rem_head,&rem_tail,temp->data);

        // remove leading zeros from running remainder list
        remove_pre_zeros(&rem_head);

        if (rem_head == NULL)
        {
            rem_tail = NULL;
        }
        else
        {
            rem_tail = rem_head;
            while(rem_tail->next != NULL)
            {
                rem_tail = rem_tail->next;
            }
        }

        int cmp = compare(rem_head, head2);
        if(cmp < 0)
        {
            //Remainder is smaller than divisor
            // if we already have digits in our quotient, a '0' must be added
            if(*headR != NULL)
            {
                insert_last(headR,tailR,0);
                //dividing 205 / 2, when looking at 0, the divisor doesn't fit, 
                //so we must record a 0 in the quotient).
            }
        }
        else
        {
            //remainder is >=divisor! Test multipliers from 9 down to 1
            int guess = 9;
            int flag = 0;// to check if guess worked or not?
    
            while(guess > 0)
            {
                // create a single node list
                node *guess_head = NULL;
                node *guess_tail = NULL;
                insert_last(&guess_head,&guess_tail,guess);

                // multiply divisor and guess to check if its closer to remiander value
                node *prod_head = NULL;
                node *prod_tail = NULL;

                node *t2 = head2;
                while(t2->next != NULL)
                {
                    t2 = t2->next;
                }

                multiplication(tail2 , guess_tail,&prod_head,&prod_tail);

                //Does (divisor*guess) fit inside remiander?
                if(compare(prod_head,rem_head) <= 0)
                {
                    //YES!, this guess works
                    // add this guess digit to final quotient result
                    insert_last(headR, tailR, guess);
                    
                    //Subtract product from remainder to get new remainder
                    node*p_tail = prod_head;
                    while(p_tail->next != NULL)
                    {
                        p_tail = p_tail->next;
                    }

                    node *new_rem_head = NULL;
                    node *new_rem_tail = NULL;
                    subtraction(rem_tail,p_tail,&new_rem_head,&new_rem_tail);

                    //update reminder pointer and free old ones
                    free_list(&rem_head,&rem_tail);
                    rem_head = new_rem_head;
                    rem_tail = new_rem_tail;

                    //clean up temporary lists and stop searching for thsi digit
                    free_list(&guess_head,&guess_tail);
                    free_list(&prod_head,&prod_tail);

                    flag = 1;
                    break;
                }

                //NO guess was too big , clean up and try guess-1
                    free_list(&guess_head,&guess_tail);
                    free_list(&prod_head,&prod_tail);
                    guess--;
            } 
            if(!flag)
            {
                if(*headR != NULL)
                {
                    insert_last(headR, tailR,0);
                }
            }  
        }
        temp = temp->next;
    }
    // clean up remaining working remainder list at the end 
    if(rem_head !=NULL)
    {
        free_list(&rem_head,&rem_tail);
    }
}
