/*
** PineApple PROJECT, 2019
** Verif_error
** File description:
** Check or Printf a multiples errors
*/

#include "filecut.h"

/* je print les erreurs et retourne 1 si il y'a une erreur*/
int print_all_error(filecut_s *filecut, char **av)
{
    int bool = 0;
    switch (filecut->error)
    {
    case ERROR_MORE_FILES :
        printf ("%sERROR %s%s%s MORE FILES THAN SIZE OR MINIMUM SIZE%s\n", ROUGE, VERT, av[3], ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG3 :
        printf ("%sERROR ARGUMENT %s%s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[3],ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG4 :
        printf ("%sERROR ARGUMENT %s%s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[4],ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG4_HIGH :
        printf ("%sERROR %s%s%s THIS MINIMUM SIZE IS TOO BIG THAN SIZE FILE FOR CREATE 2 FILES MINIMUM%s\n", ROUGE, VERT, av[4], ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG5 :
        printf ("%sERROR ARGUMENT %s%s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[5],ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG3AND4:
        printf ("%sERROR ARGUMENT %s%s %sand%s %s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[3],ROUGE, VERT, av[4], ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG3AND5:
        printf ("%sERROR ARGUMENT %s%s %sand%s %s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[3],ROUGE, VERT, av[5], ROUGE, NORMAL);
        bool++;
        break;
    case ERROR_ARG4AND5:
        printf ("%sERROR ARGUMENT %s%s %sand%s %s%s IS NOT A NUMBER%s\n", ROUGE, VERT, av[4],ROUGE, VERT, av[5], ROUGE, NORMAL);
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
    case ERROR_ARG5_SIZEMAX :
        bool++;
        break;
    default:
        bool = 0;
        break;
    }
    return (bool);
}

/*je verifie si les arguments sont compatible avec le fichier ou les autres arguments */
int verif_arg(int ac, char **av, filecut_s *origin)
{
    unsigned int temp = 0; /*je cree une varible pour des calculs*/

    /* si il y'a 4 ou plus d'arguments je les vérifies*/
    if (ac >= 4) {

        /* si il y'a un 5ème argument je le vérifie*/
        if (ac >= 5) {
            /* je récupère l'argument pour la taille minimum */
            origin->sizeMin = atoi(av[4]);
            /* Si SizeMin est inférieur ou égale à 1 alors je remet par défaut à 0*/
            if (origin->sizeMin <= 1)
                origin->sizeMin = 0;
            /*je verifie si il est possible de creer au minimum 
                deux fichier de ce minimum en le multipliant par deux et en le
                comparant avec la taille du fichier sinon je retourne une erreur*/
            temp = origin->sizeMin;
            temp *= 2;
            if (temp > origin->originSize)
                return (ERROR_ARG4_HIGH);
        }

        /*je récupère l'argument 4 pour le nombre de fichiers */
        origin->nbFiles = atoi(av[3]);
        /* Si il est inférieur ou égale à 1 alors je le remet par défaut à 0
        et alors le nombre de fichiers sera généré dans cut*/
        if (origin->nbFiles <= 1)
            origin->nbFiles = 0;
    
        /* je vérifie le nombre de fichiers, si il est supérieur à
            la taille d'origine divisé par la taille minimum alors
            j'ai trop de fichier par rapport à la taille d'origin et la taille minimum */
        if (origin->sizeMin == 0) {
            temp = 1;
            origin->sizeMin = 2;
        }
        else
            temp = 0;
        if (origin->nbFiles > (origin->originSize / origin->sizeMin))
            return (ERROR_MORE_FILES);
        if (temp == 1)
            origin->sizeMin = 0;

        /* si il y'a 6ème argument je le vérifie*/
        if (ac >= 6) {
            /* je récupère l'argument pour la taille maximum des blocs */
            origin->sizeMax = atoi(av[5]);
            /* je verifie si il est pas plus grand que la taille du fichier d'origine sinon je retourne l'erreur*/
            if (origin->sizeMax > origin->originSize) {
                printf ("%sSIZEMAX %lld IS TOO BIG THAN ORIGINSIZE %lld %s\n", ROUGE, origin->sizeMax, origin->originSize, NORMAL);
                return (ERROR_ARG5_SIZEMAX);
            }
            /* Je vérifie qu'il soit pas plus petit que sizemin sinon je retourne l'erreur */
            if (origin->sizeMax < origin->sizeMin) {
                printf ("%sSIZEMAX %lld IS TOO SMALL THAN SIZEMIN %lld %s\n", ROUGE, origin->sizeMax, origin->originSize, NORMAL);
                return (ERROR_ARG5_SIZEMAX);
            }
        }
    }
    return (0);
}

/* je vérifie si le chemin est bien un fichier et qu'il existe*/
int verif_path_of_file(char *path)
{
    /* je vérifie si le chemin existe */
    struct stat buff;
    if (stat(path, &buff) == -1)
        return (ERROR_PATH);
    /* je vérifie si il sagit bien d'un fichier*/
    if (S_ISDIR(buff.st_mode) == 1)
        return (ERROR_REPO);
    return (0);
}

/* je vérifie si l'argument est bien un nombre */
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
