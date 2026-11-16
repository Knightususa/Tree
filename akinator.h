#ifndef AKINATOR_H
#define AKINATOR_H

#include "./file_tree.h"
#include "./graphviz_func.h"

#define PrintAndVoice(str)\
{\
    printf(_YELLOW "%s\n" _RESET, str);\
    char command[512];\
    snprintf(command, sizeof(command), \
    "espeak_lib\\espeak-ng.exe -v en -s 20 \"%s\"", str);\
    system(command);\
}

enum ActionsWithTree
{
    action_edit,
    action_guess,
    action_delete,
    action_vizualize
};

typedef struct
{
    char *file_name;
    char *file_name_cd;
}File_names;

typedef struct
{
    File_names txt;
    File_names html;
    File_names dot;
    File_names svg;
    Tree_t *tree_root;
    Tree_t *tree_cur;
    bool is_new_tree;
}Akinator_union;

void Akinating(Akinator_union *akinator);

ActionsWithTree ParserForAction();
char *ParserText(const char *text);
Child_side ParserSide(const char *text_to_print);
Child_side ParserSideYN(Akinator_union *akinator);

void AddQuestion(Tree_t *tree, Tree_elem_t question, Tree_elem_t answer);
void Guessing(Akinator_union *akinator);
void EditValue(Akinator_union *akinator);
void OpenHtml(const char *file_name);


#endif
//end AKINATOR_H
