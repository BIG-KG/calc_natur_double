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

extern char funcs[][20];

#define NAME_STR(a)  #a                 

int main()
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

    tree_t testTree = {};
    node_t *test = getMain();
    testTree.treeStart = test;

    dif_calc_f(testTree.treeStart, &testTree);


    //generate_html(&testTree);

   // ease_tree(testTree.treeStart);

    printf("end1_____________\n");
    generate_html(&testTree);

    FILE *texoutput_tex = fopen("tex_file.tex", "w");

    ease_tree(testTree.treeStart);

    printTex(&testTree, texoutput_tex);
    printTex(&testTree, texoutput_tex);
    printTex(&testTree, texoutput_tex);
    makeTex(texoutput_tex);

    fclose(texoutput_tex);

    printf("end2________\n");

    return 0;

}
