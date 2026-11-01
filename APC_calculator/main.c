#include "apc.h"

void print_division_result(node *headR, int is_neg_result)
{
    remove_pre_zeros(&headR);

    if (headR == NULL || (headR->data == 0 && headR->next == NULL))
    {
        printf("0\n");
        return;
    }

    if (is_neg_result) printf("-");

    // Convert linked list to string buffer
    char buf[2048];
    int len = 0;
    node *curr = headR;
    while (curr != NULL)
    {
        buf[len++] = curr->data + '0';
        curr = curr->next;
    }
    buf[len] = '\0';

    // Separate integer part (everything except last 8 digits) and decimal part (last 8 digits)
    if (len <= 8)
    {
        // Pad with leading zeros if result is less than 1 (e.g., 0.03)
        char frac[9] = "00000000";
        int pad = 8 - len;
        for (int i = 0; i < len; i++) frac[pad + i] = buf[i];
        
        // Trim trailing zeros
        int f_len = 8;
        while (f_len > 0 && frac[f_len - 1] == '0') frac[--f_len] = '\0';

        if (f_len == 0) printf("0\n");
        else printf("0.%s\n", frac);
    }
    else
    {
        int len1 = len - 8;
        
        // Print integer part
        for (int i = 0; i < len1; i++) putchar(buf[i]);

        // Isolate and trim trailing zeros from the 8 decimal digits
        char frac[9];
        for (int i = 0; i < 8; i++) frac[i] = buf[len1 + i];
        frac[8] = '\0';

        int f_len = 8;
        while (f_len > 0 && frac[f_len - 1] == '0') frac[--f_len] = '\0';

        // Print decimal point and decimals only if any remain
        if (f_len > 0)
        {
            printf(".%s\n", frac);
        }
        else
        {
            printf("\n");
        }
    }
}

