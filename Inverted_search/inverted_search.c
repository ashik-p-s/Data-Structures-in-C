#include "main.h"
#include<string.h>

#include <stdio.h>
#include <stdlib.h>


Status cla_validation(int argc, char *argv[],Flist **head)
{
    // min 2 files need to search
    if(argc < 2)
    {
        printf(RED"Error: Insufficient files.\n"RESET);
        return FAILURE;
    }

    int valid_file_count = 0;
    for(int i=1 ; i < argc ; i++)
    {
        // checking for .txt extension
        if(strlen(argv[i]) < 4 || strcmp(argv[i] + strlen(argv[i]) - 4,".txt") != 0)
        {
            printf("Incorrect file formats.\n");
            continue;// ignore and continue
        }

        //check if file exist and can be opened
        FILE *fp = fopen(argv[i],"r");
        if(fp == NULL)
        {
            printf(RED"Error: File %s doesnot exist.\n "RESET,argv[i]);
            continue;
        }

        // to check if file is empty
        fseek(fp,0,SEEK_END);// move pointer till end of file
        if(ftell(fp) == 0)// ftell gives current byte position
        {
            printf(RED"Error: File %s is empty.\n"RESET,argv[i]);
            fclose(fp);
            continue;
        }

        // checking duplicate elements in previous arguments
        int is_duplicate = 0;
        for(int k = 1;k<i;k++)
        {
            if(strcmp(argv[i],argv[k]) == 0)
            {
                printf(RED"Error: Duplicate file %s passed.\n"RESET,argv[i]);
                is_duplicate = 1;
                break;
            }
        }
        if(is_duplicate)
        {
            fclose(fp);
            continue;// ignore this file and continue to next one 
        }
        
        fclose(fp);

        //ALL checks passed : add to valid file linked list
        Flist *new = malloc(sizeof(Flist));
        if(new == NULL)
        {
            printf("Memory allocation failed for the file node.\n");
            return FAILURE;
        }
        strcpy(new->file_name,argv[i]);
        new->link = NULL;
        if(*head == NULL)
        {
            *head = new;
        } 
        else
        {
            Flist *temp = *head;
            while(temp->link != NULL)
            {
                temp = temp->link;
            }
            temp->link = new;
        }
        valid_file_count++;
    }
    if(valid_file_count == 0)
    {
        printf(RED"Error: No valid files to process.\n"RESET);
        return FAILURE;
    }
    return SUCCESS;
}


// create database fucntion
Status create_database(Flist *head,hash_t HT_array[])
{
    //Traverse through each validated files in file list
    Flist *file_temp = head;
    while(file_temp != NULL)
    {
        FILE *fp = fopen(file_temp->file_name,"r");
        if(fp == NULL)
        {
            printf(RED"Error : could not open file %s\n"RESET,file_temp->file_name);
            file_temp = file_temp->link;
            continue;
        }
        char buffer[50];
        // read word by word from file
        while(fscanf(fp,"%s",buffer) == 1)
        {
            int index = buffer[0] - 'a';
            if(index < 0 || index > 25)// 26th index holds non alphabetic characters
            {
                index = 26;
            }

            // Case 1 : Bucket is empty.so create new main node and sub node and link each other.
            if(HT_array[index].link == NULL)
            {
                main_node *new_main = malloc(sizeof(main_node));
                if(new_main == NULL)
                {
                    return FAILURE;
                }

                strcpy(new_main->word,buffer);
                new_main->file_count = 1;
                new_main->link = NULL;

                sub_node *new_sub = malloc(sizeof(sub_node));
                if(new_sub == NULL)
                {
                    free(new_main);
                    fclose(fp);
                    return FAILURE;
                }

                strcpy(new_sub->file_name,file_temp->file_name);
                new_sub->word_count = 1;
                new_sub->link = NULL;

                new_main->sub_link = new_sub;// assigning subnode address to main_node's sublink.
                HT_array[index].link = new_main;//assigning main_node address to array->link
                //so far attached node to bucket head
            }
            else
            {
                //Case 2 :bucket is not empty. so search for word in mainlist.
                main_node *temp_main = HT_array[index].link;
                //main_node *prev_main = NULL;
                int word_found = 0;

                while(temp_main != NULL)
                {
                    if(strcmp(temp_main->word,buffer) == 0)// word comparison in main_node
                    {
                        word_found = 1;
                        break;
                    }
                    //prev_main = temp_main;
                    temp_main = temp_main->link;
                }
                if(word_found)
                {
                    // word found so search for filename exist or not in sub_node link
                    sub_node *temp_sub = temp_main->sub_link;
                    sub_node *prev_sub = NULL;
                    int file_found = 0;

                    while(temp_sub != NULL)
                    {
                        // file name comparison in sub node
                        if(strcmp(temp_sub->file_name,file_temp->file_name) == 0)
                        {
                            file_found = 1;
                            break;
                        }
                        prev_sub = temp_sub;
                        temp_sub = temp_sub->link;
                    }

                    if(file_found)
                    {
                        // found so incrementing the wordcount
                        temp_sub->word_count++;
                    }
                    else
                    {
                        //not found so create a new sub_node to store it 
                        sub_node *new_sub = malloc(sizeof(sub_node));
                        if(new_sub == NULL)
                        {
                            return FAILURE;
                        }

                        strcpy(new_sub->file_name,file_temp->file_name);
                        new_sub->word_count = 1;
                        new_sub->link = NULL;

                        prev_sub->link = new_sub;
                        temp_main->file_count++;
                    }
                }
                else
                {
                    // if word is not found in main list
                    //create a new mainnode and attach at the end
                    main_node *new_main = malloc(sizeof(main_node));
                    if(new_main == NULL)
                    {
                        return FAILURE;
                    }
                    strcpy(new_main->word,buffer);
                    new_main->file_count = 1;
                    new_main->link = NULL;

                    sub_node *new_sub = malloc(sizeof(sub_node));
                    if(new_sub == NULL)
                    {
                        free(new_main);
                        return FAILURE;
                    }

                    strcpy(new_sub->file_name,file_temp->file_name);
                    new_sub->word_count = 1;
                    new_sub->link = NULL;

                    new_main->sub_link = new_sub;
                    //Alphabetic Insertion Logic
                    main_node *curr = HT_array[index].link;
                    main_node *prev = NULL;
                    
                    //Traverse where to find the new words fits alphabetically
                    while(curr != NULL && strcmp(curr->word , buffer) < 0)
                    {
                        prev = curr;
                        curr = curr->link;
                    }

                    //Insert new_main between prev and curr
                    new_main->link = curr;
                    if(prev == NULL)
                    {
                        HT_array[index].link = new_main;
                    }
                    else
                    {
                        prev->link = new_main;
                    }
                }
            }
        }
    fclose(fp);
    file_temp = file_temp->link;
    }

    printf("Database created successfully.\n\n");
    return SUCCESS;
}


