#include "./file_tree.h"
#include "./graphviz_func.h"
#include "./akinator.h"

void MakeAllNames(Akinator_union *akinator, char *file_name);
char *CreateName(char *filename, const char *ext);
char *CreateNameToCd(char *filename);



int main()
{
    Akinator_union akinator;
    char *file_name = GetFileName(&akinator);
    MakeAllNames(&akinator, file_name);

    if(akinator.is_new_tree)
    {
        char konst[] = "This is root of new tree";
        Tree_t *tree_root = CreateTree(konst);
        akinator.tree_root = tree_root;
        akinator.tree_cur = tree_root;
    }

    else
    {
        akinator.tree_root = ReadTreeFromFile(akinator.txt.file_name_cd);
        akinator.tree_cur = akinator.tree_root;
    }

    Akinating(&akinator);
}






















void MakeAllNames(Akinator_union *akinator, char *file_name)
{
    yaissert(akinator != NULL);
    yaissert(file_name != NULL);

    akinator->txt.file_name = CreateName(file_name, "txt");
    akinator->txt.file_name_cd = CreateNameToCd(akinator->txt.file_name);
    
    akinator->html.file_name = CreateName(file_name, "html");
    akinator->html.file_name_cd = CreateNameToCd(akinator->html.file_name);

    akinator->dot.file_name = CreateName(file_name, "dot");
    akinator->dot.file_name_cd = CreateNameToCd(akinator->dot.file_name);

    akinator->svg.file_name = CreateName(file_name, "svg");
    akinator->svg.file_name_cd = CreateNameToCd(akinator->svg.file_name);
}

char *CreateName(char *filename, const char *ext)
{
    yaissert(filename != NULL);
    yaissert(ext != NULL);

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