int main(int argc, char *argv[])
{
    node *head1 = NULL, *tail1 = NULL, *head2 = NULL, *tail2 = NULL, *headR = NULL, *tailR = NULL;

    // Validation of CLA
    if(cla_validation(argc, argv) == FAILURE)
    {
        return FAILURE;
    }

    char sign1 = '+';
    char sign2 = '+';
    char *operand1 = argv[1];
    char *operand2 = argv[3];

    if(argv[1][0] == '+' || argv[1][0] == '-')
    {
        sign1 = argv[1][0];
        operand1 = argv[1] + 1;// move past the sign so create list sees only the digits.
    }

    if(argv[3][0] == '+' || argv[3][0] == '-')
    {
        sign2 = argv[3][0];
        operand2 = argv[3] + 1;// move past the sign so create list sees only the digits.
    }
    
    create_list(operand1, &head1, &tail1);// list for operand1
    create_list(operand2, &head2, &tail2);// list for operand2
    printf("Operand lists created successfully.\n");

    // 3. Remove leading zeros (if you implement remove_pre_zeros)
    remove_pre_zeros(&head1);
    remove_pre_zeros(&head2);

    printf("Operand lists created and cleaned successfully.\n");

    char oper = argv[2][0];
    int is_neg_result = 0;// flag to track if we need to print minus sign to result.

    switch(oper)
    {
    case '+':
        if(sign1 == sign2)// both sign equal
        {
            addition(tail1, tail2, &headR, &tailR);// both sign '+
            if(sign1 == '-')//both sign '-'
            {
                is_neg_result = 1;
            }
        }
        else//one sign '+' && '-'
        {
            int cmp = compare(head1, head2);
            if(cmp >= 0)
            {
                //num1 > num2
                subtraction(tail1, tail2, &headR, &tailR);
                if(sign1 == '-')
                    is_neg_result = 1;
            }
            else
            {
                //num2 < num1 swap to avoid issues.
                subtraction(tail2, tail1, &headR, &tailR);//final result negative.
                if(sign2 == '-')
                    is_neg_result = 1;
            } 
        }
        break;

    case '-':
        // For operator '-', the mathematical operation is determined by signs:
        // +A - (+B) -> A - B
        // -A - (-B) -> -A + B  == B - A
        // +A - (-B) -> A + B   (Addition!)
        // -A - (+B) -> -A - B  (Addition with negative result!)
        if(sign1 == sign2)// e.g. (+A - +B) or (-A - -B)
        {
            if(sign1 == '+')
            {
                int cmp = compare(head1, head2);
                if(cmp >= 0)
                {
                    //num1 > num2
                    subtraction(tail1, tail2, &headR, &tailR);
                }
                else
                {
                    //num2 < num1 swap to avoid issues.
                    subtraction(tail2, tail1, &headR, &tailR);//final result negative.
                    is_neg_result = 1;
                }
            }
            else// -A - (-B) => B - A
            {
                int cmp = compare(head1, head2);
                if(cmp >= 0)
                {
                    subtraction(tail1, tail2, &headR, &tailR);
                    is_neg_result = 1;
                }
                else
                {
                    subtraction(tail2, tail1, &headR, &tailR);
                }
            }
        }
        else  // Mixed signs (+A - -B becomes A + B, or -A - +B becomes -(A + B))
        {
            addition(tail1, tail2, &headR, &tailR);
            if(sign1 == '-')
            {
                is_neg_result = 1;
            }
        }
        break;

    case 'x':
    case 'X':
    case '*':
        multiplication(tail1, tail2, &headR, &tailR);
        if(sign1 != sign2)
        {
            is_neg_result = 1;
        }
        break;

    case '/':
            {

                // check if dividend is 0
                if(head1->data == 0 && head1->next == NULL)
                {
                    printf("0\n");
                    return SUCCESS;
                }

                // check if divisor is zero(head2 == 0)
                else if(head2->data == 0 && head2->next == NULL)
                {
                    printf("Division by zero error.\n");
                    return FAILURE;
                }
                else
                {
                    int cmp = compare(head1, head2);
                    if(cmp == 0)
                    {
                        // Dividend == Divisor -> Result is exactly 1
                        if(sign1 != sign2) 
                            printf("-");
                        printf("1\n");
                        return SUCCESS;
                    }
                    else
                    {
                        // STEP 1: Scale dividend by 10^8 (handles both large numbers and fractions like 2 / 10)
                        for (int i = 0; i < 8; i++)
                        {
                            insert_last(&head1, &tail1, 0);
                        }
                        
                        // Ensure tail1 points to the absolute last node after appending zeros
                        tail1 = head1;
                        while(tail1->next != NULL)
                        {
                            tail1 = tail1->next;
                        }

                        // STEP 2: Call division function
                        division(head1, head2, &headR, &tailR);
                    }
                }

                // Division sign rule: different signs yield a negative result
                if(sign1 != sign2)
                {
                    is_neg_result = 1;
                }

                // STEP 3: Call the helper function to print cleanly formatted results
                print_division_result(headR, is_neg_result);
                return SUCCESS;
            }
            break;

    default:
        printf("Invalid operator\n");
        return FAILURE;
    }

    remove_pre_zeros(&headR); //// Clean up any leading zeros that subtraction/addition might have created
    
    // print final list
    if(is_neg_result && !(headR->data == 0 && headR->next == NULL))
    {
        printf("-");
    }
    print_list(headR);

    return SUCCESS;
}




/*1. Before Division (The Scaling): You multiply the dividend by $10^8$ (by appending 8 zeros) to force 
your integer division to calculate 8 extra digits of precision.2. During Division: Your integer division function runs 
normally on the scaled-up numbers, treating the result as a giant whole number string (e.g., 625000000).3. After Division (The Formatting): 
Instead of dividing the final answer mathematically, we simply count 8 digits from the right.4. Printing & Trimming: 
We drop a decimal point (.) right at that 8-digit mark 
and strip away any unnecessary trailing zeros (turning 6.25000000 into 6.25).*/