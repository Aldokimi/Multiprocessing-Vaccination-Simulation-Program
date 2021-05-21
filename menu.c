#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> 


void print_menu()
{
    printf("\n\n****************************************\n");
    printf("0. Quit\n");
    printf("1. Add new date to the list\n");
    printf("2. Modify data in the list\n");
    printf("3. Delete data from the list\n");
    printf("4. Creat a total list\n");
    printf("5. Check weather if our list is empty or not\n");
    printf("6. Play the operation of the vaccination\n");
    printf("7. Print our list\n");
    printf("****************************************\n");
}

void menu(char *filename)
{
    bool validation = true;
    do
    {
        print_menu();
        int n;
        scanf("%d", &n);
        switch (n)
        {
        case 0:
            printf("\nThank you, I hope to see you again !\n");
            validation = false;
            break;
        case 1:
            add_new_data(filename);
            break;
        case 2:
            modify_data(filename);
            break;
        case 3:
            delete_data(filename);
            break;
        case 4:
            create_total_list(filename);
            break;
        case 5:
            if (isEmptyFile(filename) == true)
            {
                printf("\nOur file is empty !\n");
            }
            else
            {
                printf("\nOur file contains some data !\n");
            }
            break;
        case 6:
            operating_committee (filename);
        case 7:
            print_date(filename);
            break;
        default:
            break;
        }
    } while (validation);
}