Status display(hash_t HT_array[])
{

    printf("\n-------------------------------------------------------------------------------\n");
    printf(RED"                  DATABASE DISPLAY                  \n"RESET);
    printf("---------------------------------------------------------------------------------\n");


    int i = 0;
    int is_empty = 1;
    for(i = 0; i < 27; i++)
    {
        if(HT_array[i].link != NULL)
        {
            printf("\033[32m[ %-2d ] ",i);
            is_empty = 0;
            main_node *temp_main = HT_array[i].link;

            while(temp_main != NULL)
            {
                // \033[32m starts green, \033[0m resets it back to normal
                // %-15s and %-3d add clean, proper spacing/alignment.
                printf("\033[32m%-15s file_count: %-3d -> \033[0m ",temp_main->word, temp_main->file_count);

                sub_node *temp_sub = temp_main->sub_link;

                while(temp_sub != NULL)
                {
                    printf("\033[32mfile : %-10s, word_count : %-3d | ",temp_sub->file_name,temp_sub->word_count);

                    temp_sub = temp_sub->link;
                }
                printf("\033[0m\n");
                temp_main = temp_main->link;
            }
        }
    }

    if(is_empty)
    {
        printf("Database is empty.\n");
    }
    printf("---------------------------------------------------------------------------------\n");
    return SUCCESS;
}

Status save(hash_t HT_array[])
{
    char file_name[100];
    printf("Enter the file_name to save database(.txt extension) : ");
    scanf("%s",file_name);

    // if given file name has .txt extension or not?
    if(strlen(file_name) < 4 || strcmp((file_name + strlen(file_name) - 4),".txt") != 0)
    {
        printf(RED"Error: Invalid file format. Must be a .txt file.\n"RESET);
        return FAILURE;
    }

    //check if file already exist or not
    FILE *fp = fopen(file_name,"r");
    if(fp != NULL)
    {
        fclose(fp);
        int choice;
        printf("Warning: File '%s' already exists.\n", file_name);
        printf("1.Overwrite.\n2. Cancel / New filename\n");
        printf("Enter the choice : ");

        if(scanf("%d",&choice) != 1)
        {
            printf("Invalid input.\n");
            return FAILURE;
        }

        if (choice != 1)
        {
            printf("Save operation cancelled.\n");
            return FAILURE;
        }
    }

    // open the file in write mode
    fp = fopen(file_name,"w");
    if(fp == NULL)
    {
        printf(RED"Error: Could not open file for writing.\n"RESET);
        return FAILURE;
    }

    //Traverse the hash table and write to the file
    for(int i = 0; i < 27; i++)
    {
        if(HT_array[i].link != NULL)
        {
            main_node *temp_main = HT_array[i].link;
            while(temp_main != NULL)
            {
                sub_node *temp_sub = temp_main->sub_link;

                while(temp_sub != NULL)
                {
                    fprintf(fp, "# %d ; %s ; %d ; %s ; %d #\n",
                            i,
                            temp_main->word,
                            temp_main->file_count,
                            temp_sub->file_name,
                            temp_sub->word_count);
                    
                    temp_sub = temp_sub->link;
                }
                temp_main = temp_main->link;
            }
        }
    }
    //close the file
    fclose(fp);
    printf("Database successfully saved to %s.\n",file_name);
    return SUCCESS;
}

