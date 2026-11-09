#include "./tree_lib/tree_func.h"
#include "./graphviz_func.h"

int main()
{
    char str1[] = "1";
    char str2[] = "2";
    char str3[] = "3";
    Tree_t *tree = CreateTree(str1);
    AddNode(tree, left_child, str2);
    AddNode(tree, right_child, str3);
    AddNode(tree->right, right_child, str3);
    AddNode(tree->right->right, right_child, str3);
    AddNode(tree->right, left_child, str3);

    char input_file[]  = "pic1.dot";
    char output_file[] = "pic1.svg";
    char html_file[]   = "pic1.html";
    MkDotFileTree(input_file, tree);
    CompileDotFile(input_file, output_file);
    MkHTMLFileList(html_file, output_file);
}