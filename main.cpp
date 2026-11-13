#include "./file_tree.h"
#include "./graphviz_func.h"
#include "./akinator.h"

char *CreateName(char *filename, const char *ext);
char *CreateNameToCd(char *filename);

int main()
{
    char input_file_name[]  = "akinator_input";
    char output_file_name[]  = "akinator";

    char *txt_input_file = CreateName(input_file_name, "txt");
    char *txt_input_file_cd = CreateNameToCd(txt_input_file);

    char *txt_output_file = CreateName(output_file_name, "txt");
    char *txt_output_file_cd = CreateNameToCd(txt_output_file);
    
    char *html_file = CreateName(output_file_name, "html");
    char *html_file_cd = CreateNameToCd(html_file);

    char *dot_file = CreateName(output_file_name, "dot");
    char *dot_file_cd = CreateNameToCd(dot_file);

    char *svg_file = CreateName(output_file_name, "svg");
    char *svg_file_cd = CreateNameToCd(svg_file);

    
    Tree_t *tree_root = ReadTreeFromFile(txt_input_file_cd);
    Tree_t *tree_cur = tree_root;
    while(true)
    {
        ActionsWithTree action = Parser();
        switch(action)
        {
            case add_question:
            {
                char *question = ParserText("Enter question");
                char *answer = ParserText("Enter answer");
                Child_side child_side_que = ParserSide("put prev sheet on");
                AddQuestion(tree_cur, question, answer, child_side_que);
                WriteTreeToFile(txt_output_file_cd, tree_cur);
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

        MkDotFileTree(dot_file_cd, tree_cur);
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