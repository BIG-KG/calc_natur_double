#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "..\headers\tree_const.h"
#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"

extern char funcs[][20];

static int printTex_rec(node_t *currNode, FILE *outputFile);


int printTex(tree_t *currTree)
{
    FILE *texoutput_tex = fopen("texoutput.tex", "w");

    printTex_rec(currTree->treeStart, texoutput_tex);

    fclose(texoutput_tex);

    return 0;
}

static int printTex_rec(node_t *currNode, FILE *outputFile)
{
    if (currNode == NULL) return 0;
    if (outputFile == NULL) return 1;
    node_t *leftNodeFunk = NULL;

    if (currNode->data.nodeType == VARIABLE) 
    {
        fprintf(outputFile, "%c", currNode->data.nodeData.var);
        return 0;
    }

    if (currNode->data.nodeType == CONST) 
    {
        fprintf(outputFile, "%lf", currNode->data.nodeData.cnst);
        return 0;
    }

    if (currNode->data.nodeType == FUNC)
    {   
        switch (currNode->data.nodeData.func)
        {
        //__________________________________________ CODEGEN + CODE

        #define GENER(TYPE, str_name)                               \
        case(TYPE):                                         \
            fprintf(outputFile, " (");                      \
            printTex_rec(currNode->left, outputFile);       \
            fprintf(outputFile, str_name);                  \
            printTex_rec(currNode->right, outputFile);      \
            fprintf(outputFile, ") ");                      \
            break;                                          \

        #define INFICS_OPER
        #include "codegen.cpp"
        #undef INFICS_OPER
        #undef GENER

        #define GENER(F_NAME, str_name)                         \
        case(F_NAME):                                           \
            fprintf(outputFile, "%s(", str_name);               \
            printTex_rec(currNode->right,  outputFile);         \
            leftNodeFunk = currNode->left;                      \
        while(leftNodeFunk){                                    \
                fprintf(outputFile, ", ");                      \
                printTex_rec(leftNodeFunk->right, outputFile);  \
                leftNodeFunk = leftNodeFunk->left;}             \
            printf(") ");                                       \
            break;                                              \

        #define ALL_FUNKS
        #include "codegen.cpp"
        #undef ALL_FUNKS
        #undef GENER

        case(DIV):                                        
            fprintf(outputFile, "\\frac{");   
            printTex_rec(currNode->left,  outputFile);        
            fprintf(outputFile, "}{");                       
            printTex_rec(currNode->right, outputFile);      
            fprintf(outputFile, "} "); 
            break;

        case(POW):
            printTex_rec(currNode->left,  outputFile);        
            fprintf(outputFile, "^{");                       
            printTex_rec(currNode->right, outputFile);      
            fprintf(outputFile, "} "); 
            break;

        default:
            fprintf(outputFile, " %s(", funcs[currNode->data.nodeData.func]);
            printTex_rec(currNode->right, outputFile);
            fprintf(outputFile, ") ");
            break;
        }

        //__________________________________________ CODEGEN + CODE
    }

    return 1;
}