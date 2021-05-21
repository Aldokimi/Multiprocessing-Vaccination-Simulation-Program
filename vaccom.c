#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //fork and pipe
#include <sys/wait.h> //waitpid
#include <sys/time.h> //for rand
#include <signal.h>
#include <string.h>
#include "vaccination.h"
#include <time.h>

#define FIGHT_UP 1515

//the hundler function of the signal!
void handler(int signumber)
{
    printf("Signal with number %i has arrived\n", signumber);
}

int randomGernerator()
{
    return rand() % 100;
}

//the main function to handle the program!
void operating_committee(char *fileName)
{
    srand(time(NULL));

    //reding the file into array
    int nonvac_cnt = 0; // the count of the vaccinated people
    char nonvac_arr[30][100]; // the non-vaccinated people
    char arr[30][100]; // the array which will hold the content of the file

    FILE *fp;
    char *line = (char *)malloc(100);
    size_t len = 0;
    ssize_t rread = 0;
    fp = fopen(fileName, "r");
    if (fp == NULL) exit(EXIT_FAILURE);
    int arraySize = 0;
    while ((rread = getline(&line, &len, fp)) != -1)
    {
        strcpy(arr[arraySize], line);
        if (arr[arraySize][strlen(line) - 3] == 'N')
        {
            strcpy(nonvac_arr[nonvac_cnt], arr[arraySize]);
            nonvac_cnt++;
        }
        arraySize++;
    }
    fclose(fp);
    
    if (line) free(line);
    //done with reading the file 


    //the declarations!
    pid_t child_a, child_b; //for the fork() function
    signal(SIGTERM, handler); //our signal

    //the pipe declarations
    int pipefd[2];
    int pipefd2[2];
    int pipefd3[2];
    int pipefd4[4];
    
    if (pipe(pipefd) == -1)
    {
        perror("Opening error!");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd2) == -1)
    {
        perror("Opening error!");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd3) == -1)
    {
        perror("Opening error!");
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd4) == -1)
    {
        perror("Opening error!");
        exit(EXIT_FAILURE);
    }
    //done declaring!

    // check the number of the non-vaccinated people
    if(nonvac_cnt < 5){
        printf("The number of non-vaccinated people is less than 5!");
        exit(EXIT_FAILURE);
    }

    // starting the first car
    child_a = fork();
    if (child_a < 0)
    {
        perror("The fork calling was not succesful\n\n");
        exit(EXIT_FAILURE);
    }
    else if (child_a == 0)
    {
    //signal process
            
        kill(getppid(), SIGTERM);
        sleep(1);

    // pipe process
        printf("***First car is workin!***\n\n");
        printf("we recived those people from the Operating Committee!...\n\n");
        close(pipefd[1]);
        close(pipefd2[0]);

        char reCh[6][100];
        int size ;

        //reading from the parent
        for(int i = 0 ; i < 5 ; i++){
            read(pipefd[0], &size , sizeof(int));
            read(pipefd[0], reCh[i] , size);
            printf("    : %s",reCh[i]);
        }

        //sending to the parent
        for(int i = 0 ; i < 5 ; i++){
            if(randomGernerator() >= 90){
                int n = strlen(reCh[i]);
                write(pipefd2[1], &n , sizeof(int));
                write(pipefd2[1], reCh[i] , n);
            }
        }

        close(pipefd[0]);
        close(pipefd2[1]);
        exit(1);
    }
    else
    {   
        printf("***Operating Committee Starts!***\n\n");
        close(pipefd[0]);
        close(pipefd2[1]);
        //signal process
        pause();
        printf("***We got the fight up signal .. lets GOOO!***\n\n");

        //printing the people who will get vaccinated
            
        char toSend[300];
        printf("The people who will get vaccinated are: \n\n");
        for (int i = 0; i < 5; i++){
            printf("    %s", nonvac_arr[i]);
        }
        printf("\n\n");
                
            
        //pipe process
           
        // writing to the child
        for(int i = 0 ; i < 5 ; i++){
            int n = strlen(nonvac_arr[i]);
            write(pipefd[1], &n , sizeof(int));
            write(pipefd[1], nonvac_arr[i] , n);
        }
            
        printf("We sent the people who will need to get vaccinated to the first car!\n\n");
        sleep(2);
        printf("***Back to the Operating Committee***\n\n");
        printf("We resived those people from the car: \n\n");

        char reCh[6][100];
        int size ;
        for(int i = 0 ; i < 5 ; i++){
            read(pipefd2[0], &size , sizeof(int));
            read(pipefd2[0], reCh[i] , size);
            printf("    %s",reCh[i]);
        }
        if(sizeof(reCh)/ sizeof(reCh[0]) == 0){
            printf("\nWe didn't recive anything from the first car !");
        }
                
        close(pipefd[1]);
        close(pipefd2[0]);
        fflush(NULL);

        printf("\nThe file will be Modified!\n\n");
        sleep(3);

        FILE *fp = fopen(fileName, "w");
        for(int i = 0 ; i < arraySize ; i++){
            int van = 0;
            for(int j = 0 ; j < arraySize ; j++){
                if(strcmp(reCh[i] , arr[j]) == 0 ){
                    char new_string[100] = "XXXXXXXXXXXXXXXX";
                    memcpy(new_string, arr[j], strlen(arr[j]) - 4);
                    strcat(new_string , " Yes\n");
                    for(int k = 0 ; new_string[k] != '\0';k++){
                        arr[j][k] = new_string[k];
                    }
                    break;
                }
            }
        }
        for(int i = 0 ; i < arraySize ; i++){
            fprintf(fp, "%s", arr[i]);
        }
            
        printf("\n The content of my file now is : \n");
        char c = getc(fp);
        while (c != EOF)
        {
            printf("%c", c);
            c = getc(fp);
        }

        fclose(fp);
        sleep(1);
        printf("\n\n***We are done with the first car!***");
    }

    // the second car process
    if(nonvac_cnt >= 10){
        sleep(3);
        child_b = fork();
        if (child_b < 0)
        {
            perror("The fork calling was not succesful\n");
            exit(EXIT_FAILURE);
        }
        else if (child_b == 0)
        {
        //signal process
            
            kill(getppid(), SIGTERM);
            sleep(1);

        // pipe process
            printf("***Second car is workin!***\n\n");
            printf("We recived those people from the Operating Committee!...\n\n");

            close(pipefd3[1]);
            close(pipefd4[0]);

            char reCh[6][100];
            int size ;

            for(int i = 0 ; i < 5 ; i++){
                read(pipefd3[0], &size , sizeof(int));
                read(pipefd3[0], reCh[i] , size);
                printf("    %s\n",reCh[i]);
            }

            for(int i = 0 ; i < 5 ; i++){
                if(randomGernerator() <= 90){
                    int n = strlen(reCh[i]);
                    write(pipefd4[1], &n , sizeof(int));
                    write(pipefd4[1], reCh[i] , n);
                }
            }

            close(pipefd3[0]);
            close(pipefd4[1]);
        }
        else
        {
            printf("\n***Operating Committee Starts!***\n\n");
            close(pipefd3[0]);
            close(pipefd4[1]);
            //signal process
            pause();
            printf("***We got the fight up signal .. lets GOOO!***\n\n");

            //printing the people who will get vaccinated
            
            char toSend[300];
            printf("The people who will get vaccinated are: \n\n");
            for (int i = 5; i < 10; i++){
                printf("    %s", nonvac_arr[i]);
            }
            printf("\n\n");
                    
                
            //pipe process
            
            
            for(int i = 5 ; i < 10 ; i++){
                int n = strlen(nonvac_arr[i]);
                write(pipefd3[1], &n , sizeof(int));
                write(pipefd3[1], nonvac_arr[i] , n);
            }
                
            printf("We sent the people who will need to get vaccinated to the first car!\n\n");
            sleep(2);
            printf("\n***Back to the Operating Committee***\n\n");
            printf("We resived those people from the car: \n\n");

            char reCh[6][100];
            int size ;
            for(int i = 0 ; i < 5 ; i++){
                read(pipefd4[0], &size , sizeof(int));
                read(pipefd4[0], reCh[i] , size);
                printf("    %s",reCh[i]);
            }
            if(sizeof(reCh)/ sizeof(reCh[0]) == 0){
                printf("\nWe didn't recive anything from the second car !");
            }
                    
            close(pipefd3[1]);
            close(pipefd4[0]);
            fflush(NULL);
                

            printf("\nThe file will be Modified!\n\n");
            sleep(3);

        FILE *fp = fopen(fileName, "w");
        for(int i = 0 ; i < arraySize ; i++){
            int van = 0;
            for(int j = 0 ; j < arraySize ; j++){
                if(strcmp(reCh[i] , arr[j]) == 0 ){
                    char new_string[100] = "XXXXXXXXXXXXXXXX";
                    memcpy(new_string, arr[j], strlen(arr[j]) - 4);
                    strcat(new_string , " Yes\n");
                    for(int k = 0 ; new_string[k] != '\0';k++){
                        arr[j][k] = new_string[k];
                    }
                    break;
                }
            }
        }
        for(int i = 0 ; i < arraySize ; i++){
            fprintf(fp, "%s", arr[i]);
        }
                
            printf("\n The content of my file now is : \n");
            char c = getc(fp);
            while (c != EOF)
            {
                printf("%c", c);
                c = getc(fp);
            }

            fclose(fp);

            printf("\n\n***The Operating Committee***");
        }
    }
}