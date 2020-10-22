/*
** PineApple PROJECT, 2020
** Parse_Cut
** File description:
** Parser for Cut
*/

#include "filecut.h"

/* Cette fonction au nom beaucoup trop long, permet de verifier les arguments, et si il est possible de les appliquer
    il mettra certain paramètre à une valeur par défaut */
filecut_s *verify_arg_and_set_filecut_settings(int ac, char **av)
{
    filecut_s *origin = malloc(sizeof(*origin)); /* la structure que je vais retourner à parse_cut(ac, av) */
    struct stat info; /* Création d'une structure qui permettra de récupérer les infos du fichier avec la fonction stat(..) */

    /* Je rempli une partie de ma structure origin qui permet d'avoir des information pour permmette le bon fonctionnement de l'algo*/
    origin->error = 0;
    origin->containFile = NULL;
    origin->path = av[2];
    origin->nbFiles = 0;
    origin->sizeMax = 0;
    origin->sizeMin = 0;

    /* je verifie qu'il y'a 3 ou plus d'argument*/
    if (ac >= 3) {
        /* si 4 arguments je verifie si nombre ou pas sinon la variable origin->error est modifié et la structure est retourné */
        if (ac == 4)
            if (verif_if_number(av[3]) == 0) {
                origin->error = ERROR_ARG3;
                return (origin);
            }
        /* si 5 arguments je verifie si nombre ou pas sinon la variable origin->error est modifié et la structure est retourné */
        if (ac == 5) {
            verif_if_number(av[3]) == 0 ? origin->error += ERROR_ARG3 :0;
            verif_if_number(av[4]) == 0 ? origin->error += ERROR_ARG4 :0;
            if (origin->error != 0)
                return (origin);
        }
        /* si 6 arguments je verifie si nombre ou pas sinon la variable origin->error est modifié et la structure est retourné */
        if (ac == 6) {
            verif_if_number(av[3]) == 0 ? origin->error += ERROR_ARG3 :0;
            verif_if_number(av[4]) == 0 ? origin->error += ERROR_ARG4 :0;
            verif_if_number(av[5]) == 0 ? origin->error += ERROR_ARG5 :0;
            if (origin->error != 0)
                return (origin);
        }
        /* je verifie que le chemin soit un fichier ou qu'il existe, et je donne 0 à error si tout est bon*/
        origin->error = verif_path_of_file(av[2]);
        // si il y'a une erreur je retourne directement la structure
        if (origin->error != 0) 
            return (origin);
        /* sinon j'ouvre le fichier et je vais récupéré ses infos, ensuite je vérifie les arguments et si ils sont applicable */
        else { 
            /* les infos du fichier ajouté à la structure origin*/
            stat(av[2], &info);
            origin->originSize = info.st_size;
            // si la taille du fichier est plus petit que 4octets je retourne
            if (origin->originSize < 4) {
                origin->error = ERROR_SMALL_FILES;
                return (origin);
            }
            /* verifier les arguments optionnel si ils sont applicables ou non*/
            if (ac > 3) {
                origin->error = verif_arg(ac, av, origin);
                return (origin);
            }
        }
    }
    return (origin);
}
/* Va vérifier les arguments, gérer les cas d'erreur et puis aller dans "l'algo de découpage" si tout est bon */
int parse_cut(int ac, char **av, int seed)
{
    /* Je crée une structure qui me permettra d'avoir les données necessaires pour le découpage (taille: minimum, maximum .. ;nombre de fichier...)
    grâce à la fonction verify_arg_and_set_filecut_settings(ac, av) */
    filecut_s *origin = verify_arg_and_set_filecut_settings(ac, av);
    if (origin == NULL)
        return (ERROR);
    origin->seed = seed; // seed = pid = graine pour pseudo aléa

    /* Je print les erreurs si il y'en a, je retourne 84 si il y'en a une */
    if (print_all_error(origin, av) != 0) {
        if (origin->containFile != NULL) 
            free (origin->containFile);
        free (origin);
        return (ERROR);
    }
    /* ALGO CUT va cree les blocs leur taille ecrire puis supprimer le fichier d'origine */
    cut(origin);
    /* je libère la mémoire et retourne 0 tout s'est bien déroulé*/
    free (origin->path);
    free (origin);
    return (0);
}