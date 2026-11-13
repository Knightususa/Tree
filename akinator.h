#ifndef AKINATOR_H
#define AKINATOR_H

#include "./file_tree.h"
#include "./graphviz_func.h"

enum ActionsWithTree
{
    add_question,
    go_left,
    go_right,
    go_back,
    delete_sheet
};

void AddQuestion(Tree_t *tree, Tree_elem_t question, Tree_elem_t answer, Child_side child_side);

ActionsWithTree Parser();
char *ParserText(const char *text);
Child_side ParserSide(const char *text);

void OpenHtml(const char *file_name);


#endif
//end AKINATOR_H
