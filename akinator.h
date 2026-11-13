#ifndef AKINATOR_H
#define AKINATOR_H

#include "./file_tree.h"
#include "./graphviz_func.h"

enum ActionsWithTree
{
    add_question,
    edit,
    go_left,
    go_right,
    go_back,
    go_start,
    delete_sheet
};

void AddQuestion(Tree_t *tree, Tree_elem_t question, Tree_elem_t answer);

ActionsWithTree Parser();
char *ParserText(const char *text);
Child_side ParserSide(const char *text);

void OpenHtml(const char *file_name);


#endif
//end AKINATOR_H
