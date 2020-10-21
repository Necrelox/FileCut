/*
** PineApple PROJECT, 2020
** filecut
** File description:
** filecut
*/

#ifndef FILECUT_H_
#define FILECUT_H_

#include "my.h"

#define NORMAL "\033[00m "
#define ROUGE "\033[01;31m"
#define JAUNE "\033[01;33m"
#define VERT "\033[01;32m"
#define CYAN "\033[01;36m"
#define BLEU "\033[01;34m"
#define MAGENTA "\033[01;35m"

/* GENERATE ALEA of 2 to NBFILES_MAX*/
#define NBFILES_MAX 6
/*  The path is false   */
#define ERROR_PATH -1
/* This path is a repository */
#define ERROR_REPO -2
/* Error argument 3 is not a number */
#define ERROR_ARG3 4
/* Error argument 4 is not a number */
#define ERROR_ARG4 5
/* Error argument 5 is not a number */
#define ERROR_ARG5 6
/* Error argument 3 and 4 are not a number */
#define ERROR_ARG3AND4 9
/* Error argument 3 and 5 are not a number */
#define ERROR_ARG3AND5 10
/* Error argument 4 and 5 are not a number */
#define ERROR_ARG4AND5 11
/* Error this files is to small need 4 octet for work */
#define ERROR_SMALL_FILES 20
/* Error minimum too big than file size for create 2 files minimum */
#define ERROR_ARG4_HIGH 25
/* Error more files than size or minimum size */
#define ERROR_MORE_FILES 35
/* Error the sizemax too big than originsize or too small than sizemin */
#define ERROR_ARG5_SIZEMAX 45
/* Program return 84 if he find a error */
#define ERROR 84

typedef struct filecut_t
{
    int seed;
    char *path;
    char *containFile;
    int originSize;
    int nbFiles;
    int sizeMin, avgSize, sizeMax;
    int error;
}filecut_s;

typedef struct fileuncut_t
{
    int seed;
    int nbFiles;
    int originSize;
    char *path;
}fileuncut_s;

typedef struct files_t
{
    FILE *fd;
    char *name;
    int idBlock;
    int size;
    int sizeHeader;
    char *header;
    struct files_t *next, *prev, *first;
}files_s;

/*      SRC/main.c     */
/* Print how to use final cut */
int help (void);
/*Start program*/
int main (int ac, char **av);

/*      SRC/cut/parse_cut.c     */
/* Verif arg and prepare a struct with setting*/
filecut_s *verify_arg_and_set_filecut_settings(int ac, char **av);
/* Parser for Cut Files*/
int parse_cut(int ac, char **av, int seed);

/*      SRC/cut/check_or_print_error.c     */
/* Print all error and return if print*/
int print_all_error(filecut_s *filecut, char **av);
/* Verif arg compared to files */
int verif_arg(int ac, char **av, filecut_s *origin);
/* Verif it's a correct path */
int verif_path_of_file(char *path);
/* Verify it's only a number */
int verif_if_number(char *str);

/*      SRC/cut/cut.c       */
/* create a case and add to chain_list */
void create_newcase_chainfiles(files_s *chainFiles, int size);
/* Prepare a chain_list one structure is equivalent a files*/
files_s *prepare_chain_random_files_and_size(filecut_s *origin);
/* Verif if all files is finish */
int verif_all_file_complete(files_s *chainFiles);
/* Create file with chain list and write */
void create_file_and_write(files_s *chainFiles, filecut_s *origin);
/* Algo Cut Files */
void cut(filecut_s *origin);

/*      SRC/uncut/parse_uncut.c     */
/* Parser for uncut a multiples files */
int parse_uncut(char **av);
/* Verif if it's a repository */
int verif_is_repo(char *path);


/*      SRC/uncut/uncut.c       */
char *my_strdup(char *str);
/* Algo UnCut multiples files */
int uncut(char *path);

#endif /* !FILECUT_H_ */
