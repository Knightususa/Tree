#include "./tree_func.h"

Tree_t *CreateTree(Tree_elem_t value)
{
    Tree_t *tree = (Tree_t *)safe_malloc(sizeof(Tree_t));
    tree->left = NULL;
    tree->right = NULL;
    tree->parent = NULL;
    tree->value = value;
    return tree;
}

Ret_err AddNode(Tree_t *tree, Child_side child_side, Tree_elem_t value)
{
    yaissert(tree);

    Tree_t *child = (Tree_t *)safe_malloc(sizeof(Tree_t));
    switch (child_side)
    {
        case left_child:
            tree->left = child;
            break;
        
        case right_child:
            tree->right = child;
            break;
        
        default:
            printf("Invalid child side\n");
            free(child);
            return Err;
            break;
    }
    child->value = value;
    child->left = NULL;
    child->right = NULL;
    child->parent = tree;
    return noErr;
}

Ret_err DeleteSheet(Tree_t *tree, Child_side child_side)
{
    yaissert(tree);

    switch (child_side)
    {
        case left_child:
        {
            if(tree->left == NULL)
            {
                printf(_RED "Dont have left child\n" _RESET);
                return Err;
            }

            Ret_err err = DeleteNode(tree->left);
            if(err == Err)
                return Err;

            tree->left = NULL;
            break;
        }
            
        case right_child:
        {
            if(tree->right == NULL)
            {
                printf(_RED "Dont have left child\n" _RESET);
                return Err;
            }

            Ret_err err = DeleteNode(tree->right);
            if(err == Err)
                return Err;

            tree->right = NULL;
            break;
        }
        
        default:
            printf("Invalid child side\n");
            return Err;
            break;
    }
    return noErr;
}

Ret_err DeleteNode(Tree_t *node)
{
    yaissert(node);
    if(node->left != NULL || node->right != NULL)
    {
        printf(_RED "Cant delete parent(((\n" _RESET);
        return Err;
    }

    free(node->value);
    free(node);
    return noErr;
}