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
    strcpy(funcs[SIN], NAME_STR(SIN));
    strcpy(funcs[COS], NAME_STR(COS));
    strcpy(funcs[SUB], "-");
    strcpy(funcs[SUM], "+");
    strcpy(funcs[DIV], "/");
    strcpy(funcs[MUL], "*");
    strcpy(funcs[DIF], "d");
    strcpy(funcs[POW], "^");

    tree_t testTree = {};
    node_t *test = getG();

    testTree.treeStart = test;

    dif_calc_f(testTree.treeStart, &testTree);

    generate_html(&testTree);

}