Status search(hash_t HT_array[])
{
    char search[100];
    printf("Enter the word to be searched :");
    scanf("%s",search);

    // find hadh index based on the first charcter user's search input
    int index = search[0] - 'a';
    if(index < 0 || index > 25)
    {
        index = 26;//Non alphabetic bucket
    }
    //check if bucket is empty?
    if(HT_array[index].link == NULL)
    {
        printf("Word not found in the database.\n");
        return  FAILURE;
    }

    //Traverse through the main node list
    main_node *temp_main = HT_array[index].link;
    int word_found = 0;
    while(temp_main != NULL)
    {
        if(strcmp(search,temp_main->word) == 0)
        {
            word_found =1;
            printf(GREEN"\n----------------------------------------\n");
            printf(GREEN"Word Found: %s\n",temp_main->word);
            printf(GREEN"Total File Count: %d\n",temp_main->file_count);

            //Traverse sub_nodes to print the file names and word counts
            sub_node *temp_sub = temp_main->sub_link;
            while(temp_sub != NULL)
            {
                printf("\033[32m  -> File: %s | Word Count: %d\n", temp_sub->file_name, temp_sub->word_count);
                temp_sub = temp_sub->link;
            }
            printf("----------------------------------------\n"RESET);
            break;
        }
        temp_main = temp_main->link;
    }

    //If traversed till the end and word is not yet found
    if(word_found == 0)
    {
        printf("Word not found in the database.\n");
        return FAILURE;
    }
    return SUCCESS;
}



Status update(hash_t HT_array[])
{
    char filename[100];
    printf("Enter the backup file name to update : ");
    scanf("%s",filename);

    // validate .txt extension
    if(strlen(filename) < 4 || strcmp((filename + strlen(filename) - 4),".txt") != 0)
    {
        printf("Error: Invalid file format. Must be a .txt file.\n");
        return FAILURE;
    }

    // open the file in read mode
    FILE *fp = fopen(filename , "r");
    if(fp == NULL)
    {
        printf(RED"Error: Could not open backup file '%s'.\n"RESET, filename);
        return FAILURE;
    }

    // check if file starts with # .
    char ch;
    if(fscanf(fp, " %c", &ch) != 1 || ch != '#')
    {
        printf(RED"Error: Invalid backup file format (does not start with #).\n"RESET);
        fclose(fp);
        return FAILURE;
    }
    rewind(fp);

    int index;
    char word[50];
    int file_count;
    char fname[100];
    int word_count;

    // read file contents line by line until EOF 
    while(fscanf(fp," # %d ; %s ; %d ; %s ; %d #",&index,word,&file_count,fname,&word_count) == 5)
    {
        // 
        main_node *temp_main = HT_array[index].link;
        int word_found = 0;
        
        while(temp_main != NULL)
        {
            if(strcmp(temp_main->word,word) == 0)
            {
                word_found = 1;
                break;
            }
            temp_main = temp_main->link;
        }

        // createsub node for this file entry
        sub_node *new_sub = malloc(sizeof(sub_node));
        if(new_sub == NULL)
        {
            printf("Memmory allocation failed for sub_node.\n");
            fclose(fp);
            return FAILURE;
        }
        strcpy(new_sub->file_name,fname);
        new_sub->word_count = word_count;
        new_sub->link = NULL;

        if(word_found)
        {
            //word  exists and insert the sub node at begining of sub_link
            new_sub->link = temp_main->sub_link;
            temp_main->sub_link = new_sub;
        }
        else
        {
            // word doesnot exist so create a new main_node
            main_node *new_main = malloc(sizeof(main_node));
            if(new_main == NULL)
            {
                printf("Memory alocation failed for main_node.\n");
                free(new_main);
                fclose(fp);
                return FAILURE;
            }
            strcpy(new_main->word,word);
            new_main->file_count = file_count;
            new_main->sub_link = new_sub;
            new_main->link = NULL;

            // insert new main at the head of hash bucket
           // ALPHABETIC INSERTION LOGIC FOR UPDATE:
            main_node *curr = HT_array[index].link;
            main_node *prev = NULL;

            while(curr != NULL && strcmp(curr->word, word) < 0)
            {
                prev = curr;
                curr = curr->link;
            }

            if(prev == NULL)
            {
                // Insert at the very head of the bucket
                new_main->link = HT_array[index].link;
                HT_array[index].link = new_main;
            }
            else
            {
                // Insert between prev and curr
                new_main->link = curr;
                prev->link = new_main;
            }
        }
    }

    fclose(fp);
    printf("Database successfully updated from %s.\n",filename);
    return SUCCESS;
}
