/*
** PineApple PROJECT, 2020
** cut
** File description:
** Cut Files
*/

#include "filecut.h"

void create_newcase_chainfiles(files_s *chainFiles, long long size)
{
    files_s *newcase = malloc(sizeof(*newcase));

    newcase->size = size;
    for (; chainFiles->next != NULL; chainFiles = chainFiles->next);
    chainFiles->next = newcase;
    newcase->next = NULL;
    newcase->prev = chainFiles;
    newcase->first = chainFiles->first;
}

files_s *prepare_chain_random_files_and_size(filecut_s *origin)
{
    files_s *chainFiles = malloc(sizeof(*chainFiles));
    MTRand r = seedRand(origin->seed);

    long long dif = 0, rest = 0;
    long long temp = 0, temp2 = 0;

    chainFiles->first = chainFiles;
    chainFiles->next = NULL, chainFiles->prev = NULL;

    origin->avgSize = origin->originSize / origin->nbFiles;
    dif = origin->originSize - (origin->avgSize * origin->nbFiles);

    if (origin->avgSize > 1) {

        chainFiles->size = genRandLong(&r) % (origin->avgSize + dif + 1);
        if ((origin->avgSize + dif) > 4) {
            chainFiles->size = ((origin->avgSize + dif) / 4) * 3;
            chainFiles->size += genRandLong(&r) % ((origin->avgSize + dif ) - chainFiles->size + 1);
        }

        if ((origin->avgSize + dif) <= 4) {
            while (chainFiles->size <= 1)
                chainFiles->size = genRandLong(&r) % (origin->avgSize + dif + 1);
        }
        rest = (origin->avgSize + dif) - chainFiles->size;
        temp = chainFiles->size;

        if (origin->nbFiles > 2) {
            for (int x = 1; x < (origin->nbFiles - 1) ; x++) {
                temp2 = genRandLong(&r) % (origin->avgSize + rest + 1);

                if ((origin->avgSize + rest) > 4) {
                    temp2 = ((origin->avgSize + dif) / 4) * 3;
                    temp2 += genRandLong(&r) % ((origin->avgSize + rest) - temp2 +1);
                }
    
                if ((origin->avgSize + rest) <= 4)
                    while (temp2 <= 1)
                        temp2 = genRandLong(&r) % (origin->avgSize + rest + 1);

                rest = (origin->avgSize + rest) - temp2;
                create_newcase_chainfiles(chainFiles, temp2);
                temp += temp2;
            }
        }
        temp2 = (origin->originSize - temp);
        create_newcase_chainfiles(chainFiles, temp2);
        return (chainFiles);
    }
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
    char *buff;
    char *oldPath = my_strdup(origin->path);
    long long blockSize = 0, i = 0;

    MTRand r = seedRand(origin->seed);
    origin->path = prepare_new_path(origin);
    
    for (temp = chainFiles->first; temp != NULL; temp = temp->next, i++) {
        temp->name = malloc(sizeof(char) * (30));
        temp->header = malloc(sizeof(char) * (1000000));
        sprintf(temp->name, "%sbloc_%lld.pine", origin->path, i);;
        sprintf(temp->header, "[%d;%ld;%lld;%lld;%lld]", origin->nbFiles, origin->seed, temp->size, origin->originSize, i);
        temp->fd = fopen(temp->name, "w+");
        fclose(temp->fd);
        temp->fd = fopen(temp->name, "a+");
        fwrite(temp->header, strlen(temp->header), 1, temp->fd);
    }

    for (int choose = 0 ; verif_all_file_complete(chainFiles);) {
        choose = genRandLong(&r) % (origin->nbFiles);
        for (temp = chainFiles->first; temp != NULL && choose > 0; temp = temp->next, choose--);
        if (temp->size > 0) {
            if (temp->size >= (65536 * 2)) {
                blockSize = 65536;
                buff = malloc(sizeof(char) * (blockSize));
                fread (buff, blockSize, 1, fo);
                fwrite(buff, blockSize, 1, temp->fd);
                free (buff);
                temp->size -= blockSize;
            }
            else if (temp->size < (65536 * 2)) {
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
    //remove(oldPath);
    free (oldPath);
}

void cut(filecut_s *origin)
{
    files_s *chainFiles = prepare_chain_random_files_and_size(origin);
    files_s *freeChain = chainFiles->first;

    create_file_and_write(chainFiles, origin);

    for (chainFiles = chainFiles->first;chainFiles != NULL;) {
        freeChain = chainFiles;
        chainFiles = chainFiles->next;
        free (freeChain->header);
        free (freeChain->name);
        free (freeChain);
    }
}