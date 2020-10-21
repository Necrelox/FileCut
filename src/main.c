/*
** PineApple PROJECT, 2020
** Main
** File description:
** Start
*/

#include "filecut.h"

/* print de comment utilisé le programme*/
int help (void) 
{
    printf ("%s\t\tWelcome on FileCut\n\n%s", VERT, NORMAL);
    printf ("How to use FileCut?\n");
    printf ("\tFor %sCut%s execute : %s./FileCut cut pathtofile [nFiles] [minSize] [maxSize]%son your shell %s( '[]' is optionnal argument )%s\n", CYAN, NORMAL, JAUNE, NORMAL, MAGENTA, NORMAL);
    printf ("\tFor %sUncut%s execute : %s./FileCut uncut [pathofdirectory]%son your shell %s( '[]' is optionnal argument )%s\n", CYAN, NORMAL, JAUNE, NORMAL, MAGENTA, NORMAL);
    return (0);
}

/* Fonction de départ */
int main (int ac, char **av)
{
    /* je donne le pid du programmen en seed pour que cela soit un peu plus aléatoire
    quand j'utiliserai rand()
    les instruction préprocesseurs permettent de définir le nom de la fonction selon le système utilisé  */
    #ifdef WIN32
        int seed = _getpid();
    #else
        int seed = getpid();
    #endif

    /* Permet de verifier le premier argument vaut -help alors il retourne la fonction help() */
    if ((ac < 2 || ac >= 7) || my_strcmp(av[1], "-help") == 0)
        return (help());

    /* si il y'a entre 2 et 7 arguments, et que le premier vaut cut je l'envoie dans 
        la fonction parse_cut()*/
    else if (ac > 2 && ac < 7 && my_strcmp(av[1], "cut") == 0) {
        clock_t t1 = clock();
        srand(seed); // J'initialise l'algo de rand une première fois avec le seed qui vaut le pid
        if (parse_cut(ac, av, seed) == 84) // va vérifier les arguments et découper le fichier si il y'a une erreur il retourne 84
            return (84);
        clock_t t2 = clock();
        printf("%f secondes\n", (double)(t2 - t1 ) / CLOCKS_PER_SEC);
    }
    else if ((ac == 2 || ac == 3) && my_strcmp(av[1], "uncut") == 0) {
        clock_t t1 = clock();
        if (parse_uncut(av) == 84)
            return (84);
        clock_t t2 = clock();
        printf("%f\n", (double)(t2 - t1 ) / CLOCKS_PER_SEC);
    }
    return (ERROR);
}