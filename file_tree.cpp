#include "./file_tree.h"

void WriteTreeToFile(char *file_name, const Tree_t *tree)
{
    char mode[] = "w";
    FILE *pfile = safe_fopen(file_name, mode);

    WriteNodeToFile(pfile, tree);
    fclose(pfile);
}

void WriteNodeToFile(FILE *pfile, const Tree_t *tree)
{
    yaissert(tree);

    fprintf(pfile, "%s", tree->value);
    if(tree->left == NULL && tree->right == NULL)
        return;

    fprintf(pfile, "(");
    if(tree->left != NULL)
        WriteNodeToFile(pfile, tree->left);
    fprintf(pfile, ")");

    fprintf(pfile, "(");
    if(tree->right != NULL)
        WriteNodeToFile(pfile, tree->right);
    fprintf(pfile, ")");
}

Tree_t *ReadTreeFromFile(char *file_name)
{
    yaissert(file_name);

    char mode[] = "rb";
    FILE *pfile = safe_fopen(file_name, mode);

    yaissert(pfile);

    struct stat fileStat = {};
    fstat(fileno(pfile), &fileStat);
    size_t fileSize = fileStat.st_size;
    
    // printf("Size of file = " RED "%lli bytes\n\n\n" RESET, fileSize);
    char *stringText = (char *)safe_calloc((size_t)fileSize + 1, sizeof(char));
    
    size_t nReadBytes = safe_fread((void *) stringText, sizeof(char), fileSize, pfile);
    yaiwarning(nReadBytes == fileSize, "nReadBytes not equals fileSize");
    yaissert(nReadBytes == fileSize);

    char *stringTextCopy = stringText;
    Tree_t *tree = ReadNodeFromFile(pfile, NULL, &stringTextCopy);
    fclose(pfile);

    return tree;
}

Tree_t *ReadNodeFromFile(const FILE *pfile, Tree_t *parent, char *(*stringText))
{
    yaissert(pfile);
    yaissert(stringText);

    while((*stringText)[0] == ')' || (*stringText)[0] == '(')
        (*stringText)++;

    char *text = (char *)safe_calloc(str_max_len, sizeof(char));
    int nscan = 0;
    sscanf((*stringText), "%[^()\r]%n", text, &nscan);

    Tree_t *tree;
    if(nscan > 0)
    {
        tree = CreateTree(text);
        if(parent != NULL)
            tree->parent = parent;
    }
    else
        return NULL;
    if((*stringText)[nscan] == '(')
    {
        (*stringText) += nscan + 1;

        if(*stringText[0] == ')')
            tree->left = NULL;
        else
        {
            Tree_t *left = ReadNodeFromFile(pfile, tree, stringText);
            tree->left = left;
        }

        *stringText += 1;

        if(*stringText[0] == ')')
            tree->right = NULL;
        else
        {
            Tree_t *right = ReadNodeFromFile(pfile, tree, stringText);
            tree->right = right;
        }

        return tree;
    }

    else if((*stringText)[nscan] == ')')
    {
        (*stringText) += nscan + 1;
        return tree;
    }

    else
    {
        printf(_RED "Invalid input\n" _RESET);
        return NULL;
    }
}