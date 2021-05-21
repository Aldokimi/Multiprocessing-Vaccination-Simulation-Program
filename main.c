#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>      
#include <unistd.h>
#include "menu.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        perror("Give a filename as a command line argument\n");
        exit(1);
    }
    if (!access(argv[1], F_OK) == 0)
    {
        perror("The file is not exist!\n");
        exit(1);
    }
    menu(argv[1]);

    return 0;
}