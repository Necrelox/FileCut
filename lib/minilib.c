/*
** PineApple PROJECT, 2020
** minilib
** File description:
** minilib
*/

#include "my.h"

char *my_strdup(char *str)
{
    char *str2 = malloc(sizeof(char) * (strlen(str) + 1));
    int i = 0;

    for (; str[i] != '\0'; i++)
        str2[i] = str[i];
    str2[i] = '\0';
    return (str2);
}

int my_strcmp(char *str, char *str2)
{
    int i = 0;
    for (; str[i] == str2[i] && str[i] != '\0' && str2[i] != '\0'; i++);
    return (str[i] - str2[i]);
}