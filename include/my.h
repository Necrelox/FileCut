/*
** PineApple PROJECT, 2020
** myh
** File description:
** myh
*/

#ifndef MY_H_
#define MY_H_

#include <stdio.h>
#include <fcntl.h>
#ifdef WIN32
    #include <window.h>
    #include <process.h>
#else
    #include <unistd.h>
#endif
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "mtwister.h"

/*     LIB/minilib.c      */
/* Compare two str if same str this function return 0*/
int my_strcmp(char *str, char *str2);
/* Return a str duplicate */
char *my_strdup(char *str);

#endif /* !MY_H_ */