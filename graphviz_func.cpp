#include "./graphviz_func.h"

void MkDotFileTree(char *file_name, const Tree_t *tree)
{
    yaissert(file_name != NULL);
    //TODO: CheckList

    char open_mode[] = "w";
    FILE *file = safe_fopen(file_name, open_mode);

    fprintf(file,   "digraph G {\n"
                    "splines = curved\n");

    WriteGraphSettings(file);
    WriteTree(file, tree);

    fprintf(file, "}\n");

    fclose(file);
}

void WriteGraphSettings(FILE *file)
{
    fprintf(file,
        "\tgraph ["
        "bgcolor = \"#5B8298\""
        "]\n"

        "\tnode ["
        "shape = record, "
        "fontname = \"Consolas\", "
        "style = \"rounded, filled\", "
        "height = 0.5, "
        "width = 1, "
        "color = \"#86BBD8\", "
        "fontcolor = \"#E8F1F5\", "
        "penwidth = 3"
        "]\n"
    
        "\tedge ["
        "color = \"#2F4858\", "
        "fontcolor = \"#2F4858\", "
        "penwidth = 2, "
        "arrowsize = 0.6"
        "];\n\n");


}

void WriteTree(FILE *file, const Tree_t *tree)
{
    fprintf(file, "\n\tnode [fillcolor = \"#78a5728b\"]\n");
    WriteNodes(file, tree);
    fprintf(file, "\n");
    
    WriteTreeChain(file, tree);
}

void WriteNodes(FILE *file, const Tree_t *tree)
{
    if(tree == NULL)
        return;
    char *strNode  = tree->value;

    if(strNode != NULL)
        fprintf(file, "\t%lli [label = %s]\n", (long long int)tree, strNode);
    WriteNodes(file, tree->left);
    WriteNodes(file, tree->right);
}

void WriteTreeChain(FILE *file, const Tree_t *tree)
{
    if(tree == NULL || (tree->left == NULL && tree->right == NULL))
        return;

    if(tree->left != NULL)
        fprintf(file, "\t%lli -> %lli [color = \"#e6a422\"];\n", (long long int)tree, (long long int)tree->left);

    if(tree->right != NULL)
        fprintf(file, "\t%lli -> %lli [color = \"#e6a422\"];\n", (long long int)tree, (long long int)tree->right);

    WriteTreeChain(file, tree->left);
    WriteTreeChain(file, tree->right);
}

void CompileDotFile(char *inputFile, char *outputFile)
{
    char command[512];

    int len = snprintf(command, sizeof(command),
        ".\\Graphviz\\bin\\dot.exe "
        "-Tsvg %s "
        "-o %s",
        inputFile, outputFile);

    if (len < 0 || (size_t)len >= sizeof(command))
        yaiwarning(0, "Invalid len of command");

    int result = system(command);

    if (result != 0)
        yaiwarning(0, "Graphviz error!\n");

    printf("Graph generated successfully!\n");
}

void MkHTMLFileList(char *htmlFileName, char *imageFileName)
{
    yaissert(htmlFileName != NULL);
    yaissert(imageFileName != NULL);

    char open_mode[] = "w";
    FILE *file = safe_fopen(htmlFileName, open_mode);

    fprintf(file,
        "<!DOCTYPE html>\n"
        "<html lang=\"ru\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Tree_t visualization</title>\n"
        "    <style>\n"
        "        body {\n"
        "            background-color: #5B8298;\n"
        "            display: flex;\n"
        "            justify-content: center;\n"
        "            align-items: center;\n"
        "            min-height: 100vh;\n"
        "            margin: 0;\n"
        "        }\n"
        "        img {\n"
        "            max-width: 95%%;\n"
        "            max-height: 95vh;\n"
        "            object-fit: contain;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <img src=\"%s\" alt=\"Tree_t visualization\">\n"
        "</body>\n"
        "</html>\n",
        imageFileName
    );

    fclose(file);
}