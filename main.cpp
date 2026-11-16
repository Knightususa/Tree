#include "./file_tree.h"
#include "./graphviz_func.h"
#include "./akinator.h"

#include <windows.h>

char *GetFileName(Akinator_union *akinator);
char *OpenTree();
char **ListFiles(const char *ext);
char *InputFileName();

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




















char *GetFileName(Akinator_union *akinator)
{
    akinator->is_new_tree = false;
    while(true)
    {
        char text[str_max_len] = "";
        printf(_YELLOW "Enter [o/m] (open tree / make new tree)\n" _RESET);
        scanf("%100s", text);
        printf("\n");

        if(strcmp(text, "o") == 0)
        {
            char *file_name = OpenTree();

            if(file_name == NULL)
                continue;

            char *ext = strstr(file_name, ".txt");
            *ext = '\0';
            return file_name;
        }

        if(strcmp(text, "m") == 0)
        {
            char *file_name = InputFileName();
            akinator->is_new_tree = true;
            return file_name;
        }

        else
            printf(_RED "Invalid input\n" _RESET);
    }
}

char *InputFileName()
{
    char *file_name = (char *)safe_calloc(str_max_len, sizeof(char));
    while(true)
    {
        printf("Enter tree name\n");
        int inputN = scanf("%100s", file_name);
        if(inputN == 1)
            break;
        printf(_RED "Invalid input\n" _RESET);
    }
    char *ext = strstr(file_name, ".txt");
    if(ext != NULL)
        *ext = '\0';
    return file_name;
}

char *OpenTree()
{
    printf("\t\tAll trees:\n");
    char **list_names = ListFiles("txt");
    if(list_names[0] == NULL)
    {
        printf("No files in folder");    
        return NULL;
    }

    else
    {
        while(true)
        {
            printf("Enter number of filename\n");
            int number_filename = 0;
            int inputN = scanf("%i", &number_filename);

            if(inputN < 1)
            {
                printf(_RED "Invalid input\n" _RESET);
                continue;
            }

            if(number_filename < 0 || list_names[number_filename] == NULL)
            {
                printf(_RED "Invalid number of input\n" _RESET);
                continue;
            }

            char *ret = strdup(list_names[number_filename]);
            int i = 0;
            while(list_names[i] != NULL)
                free(list_names[i++]);
            free(list_names);
            return ret;
        }
    }
}

char **ListFiles(const char *ext)
{
    yaissert(ext != NULL);

    const char dir[] = "./out_files/";
    char pattern[MAX_PATH];
    snprintf(pattern, sizeof(pattern), "%s\\*.%s", dir, ext);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE)
    {
        printf("No files found or error: %lu\n", GetLastError());
        return NULL;
    }

    int i = 0;
    int Nmax = 100;
    char **list_names = (char **)safe_calloc(Nmax, sizeof(char *));
    do
    {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            list_names[i] = strdup(fd.cFileName);
            printf("%i)\t%s\n", i++, fd.cFileName);
        }

        if(i >= Nmax)
        {
            Nmax *= 2;
            list_names = (char **)realloc(list_names, Nmax);
        }
    }while (FindNextFileA(h, &fd));
    FindClose(h);
    printf("\n");
    return list_names;
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