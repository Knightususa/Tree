#ifndef TREE_FUNC_H
#define TREE_FUNC_H

#include "./common.h"

enum Ret_err
{
    noErr,
    Err
};

typedef char* Tree_elem_t;

struct Tree_t
{
    Tree_elem_t value;
    Tree_t *left;
    Tree_t *right;
    Tree_t *parent;
};

enum Child_side
{
    left_child = 0,
    right_child = 1
};


Tree_t *CreateTree(Tree_elem_t value);
Ret_err AddNode(Tree_t *tree, Child_side child_side, Tree_elem_t value);
Ret_err DeleteSheet(Tree_t *tree, Child_side child_side);
Ret_err DeleteNode(Tree_t *node);

#endif

//end ifndef TREE_FUNC_H