#include "./akinator.h"

ActionsWithTree Parser()
{
    char *text = (char *)safe_calloc(str_max_len, sizeof(char));
    printf(_YELLOW  "Enter <add>    to add question on sheat:\n"
                    "Enter <left>   to go to left child\n"
                    "Enter <right>  to go to right child\n"
                    "Enter <back>   to go to parent\n"
                    "Enter <delete> to delete this sheet(shit)\n\t\t" _RESET);
    scanf("%100s", text);
    printf("\n");

    if(strcmp(text, "add") == 0)
    {
        free(text);
        return add_question;
    }

    if(strcmp(text, "left") == 0)
    {
        free(text);
        return go_left;
    }

    if(strcmp(text, "right") == 0)
    {
        free(text);
        return go_right;
    }

    if(strcmp(text, "back") == 0)
    {
        free(text);
        return go_back;
    }

    if(strcmp(text, "delete") == 0)
    {
        free(text);
        return delete_sheet;
    }

    else
    {
        free(text);
        printf(_RED "Invalid input\n" _RESET);
        ActionsWithTree ret = Parser();
        return ret;
    }
}

char *ParserText(const char *text)
{
    char *input = (char *)safe_calloc(str_max_len, sizeof(char));
    printf(_YELLOW "%s\n" _RESET, text);
    scanf("%100s", input);
    printf("\n");
    return input;
}

Child_side ParserSide(const char *text_to_print)
{
    char *text = (char *)safe_calloc(str_max_len, sizeof(char));
    printf(_YELLOW  "Enter <left>  to %s left child\n"
                    "Enter <right> to %s right child:\n\t\t" _RESET, text_to_print, text_to_print);
    scanf("%100s", text);
    printf("\n");

    if(strcmp(text, "left") == 0)
    {
        free(text);
        return left_child;
    }

    if(strcmp(text, "right") == 0)
    {
        free(text);
        return right_child;
    }

    else
    {
        free(text);
        printf(_RED "Invalid input\n" _RESET);
        Child_side ret = ParserSide(text_to_print);
        return ret;
    }
}

void AddQuestion(Tree_t *tree, Tree_elem_t question, Tree_elem_t answer, Child_side child_side)
{
    yaissert(question);
    yaissert(answer);
    yaissert(tree);

    Tree_elem_t prev_value = tree->value;
    tree->value = question;

    Tree_t *childPrev = CreateTree(prev_value);
    Tree_t *childAnsw = CreateTree(answer);

    yaissert(childPrev);
    yaissert(childAnsw);

    switch(child_side)
    {
        case left_child:
            tree->left = childPrev;
            tree->right = childAnsw;
            break;

        case right_child:
            tree->left = childAnsw;
            tree->right = childPrev;
            break;
        default:
            printf(_RED "Invalid child side\n" _RESET);
            free(childAnsw);
            free(childPrev);
            return;
    }
    childPrev->parent = tree;
    childAnsw->parent = tree;
}

void OpenHtml(const char *file_name)
{
    char command[128];

    int len = snprintf(command, sizeof(command),  "start \"\" \"%s\"", file_name);

    if (len < 0 || (size_t)len >= sizeof(command))
        yaiwarning(0, "Invalid len of command");

    int result = system(command);

    if (result != 0)
        yaiwarning(0, "Html error!\n");
}