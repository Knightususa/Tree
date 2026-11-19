#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "./tree_lib/tree_func.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int str_max_len = 100;

void WriteTreeToFile(char *file_name, const Tree_t *tree);
void WriteNodeToFile(FILE *pfile, const Tree_t *tree);

Tree_t *ReadTreeFromFile(char *file_name);
Tree_t *ReadNodeFromFile(Tree_t *parent, char **stringText);

#endif
//end ifndef FILE_TREE_H