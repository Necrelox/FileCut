/*
** PineApple PROJECT, 2020
** cut
** File description:
** Cut Files
*/

#include "filecut.h"

void create_newcase_chainfiles(files_s *chainFiles, int size)
{
    /* cree une nouvelle case */
    files_s *newcase = malloc(sizeof(*newcase));

    newcase->size = size;
    /* va au bout de la liste et y rajoute la nouvelle case*/
    for (; chainFiles->next != NULL; chainFiles = chainFiles->next);
    chainFiles->next = newcase;
    newcase->next = NULL;
    newcase->prev = chainFiles;
    newcase->first = chainFiles->first;
}

/* Create a chain list of files with random size */
files_s *prepare_chain_random_files_and_size(filecut_s *origin)
{
    /*creation de la premiere structure de la chaine
    ainsi que de multiple variable pour des calcul en float et aussi en int */
    files_s *chainFiles = malloc(sizeof(*chainFiles));
    float OriginSize = origin->originSize;
    float nbFiles;
    float avg1 = 0;
    int dif = 0, rest = 0;
    int temp = 0, temp2 = 0;

    /* j'initialise les pointeurs */
    chainFiles->first = chainFiles;
    chainFiles->next = NULL, chainFiles->prev = NULL;

    /* je vérifie le nombre de fichiers si il est inférieur ou égale à 1 je lui donne une valeur
     pour le moment une const de 5*/
    if (origin->nbFiles <= 1)
        origin->nbFiles = 5;

    /* je calcul la moyenne aisi que la différence selon OriginSize / nbFiles*/
    nbFiles = origin->nbFiles;
    avg1 = (OriginSize / nbFiles);
    origin->avgSize = avg1;
    avg1 -= origin->avgSize;
    avg1 *= nbFiles;
    dif = avg1;
    avg1 -= dif;
    avg1 >= 0.5 ? dif++ :0; 

    /* si la moyenne est supérieur à 1 alors* je vais donner une taille aléatoire
    pour le premiere bloc, qui sera stocké dans la première structure de la chaîne */
    if (origin->avgSize > 1) {
        chainFiles->size = rand() % (origin->avgSize + dif + 1);
        if ((origin->avgSize + dif) > 4)
            while (chainFiles->size <= (((origin->avgSize + dif) / 4) * 3))
                chainFiles->size = rand() % (origin->avgSize + dif + 1);
        if ((origin->avgSize + dif) <= 4)
            while (chainFiles->size <= 1)
                chainFiles->size = rand() % (origin->avgSize + dif + 1);
        rest = (origin->avgSize + dif) - chainFiles->size;
        temp = chainFiles->size;

        /* je vais cree une nouvelle structure et la lié à ma chaine
        je donne à chaque structure une taille aléatoire selon la taille moyenne*/
        for (int x = 1; x < (origin->nbFiles - 1) ; x++) {
            temp2 = rand() % (origin->avgSize + rest + 1);
            if ((origin->avgSize + rest) > 4)
                while (temp2 <= (((origin->avgSize + rest) / 4) * 3))
                    temp2 = rand() % (origin->avgSize + rest + 1);
            if ((origin->avgSize + rest) <= 4)
                while (temp2 <= 1)
                    temp2 = rand() % (origin->avgSize + rest + 1);
            rest = (origin->avgSize + rest) - temp2;
            create_newcase_chainfiles(chainFiles, temp2);
            temp += temp2;
        }
        temp2 = (origin->originSize - temp);
        create_newcase_chainfiles(chainFiles, temp2);
        return (chainFiles);
    }
    /*si la moyenne valait 1 ou moins la structure du début de chaine est libéré et je retourne NULL*/
    free (chainFiles);
    return (NULL);
}

int verif_all_file_complete(files_s *chainFiles)
{
    files_s *temp = chainFiles->first;

    for (;temp != NULL && temp->size == 0; temp = temp->next);
    if (temp != NULL)
        return (1);
    return (0);
}

