#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> 

typedef enum
{
    false,
    true
} bool;

bool isEmptyFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    if (NULL != file)
    {
        fseek(file, 0, SEEK_END);
        unsigned long int size = (unsigned long int)ftell(file);
        if (size == 0)
        {
            return true;
        }
    }
    fclose(file);
    return false;
}

void add_new_data(char *fileName)
{
    char givenName[20];
    char dateOfBirth[20];
    char phoneNumber[20];
    char extraCharge[20];
    char vaccinated[20];

    printf("\nPlease enter the patient date: \n");
    printf("Name: ");
    scanf("%s", givenName);
    printf("Date of birth: ");
    scanf("%s", dateOfBirth);
    printf("Phone number: ");
    scanf("%s", phoneNumber);
    printf("Has to pay extra charge (Yes, No): ");
    scanf("%s", extraCharge);
    FILE *fp = fopen(fileName, "a");
    fprintf(fp, "%s %s %s %s %s %s", givenName, dateOfBirth, phoneNumber, extraCharge, vaccinated ,"\n");
    fclose(fp);
}

void modify_data(char *fileName)
{
    FILE *fptr1, *fptr2;
    char c;
    int delete_line, temp = 1;

    fptr1 = fopen(fileName, "r");
    int linesCount = 0;
    for (c = getc(fptr1); c != EOF; c = getc(fptr1))
    {
        if (c == '\n')
            linesCount++;
    }

    rewind(fptr1);
    if (isEmptyFile(fileName))
    {
        printf("The file is empty so there are no data to moify!\n");
        return;
    }
    printf("\nHere is the content of my file: \n\n");
    c = getc(fptr1);
    while (c != EOF)
    {
        printf("%c", c);
        c = getc(fptr1);
    }

    bool van = false;
    do
    {
        van = false;
        printf("\nEnter line number to be modified: ");
        scanf("%d", &delete_line);
        if (delete_line < 0 || delete_line > linesCount)
        {
            printf("You entered a wrong line number please try to enter again!\n");
            van = true;
        }
    } while (van);

    rewind(fptr1);
    if (delete_line == 1 && linesCount == 2)
    {
        fptr1 = freopen(NULL, "w", fptr1);
    }
    fptr2 = fopen("replace.c", "w");
    c = getc(fptr1);
    while (c != EOF)
    {
        if (c == '\n')
        {
            temp++;
        }
        if (temp != delete_line)
        {
            putc(c, fptr2);
        }
        else
        {
            while ((c = getc(fptr1)) != '\n')
            {
            }
            printf("\nEnter new data (Name , date of birth , phone number , extra Charge(Yes , No), vacconated(Yes, No) ): ");
            fflush(stdin);
            putc('\n', fptr2);
            while ((c = getchar()) != '\n')
                putc(c, fptr2);
            fputs("\n", fptr2);
            temp++;
        }
        c = getc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
    remove(fileName);
    rename("replace.c", fileName);
    printf("\nThe contents of file after being modified are as follows:\n");
    fptr1 = fopen(fileName, "r");
    c = getc(fptr1);
    while (c != EOF)
    {
        printf("%c", c);
        c = getc(fptr1);
    }
    fclose(fptr1);
}

void delete_data(char *filename)
{
    FILE *fptr1, *fptr2;
    char ch;
    int delete_line, temp = 1;

    fptr1 = fopen(filename, "r");

    int linesCount = 0;
    for (ch = getc(fptr1); ch != EOF; ch = getc(fptr1))
    {
        if (ch == '\n')
            linesCount++;
    }

    rewind(fptr1);

    if (isEmptyFile(filename))
    {
        printf("The file is empty so there are no data to moify!\n");
        return;
    }

    printf("\nHere is the content of my file: \n\n");

    ch = getc(fptr1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fptr1);
    }
    rewind(fptr1);

    bool van = false;

    do
    {
        van = false;
        printf("\nEnter line number to be deleted: ");
        scanf("%d", &delete_line);
        if (delete_line < 0 || delete_line > linesCount)
        {
            printf("\nYou entered a wrong line number please try to enter again!\n");
            van = true;
        }
    } while (van);

    fptr2 = fopen("replica.c", "a");
    ch = getc(fptr1);
    while (ch != EOF)
    {
        if (ch == '\n')
            temp++;
        if (temp != delete_line)
        {
            putc(ch, fptr2);
        }
        ch = getc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
    remove(filename);
    rename("replica.c", filename);
    printf("\nThe contents of file after being modified are as follows:\n");
    fptr1 = fopen(filename, "r");
    ch = getc(fptr1);
    while (ch != EOF)
    {
        printf("%c", ch);
        ch = getc(fptr1);
    }
    fclose(fptr1);
}

void create_total_list(char *fileName)
{
    int list_size;
    printf("\nPlease enter the size of the list that you want to creat: ");
    scanf("%d", &list_size);
    for (int i = 0; i < list_size; i++)
    {
        add_new_data(fileName);
    }
}

void print_date(char *fileName)
{
    if (isEmptyFile(fileName) == false)
    {
        FILE *file = fopen(fileName, "r");
        printf("\n");
        char c = getc(file);
        while (c != EOF)
        {
            printf("%c", c);
            c = getc(file);
        }
        fclose(file);
    }
    else
    {
        printf("\nThe file is Empty !\n");
    }
}

