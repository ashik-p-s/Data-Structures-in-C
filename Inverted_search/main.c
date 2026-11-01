#include<stdio.h>
#include "main.h"

hash_t HT_array[27];

int main(int argc,char *argv[])
{

    Flist *head = NULL;    
    if(cla_validation(argc,argv,&head)== SUCCESS)
    {
        printf("CLA Validation Successful.\n");
    }
    else
    {
        printf(RED"ERROR: CLA Validation Failed.\n"RESET);
        return FAILURE;
    }

    int choice;
    int is_created = 0;
    int is_updated = 0;
    
    do
    {
        printf(GREEN"\nSelect a choice : \n"RESET);
        printf("1.Create DATABASE\n2.Display DATABASE\n3.Save Database\n4.Search\n5.Update DATABASE\n6.Exit\n");
        printf("Enter the choice : ");
        if(scanf("%d",&choice) != 1)
        {
            printf("Invalid choice");
            return 0;
        }

        switch(choice)
        {
            case 1 : 
                    if(is_created == 1)
                    {
                        printf(RED"Error : Database has already been declared.\n"RESET);
                        break;
                    }

                    if(create_database(head,HT_array) == SUCCESS)
                    {
                        is_created = 1;
                    }
                    break;
            case 2 :
                    display(HT_array);
                    break;
            case 3 :
                    save(HT_array);
                    break;
            case 4 :
                    search(HT_array);
                    break;
            case 5 :
                    if(is_updated == 1)
                    {
                        printf(RED"Error : Database has already been updated from a backup.\n"RESET);
                        break;
                    }
                    if(is_created == 1)
                    {
                        printf(RED"Error : Cannot update database after creating it from source file.\n"RESET);
                        break;
                    }

                    if(update(HT_array) == SUCCESS)
                    {
                        is_updated = 1;
                    }
                    break;
            case 6 : 
                    printf("Exiting Program...\n");
                    return 0;
            default : 
                    printf("Invalid choice!\n");

        }
    }while(choice != 6);
    return SUCCESS;
    
}

