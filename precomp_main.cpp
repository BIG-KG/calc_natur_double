# 1 "main.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.cpp"
# 15 "main.cpp"
extern char funcs[][20];



int main()
{




# 1 ".\\sourse\\codegen.cpp" 1
# 31 ".\\sourse\\codegen.cpp"
strcpy(funcs[SIN], "SIN");;
strcpy(funcs[COS], "COS");;
strcpy(funcs[TNG], "TNG");;
strcpy(funcs[CTG], "CTG");;

strcpy(funcs[ASIN], "ASIN");;
strcpy(funcs[ACOS], "ACOS");;
strcpy(funcs[ATNG], "ATNG");;
strcpy(funcs[ACTG], "ACTG");;

strcpy(funcs[LN], "LN");;
# 26 "main.cpp" 2



    strcpy(funcs[SUB], "-");
    strcpy(funcs[SUM], "+");
    strcpy(funcs[DIV], "/");
    strcpy(funcs[MUL], "*");
    strcpy(funcs[DIF], "d");
    strcpy(funcs[POW], "^");

    tree_t testTree = {};
    node_t *test = getMain();
    testTree.treeStart = test;

    printTex(&testTree);

    generate_html(&testTree);

    dif_calc_f(testTree.treeStart, &testTree);

    printf("end\n");

    generate_html(&testTree);

    ease_tree(testTree.treeStart);

    generate_html(&testTree);

    printf("end2________\n");

    return 0;

}
