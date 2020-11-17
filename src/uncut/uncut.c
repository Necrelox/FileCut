/*
** EPITECH PROJECT, 2020
** uncut
** File description:
** recreat a file by multiple files
*/

#include "filecut.h"

void free_chain(files_s *chain)
{
    files_s *freechain = chain->first;

    while (chain != NULL) {
        freechain = chain;
        chain = chain->next;
        free(freechain->header);
        free(freechain->name);
        free(freechain);
    }
}

int verif_its_good_file(char *path)
{
    char *extension = ".pine";
    int i = 0, j = 0;
    struct stat try;
    stat (path, &try);

    if (S_ISDIR(try.st_mode) == 1)
        return (1);
    for (; path[i] != '\0'; i++);
    for (; i > 0 && path[i] != '.'; i--);
    for (;path[i] != '\0' && extension[j] != '\0' && path[i] == extension[j]; i++, j++);
    if (path[i] == extension[j])
        return 0;
    return 1;
}

char *recup_header_in_file(char *name)
{
    FILE *fd = fopen(name, "r");
    struct stat info;
    stat(name, &info);
    char *search = "[;;;;]";
    char *buff = malloc(sizeof(char) * 1);
    char *header;
    int i = 1, j = 1;

    fread (buff, 1, 1, fd);
    if (buff[0] == '[') {
        for (; i < info.st_size && search[j] != '\0'; i++) {
            fread (buff, 1, 1, fd);
            if (buff[0] == search[j])
                j++;
        }
        if (search[j] != '\0') {
            free(buff);
            fclose(fd);
            return NULL;
        }
        fclose(fd);
        free (buff);
        header = malloc(sizeof(char) * (i + 1));
        fd = fopen(name, "r");
        fread (header, i, 1, fd);
        header[i] = '\0';
        fclose(fd);
        return header;
    }
    fclose (fd);
    free (buff);
    return NULL;
}

void create_newcase_chain(files_s *chain, char *path, char *header)
{
    files_s *newcase = malloc(sizeof(*newcase));

    newcase->name = my_strdup(path);
    newcase->header = header;
    // for (;chain->next != NULL; chain = chain->next);
    chain->next = newcase;
    newcase->next = NULL;
    newcase->prev = chain;
    newcase->first = chain->first;
}
files_s *prepare_chain_file(char *path)
{
    files_s *chain = malloc(sizeof(*chain));
    DIR *dir;
    struct dirent *readir;
    char name[257];
    char *header;
    int pine = 0;

    chain->first = chain;
    chain->next = NULL, chain->prev = NULL;
    chain->name = malloc(sizeof(char) * 1);
    chain->header = malloc(sizeof(char) * 1);

    dir = opendir(path);
    if (dir == NULL) {
        closedir(dir);
        free_chain(chain);
        return NULL;
    }

    for (int i = 0;(readir = readdir(dir)) != NULL; i++) {
        sprintf(name, "%s%s", path, readir->d_name);
        if (verif_its_good_file(name) == 0) {
            header = recup_header_in_file(name);
            if (header == NULL) {
                free_chain(chain);
                closedir(dir);
                printf ("%s ERROR HEADER ON %s%s\n", ROUGE, name, NORMAL);
                return NULL;
            }
            pine++;
            create_newcase_chain(chain, name, header);
            chain = chain->next;
        }
    }
    if (closedir(dir) == -1) {
        free_chain(chain);
        return NULL;
    }
    if (pine == 0) {
        printf("%sERROR NO FILE .PINE%s\n", ROUGE, NORMAL);
        free_chain(chain);
        return NULL;
    }
    return chain->first;
}

char **header_to_tab(char *header)
{
    char **tab;
    int countChar = 0;
    int count = 0;
    int i = 0, j = 0, x = 0;

    for (int k = 1; header[k] != ']'; k++)
        header[k] == ';' ? countChar++ : 0;
    tab = malloc(sizeof(char *) * (countChar + 2));
    for (i = 0; i < countChar + 1; i++) {
        for (j++, count = 0; header[j] != ';' && header[j] != ']'; j++) {
            if (header[j] >= '0' && header[j] <= '9')
                count++;
        }
        tab[i] = malloc(sizeof(char) * (count + 1));
    }
    for (i = 0, j = 0; i < countChar + 1; i++) {
        for (j++, x = 0, count = 0; header[j] != ';' && header[j] != ']'; j++, x++) {
            if (header[j] >= '0' && header[j] <= '9')
                tab[i][x] = header[j];
        }
        tab[i][x] = '\0';
    }
    tab[i] = NULL;
    return tab;
}

fileuncut_s *get_and_prepare_info(files_s *chain, char *path)
{
    fileuncut_s *origin = malloc(sizeof(*origin));
    files_s *use = chain->first->next;
    char **tab;

    for (; use != NULL; use = use->next) {
        tab = header_to_tab(use->header);
        origin->nbFiles = atoi(tab[0]);
        origin->seed = atol(tab[1]);
        use->size = atoll(tab[2]);
        origin->originSize = atoll(tab[3]);
        use->idBlock = atoi(tab[4]);
        for (int i = 0; tab[i] != NULL; i++)
            free (tab[i]);
        free (tab);
    }
    origin->path = malloc(sizeof(char) * (strlen(path) + 15));
    sprintf(origin->path, "%sOrigin", path);
    return origin;
}

int verif_all_file_is_empty(files_s *chain)
{
    files_s *use = chain->first->next;

    for (; use != NULL && use->size == 0; use = use->next);
    if (use != NULL)
        return 1;
    return 0;
}

void create_original_file(files_s *chain, fileuncut_s *origin)
{
    FILE *fo = fopen(origin->path, "w+");
    files_s *use = chain->first->next;
    int blockSize = 0;
    char *buff;

    MTRand r = seedRand(origin->seed);
    for (use = chain->first->next; use != NULL; use = use->next) {
        use->fd = fopen(use->name, "r");
        fseek(use->fd, strlen(use->header), SEEK_SET);
    }
    for (int choose = 0; verif_all_file_is_empty(chain); choose = 0) {
        choose = genRandLong(&r) % (origin->nbFiles);
        for (use = chain->first->next; use != NULL && use->idBlock != choose; use = use->next);
        if (use->size > 0) {
            if (use->size >= (65536 * 2)) {
                blockSize = 65536;
                buff = malloc(sizeof(char) * blockSize);
                fread (buff, blockSize, 1, use->fd);
                fwrite (buff, blockSize, 1, fo);
                use->size -= blockSize;
                free (buff);
            }
            else if (use->size < (65536 * 2)) {
                (use->size / 2) > 0 ? blockSize = (use->size / 2) :0;
                (use->size / 2) == 0 ? blockSize = 1 :0;
                buff = malloc(sizeof(char) * (blockSize));
                use->size -= blockSize;
                fread (buff, blockSize, 1, use->fd);
                fwrite(buff, blockSize, 1, fo);
                free(buff);
            }
        }
    }
    for (use = chain->first->next; use != NULL; use = use->next) {
        remove (use->name);
        fclose(use->fd);
    }
    fclose (fo);
}

int uncut(char *path)
{
    files_s *chain = prepare_chain_file(path);
    if (chain == NULL)
        return 84;
    fileuncut_s *origin = get_and_prepare_info(chain, path);
    create_original_file(chain, origin);
    free_chain(chain);
    free (origin->path);
    free (origin);
    return 0;
}