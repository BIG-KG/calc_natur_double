#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include ".\headers\tree_types.h"
#include ".\headers\tree_funck.h"
#include ".\headers\tree_data_base_funk.h"
#include ".\headers\tree_const.h"
#include ".\headers\calculator.h"
#include ".\headers\comporator.h"

const int MAX_VALUE_SIZE = 256;

extern char funcs[][20];
FILE* dumpingFile = NULL;

#define NAME_STR(a)  #a      

__attribute__((constructor))
void funckArray_init()
{
    #define GENER(TYPE, str_name)                               \
        strcpy(funcs[TYPE], NAME_STR(TYPE));                    \

    #define ALL_FUNKS
    #include ".\sourse\codegen.cpp"
    #undef ALL_FUNKS
    #undef GENER

    strcpy(funcs[SUB], "-");
    strcpy(funcs[SUM], "+");
    strcpy(funcs[DIV], "/");
    strcpy(funcs[MUL], "*");
    strcpy(funcs[DIF], "d");
    strcpy(funcs[POW], "^");
}


int main()
{
    tree_t testTree = {};
    node_t *test    = NULL;
    dumpingFile     = fopen("tex_file.tex", "w");


    printf("To use your value print YES to load your print NO\n");
    if(yes_no_scan() == 0)
    {

        char value[MAX_VALUE_SIZE] = "";
        scanf("%*[\n]%[^\n]", value);
        printf("start No\n");
        test = getMain(value);
    }
    else
    {
        printf("start YEs\n");
        test = getMain(NULL);
    }

    testTree.treeStart = test;

    printTex(&testTree, dumpingFile);

    dif_calc_f(testTree.treeStart, &testTree, dumpingFile);

    ease_tree(testTree.treeStart);

    printTex(&testTree, dumpingFile);

    generate_html(&testTree);

    makeTex(dumpingFile);

    delete_tree(testTree.treeStart);

    return 0;
}

int yes_no_scan() 
{
    int attempts = 0;
    while(attempts < 10){
        char tmpr[STRING_DATA_SIZE] = {};
        scanf ("%s", tmpr);
        if ( !strcasecmp (tmpr, "yes") ) return 1;
        if ( !strcasecmp (tmpr, "no" ) ) return 0;

        printf("You can only enter \"Yes\" or \"No\".\n");
    }

    return 0;
}
