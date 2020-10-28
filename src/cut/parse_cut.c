/*
** PineApple PROJECT, 2020
** Parse_Cut
** File description:
** Parser for Cut
*/

#include "filecut.h"

int print_all_error(filecut_s *filecut, char **av)
{
    int bool = 0;
    switch (filecut->error)
    {
    case ERROR_ARG3 :
        printf ("%sERROR ARGUMENT %s%s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[3],ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_SMALL_FILES :
        printf ("%sERROR %s%s%s THIS FILE IS TOO SMALL NEED MINIMUM 4 OCTETS %s\n", ROUGE, VERT, av[2], ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_REPO :
        printf ("%sERROR %s%s%s IS A REPOSITORY %s\n", ROUGE, VERT, av[2], ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_PATH :
        printf ("%sERROR PATH, FILES NOT EXIST : %s%s %s\n", ROUGE, VERT, av[2],NORMAL);
        bool++;
        break;
    default:
        bool = 0;
        break;
    }
    return (bool);
}

int verif_path_of_file(char *path)
{
    struct stat buff;
    if (stat(path, &buff) == -1)
        return (ERROR_PATH);
    if (S_ISDIR(buff.st_mode) == 1)
        return (ERROR_REPO);
    return (0);
}

int verif_if_number(char *str)
{
    int count = 0, i = 0;
    for (i = 0; str[i] != '\0' && str[i] != ' '; i++)
        if (str[i] > 47 && str[i] < 58)
            count++;
    if ((i - count) == 0)
        return (1);
    return (0);
}

filecut_s *verify_arg_and_set_filecut_settings(int ac, char **av)
{
    filecut_s *origin = malloc(sizeof(*origin));
    struct stat info;

    origin->error = 0;
    origin->path = av[2];
    origin->nbFiles = 0;

    if (ac >= 3) {
        if (ac == 4) {
            if (verif_if_number(av[3]) == 0) {
                origin->error = ERROR_ARG3;
                return (origin);
            }
            origin->nbFiles = atoi(av[3]);
        }
        if (origin->nbFiles <= 1)
            origin->nbFiles = 5;
        if ((origin->error = verif_path_of_file(av[2])) != 0)
            return (origin);
        else { 
            stat(av[2], &info);
            origin->originSize = info.st_size;
            if (origin->originSize < 4) {
                origin->error = ERROR_SMALL_FILES;
                return (origin);
            }
        }
    }
    return (origin);
}

int parse_cut(int ac, char **av, int seed)
{
    filecut_s *origin = verify_arg_and_set_filecut_settings(ac, av);
    if (origin == NULL)
        return (ERROR);
    origin->seed = seed;

    if (print_all_error(origin, av) != 0) {
        free (origin);
        return (ERROR);
    }
    cut(origin);
    free (origin->path);
    free (origin);
    return (0);
}