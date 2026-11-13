#include "./file_tree.h"
#include "./graphviz_func.h"
#include "./akinator.h"

char *CreateName(char *filename, const char *ext);
char *CreateNameToCd(char *filename);

int main()
{
    char file_name[]  = "akinator";

    char *txt_file = CreateName(file_name, "txt");
    char *txt_file_cd = CreateNameToCd(txt_file);
    
    char *html_file = CreateName(file_name, "html");
    char *html_file_cd = CreateNameToCd(html_file);

    char *dot_file = CreateName(file_name, "dot");
    char *dot_file_cd = CreateNameToCd(dot_file);

    char *svg_file = CreateName(file_name, "svg");
    char *svg_file_cd = CreateNameToCd(svg_file);

    
    Tree_t *tree_root = ReadTreeFromFile(txt_file_cd);
    Tree_t *tree_cur = tree_root;
    while(true)
    {
        ActionsWithTree action = Parser();
        switch(action)
        {
            case add_question:
            {
                if(tree_cur->left != NULL || tree_cur->right != NULL)
                {
                    printf(_RED "Cannot add to node with sheet(-s)\n" _RESET);
                    break;
                }
                char *answer = ParserText("Enter person");
                char *question = ParserText("Enter his characteristic");
                AddQuestion(tree_cur, question, answer);
                WriteTreeToFile(txt_file_cd, tree_root);
                break;
            }
            
            case edit:
            {
                char *question = ParserText("Enter new value");
                free(tree_cur->value);
                tree_cur->value = question;
                WriteTreeToFile(txt_file_cd, tree_root);
                break;
            }

            case go_left:
                if(tree_cur->left == NULL)
                    printf(_RED "Left child dead)))\n" _RESET);
                else
                    tree_cur = tree_cur->left;
                break;

            case go_right:
                if(tree_cur->right == NULL)
                    printf(_RED "Right child dead)))\n" _RESET);
                else
                    tree_cur = tree_cur->right;
                break;

            case go_back:
                if(tree_cur->parent == NULL)
                    printf(_RED "Parent dead)))\n" _RESET);
                else
                    tree_cur = tree_cur->parent;
                break;

            case go_start:
                tree_cur = tree_root;
                break;

            case delete_sheet:
            {
                Child_side child_side_del = ParserSide("delete");
                DeleteSheet(tree_cur, child_side_del);
                break;
            }

            default:
                printf(_RED "Invalid command\n" _RESET);
                break;
        }

        MkDotFileTree(dot_file_cd, tree_root, tree_cur);
        CompileDotFile(dot_file_cd, svg_file_cd);
        MkHTMLFileList(html_file_cd, svg_file);
        OpenHtml(html_file_cd);
    }
}






char *CreateName(char *filename, const char *ext)
{
    yaissert(filename != NULL);

    char *nameOut = (char *)safe_calloc(strlen(filename) + strlen(ext) + 1, sizeof(char));
    strcat(nameOut, filename);
    strcat(nameOut, ".");
    strcat(nameOut, ext);
    return nameOut;
}

char *CreateNameToCd(char *filename)
{
    yaissert(filename != NULL);

    char *nameOut = (char *)safe_calloc(strlen(filename) + strlen("./out_files/.") + 1, sizeof(char));
    strcpy(nameOut, "./out_files/");
    strcat(nameOut, filename);
    return nameOut;
}