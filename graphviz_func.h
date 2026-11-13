#ifndef GRAPHVIZ_FUNC_H
#define GRAPHVIZ_FUNC_H

#include "./tree_lib/tree_func.h"


void MkDotFileTree(char *file_name, const Tree_t *tree_root, const Tree_t *tree_cur);
void WriteGraphSettings(FILE *file);
void WriteTree(FILE *file, const Tree_t *tree_root, const Tree_t *tree_cur);
void WriteNodes(FILE *file, const Tree_t *tree, const Tree_t *tree_cur);
void WriteTreeChain(FILE *file, const Tree_t *tree);

void CompileDotFile(char *inputFile, char *outputFile);
void MkHTMLFileList(char *htmlFileName, char *imageFileName);

#endif
//end ifndef GRAPHVIZ_FUNC_H