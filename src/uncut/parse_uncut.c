/*
** EPITECH PROJECT, 2020
** parse_uncut
** File description:
** pars for uncut multiples files
*/

#include "filecut.h"

/* cree un bon chemin */
char *check_and_modif_path(char *path)
{
    int i = 0;
    char *newpath;
    char *samePath = "./";

    if (path != NULL) {
        for (i = 0; path[i] != '\0'; i++);
        i--;
        if (path[i] != '/') {
            newpath = malloc(sizeof(char) * (strlen(path) + 2));
            sprintf(newpath, "%s/", path);
        }
        else if (path[i] == '/') {
            newpath = malloc(sizeof(char) * (strlen(path) + 2));
            sprintf(newpath, "%s", path); 
        }
    }
    if (path == NULL) {
        newpath = malloc(sizeof(char) * (4));
        for (i = 0; samePath[i] != '\0'; i++)
            newpath[i] = samePath[i];
        newpath[i] = '\0';
    }
    return (newpath);
}

int verif_is_repo(char *path)
{
    struct stat info;
    stat(path, &info);
    if (S_ISDIR(info.st_mode) == 1)
        return (1);
    return (84);
}

int parse_uncut(char **av)
{
    /* Permet d'avoir un chemin correct*/
    char *path = check_and_modif_path(av[2]);
    if (verif_is_repo(path) == 84) { // verif que ce soit bien un répertoire et non un fichier sinon il retourne 84
        free (path);
        return (84);
    }
    if (uncut(path) == 84) { // algo de remise à l'état initiale
        free (path);
        return (84);
    }
    free (path);
    return (0);
}