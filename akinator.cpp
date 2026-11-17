#include "./akinator.h"

void Akinating(Akinator_union *akinator)
{
    while(true)
    {
        ActionsWithTree action = ParserForAction();
        switch(action)
        {
            case action_guess:
                akinator->tree_cur = akinator->tree_root;
                Guessing(akinator);
                break;

            case action_vizualize:
                MkDotFileTree(akinator->dot.file_name_cd, akinator->tree_root, akinator->tree_cur);
                CompileDotFile(akinator->dot.file_name_cd, akinator->svg.file_name_cd);
                MkHTMLFileList(akinator->html.file_name_cd, akinator->svg.file_name);
                OpenHtml(akinator->html.file_name_cd);
                break;

            case action_edit:
            {
                char *question = ParserText("Enter new value");
                free(akinator->tree_cur->value);
                akinator->tree_cur->value = question;
                WriteTreeToFile(akinator->txt.file_name_cd, akinator->tree_root);
                break;
            }

            case action_delete:
            {
                Child_side child_side_del = ParserSide("delete");
                DeleteSheet(akinator->tree_cur, child_side_del);
                break;
            }

            default:
                printf(_RED "Invalid command\n" _RESET);
                break;
        }
    }
}





ActionsWithTree ParserForAction()
{
    char text[str_max_len] = "";
    printf(_YELLOW  "Enter <g> to start guessing\n"
                    "Enter <v> to vizualize tree\n"
                    "Enter <e> to edit value of this node\n"
                    "Enter <d> to delete this sheet(shit)\n" _RESET);
    scanf("%100s", text);
    printf("\n");

    if(strcmp(text, "g") == 0)
        return action_guess;

    if(strcmp(text, "v") == 0)
        return action_vizualize;

    if(strcmp(text, "e") == 0)
        return action_edit;

    if(strcmp(text, "d") == 0)
        return action_delete;

    else
    {
        printf(_RED "Invalid input\n" _RESET);
        ActionsWithTree ret = ParserForAction();
        return ret;
    }
}

char *ParserText(const char *text)
{
    char *input = (char *)safe_calloc(str_max_len, sizeof(char));
    PrintAndVoice(text);
    getchar();
    scanf("%100[^\n]", input);
    printf("\n");
    return input;
}

Child_side ParserSide(const char *text_to_print)
{
    char text[str_max_len] = "";
    printf(_YELLOW  "Enter <left>  to %s left child\n"
                    "Enter <right> to %s right child:\n\t\t" _RESET, text_to_print, text_to_print);
    scanf("%100s", text);
    printf("\n");

    if(strcmp(text, "left") == 0)
        return left_child;

    if(strcmp(text, "right") == 0)
        return right_child;

    else
    {
        printf(_RED "Invalid input\n" _RESET);
        Child_side ret = ParserSide(text_to_print);
        return ret;
    }
}

void AddQuestion(Tree_t *tree, Tree_elem_t question, Tree_elem_t answer)
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

    tree->left = childAnsw;
    tree->right = childPrev;

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

void Guessing(Akinator_union *akinator)
{
    while(true)
    {
        if(akinator->tree_cur->left == NULL && akinator->tree_cur->right == NULL)
        {
            PrintAndVoice("It is");
            Child_side child_side = ParserSideYN(akinator);
            printf("\n");
            switch(child_side)
            {
                case left_child:
                    PrintAndVoice("YOUHUUUU I right!!!\n");
                    return;

                case right_child:
                {
                    PrintAndVoice("Add your character\n");
                    char *answer = ParserText("Enter person");
                    char *question = ParserText("Enter his characteristic");
                    AddQuestion(akinator->tree_cur, question, answer);
                    WriteTreeToFile(akinator->txt.file_name_cd, akinator->tree_root);
                    return;
                }
                
                default:
                    printf("Invalid side in guessing\n");
                    return;
            }
        }

        Child_side child_side = ParserSideYN(akinator);
        printf("\n");
        switch(child_side)
        {
            case left_child:
                if(akinator->tree_cur->left == NULL)
                    printf(_RED "Left child dead)))\n" _RESET);
                else
                    akinator->tree_cur = akinator->tree_cur->left;
                break;

            case right_child:
                if(akinator->tree_cur->right == NULL)
                    printf(_RED "Right child dead)))\n" _RESET);
                else
                    akinator->tree_cur = akinator->tree_cur->right;
                break;

            default:
                printf("Invalid side in akinatoring\n");
                break;
        }
    }
}

Child_side ParserSideYN(Akinator_union *akinator)
{
    PrintAndVoice(akinator->tree_cur->value);
    char text[str_max_len] = "";
    printf(_YELLOW  "Enter [ y / n / e / d / r] (yes / no / edit value / delete sheet / repeat question): " _RESET);
    scanf("%100s", text);
    printf("\n");

    if(strcmp(text, "y") == 0)
        return left_child;

    if(strcmp(text, "n") == 0)
        return right_child;

    if(strcmp(text, "e") == 0)
    {
        EditValue(akinator);
        Child_side ret = ParserSideYN(akinator);
        return ret;
    }

    if(strcmp(text, "d") == 0)
    {
        Child_side child_side_del = ParserSide("delete");
        DeleteSheet(akinator->tree_cur, child_side_del);
        Child_side ret = ParserSideYN(akinator);
        return ret;
    }

    if(strcmp(text, "r") == 0)
    {
        Child_side ret = ParserSideYN(akinator);
        return ret;
    }

    else
    {
        printf(_RED "\nInvalid input\n" _RESET);
        Child_side ret = ParserSideYN(akinator);
        return ret;
    }
}

void EditValue(Akinator_union *akinator)
{
    char *question = ParserText("Enter new value");
    free(akinator->tree_cur->value);
    akinator->tree_cur->value = question;
    WriteTreeToFile(akinator->txt.file_name_cd, akinator->tree_root);
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
            char *file_name = ChooseTree();

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

char *ChooseTree()
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