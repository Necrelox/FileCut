/*
** PineApple PROJECT, 2020
** Main
** File description:
** Start
*/

#include "filecut.h"

int help (void) 
{
    printf ("%s\t\tWelcome on FileCut\n\n%s", VERT, NORMAL);
    printf ("How to use FileCut?\n");
    printf ("\tFor %sCut%s execute : %s./FileCut cut pathtofile [nFiles] [minSize] [maxSize]%son your shell %s( '[]' is optionnal argument )%s\n", CYAN, NORMAL, JAUNE, NORMAL, MAGENTA, NORMAL);
    printf ("\tFor %sUncut%s execute : %s./FileCut uncut [pathofdirectory]%son your shell %s( '[]' is optionnal argument )%s\n", CYAN, NORMAL, JAUNE, NORMAL, MAGENTA, NORMAL);
    return 0;
}

int main (int ac, char **av)
{
    #ifdef WIN32
        int seed = _getpid();
    #else
        int seed = getpid();
    #endif
    if (ac > 1 && ac < 5) {
        if (my_strcmp(av[1], "cut") == 0) {
            clock_t t1 = clock();
            if (parse_cut(ac, av, seed) == 84)
                return (84);
            clock_t t2 = clock();
            printf("%s;%f\n", av[3],(double)(t2 - t1 ) / CLOCKS_PER_SEC);
            return 0;
        }

        if (my_strcmp(av[1], "uncut") == 0) {
            clock_t t1 = clock();
            if (parse_uncut(av) == 84)
                return (84);
            clock_t t2 = clock();
            printf("%f\n", (double)(t2 - t1 ) / CLOCKS_PER_SEC);
            return 0;
        }
        return help();
    }
    return help();
}