char *prepare_new_path(filecut_s *origin)
{
    char *newpath;
    char *samePath = "./";
    int i = 0, j = 0, count = 0;

    for (;origin->path[i] != '\0'; i++)
        if (origin->path[i] == '/')
            count++;
    if (count > 0) {
        for (i = 0; origin->path[i] != '\0' && count > 0; i++) {
            if (origin->path[i] == '/')
                count--;
        }
        newpath = malloc (sizeof(char) * (i + 1));
        for (; j < i; j++)
            newpath[j] = origin->path[j];
        newpath[j] = '\0';
        return (newpath);
    }
    else if (count == 0) {
        newpath = malloc(sizeof(char) * (3));
        for (i = 0; samePath[i] != '\0'; i++)
            newpath[i] = samePath[i];
        newpath[i] = '\0';
        return (newpath);
    }
    return (NULL);
}

void create_file_and_write(files_s *chainFiles, filecut_s *origin)
{
    FILE *fo = fopen(origin->path, "r");
    files_s *temp = chainFiles->first;
    int blockSize = 0, i = 0;
    char *buff;
    char *oldPath = my_strdup(origin->path);

    srand(origin->seed);
    origin->path = prepare_new_path(origin);
    
    for (temp = chainFiles->first; temp != NULL; temp = temp->next, i++) {
        temp->name = malloc(sizeof(char) * (30));
        temp->header = malloc(sizeof(char) * (1000000));
        sprintf(temp->name, "%sbloc_%d.pine", origin->path, i);;
        sprintf(temp->header, "[%d;%d;%d;%d;%d]", origin->nbFiles, origin->seed, temp->size, origin->originSize, i);
        temp->fd = fopen(temp->name, "w+");
        fclose(temp->fd);
        temp->fd = fopen(temp->name, "a+");
        fwrite(temp->header, strlen(temp->header), 1, temp->fd);
    }

    for (int choose = 0 ; verif_all_file_complete(chainFiles);) {
        choose = rand() % (origin->nbFiles);
        for (temp = chainFiles->first; temp != NULL && choose > 0; temp = temp->next, choose--);
        if (temp->size > 0) {
            if (temp->size > 1000) {
                for (int x = 100; (x * 10) < temp->size && x < 10000000; x += 100, blockSize = x);
                buff = malloc(sizeof(char) * (blockSize));
                temp->size -= blockSize;
                fread (buff, blockSize, 1, fo);
                fwrite(buff, blockSize, 1, temp->fd);
                free (buff);
            }
            else if (temp->size <= 1000) {
                (temp->size / 2) > 0 ? blockSize = (temp->size / 2) :0;
                (temp->size / 2) == 0 ? blockSize = 1 :0;
                buff = malloc(sizeof(char) * (blockSize));
                temp->size -= blockSize;
                fread (buff, blockSize, 1, fo);
                fwrite(buff, blockSize, 1, temp->fd);
                free(buff);
            }
        }
    }
    for (temp = chainFiles->first; temp != NULL; temp = temp->next) {
        fclose (temp->fd);
    }
    fclose (fo);
    remove(oldPath);
    free (oldPath);
}

void cut(filecut_s *origin)
{
    /*je crée ma chaine de fichiers avec la fonction prepare_chain_random_files_and_size(origin);*/
    files_s *chainFiles = prepare_chain_random_files_and_size(origin);
    /* je cree une autre chaine qui va me permettre */
    files_s *freeChain = chainFiles->first, *temp = chainFiles->first;
    int cal = 0;
    for (int i = 0;temp != NULL; temp = temp->next, i++) {
        //printf ("Fichier (%d), size : %d\n", i, temp->size);
        cal += temp->size;
    }
    create_file_and_write(chainFiles, origin);
    for (chainFiles = chainFiles->first;chainFiles != NULL;) {
        freeChain = chainFiles;
        chainFiles = chainFiles->next;
        free (freeChain->header);
        free (freeChain->name);
        free (freeChain);
    }
}