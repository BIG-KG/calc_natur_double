#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include <tree_const.h>
#include <tree_types.h>
#include <tree_funck.h>
#include <calculator.h>

extern char funcs[][20];

static int printTex_rec(node_t *currNode, FILE *outputFile, int needBrakes);


int printTex(tree_t *currTree, FILE *texoutput_tex)
{     
    //ease_tree(currTree->treeStart);

    static int numOfLaunches = 0;

    if (numOfLaunches == 0)
    {   
        fprintf(texoutput_tex, "\\documentclass[a4paper]{article} \n\\begin{document} \n"

                                "\\begin{titlepage}\n"
                                "\\vspace*{2cm}\n"

                                "{\\huge \\textbf Differentiation of a function} \\\\ \n\n\n{ \\large");

        fprintf(texoutput_tex, "$");
        printTex_rec(currTree->treeStart, texoutput_tex, 0);
        fprintf(texoutput_tex, "$");

        fprintf(texoutput_tex," }\\ \\vspace{1.5cm}\n"
                                " \\ \\ {\\textbf{Bekmambetov George}}\\ \n"
                                "\\vfill\n"
 
                                "\\vspace{0.8cm}\n"
                                "\\end{titlepage}\n"
                                );
    
    numOfLaunches ++;
    return 0;
    }
    

    fprintf(texoutput_tex, "$");
    printTex_rec(currTree->treeStart, texoutput_tex, 0);
    fprintf(texoutput_tex, "$");

    fprintf(texoutput_tex, "\\\\\n");

    return 0;
}

static int printTex_rec(node_t *currNode, FILE *outputFile, int needBrackes)
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
        fprintf(outputFile, "%lg", currNode->data.nodeData.cnst);
        return 0;
    }

    int needRighrBrakes = 0;

    if (currNode->data.nodeType == FUNC && currNode->data.nodeData.func == MUL &&
        currNode->right->data.nodeType == FUNC && (currNode->right->data.nodeData.func == SUM || currNode->right->data.nodeData.func == SUB))
        needRighrBrakes = 1;

    int needLeftBrakes = 0;

    if (currNode->data.nodeType == FUNC && currNode->data.nodeData.func == MUL &&
        currNode->left->data.nodeType == FUNC && (currNode->left->data.nodeData.func == SUM || currNode->left->data.nodeData.func == SUB))
        needLeftBrakes = 1;



    if (currNode->data.nodeType == FUNC)
    {
       
        switch (currNode->data.nodeData.func)
        {
        //__________________________________________ CODEGEN + CODE

        #define GENER(TYPE, str_name)                       \
        case(TYPE):                                         \
            if(needBrackes) fprintf(outputFile, " (");      \
            printTex_rec(currNode->left,  outputFile, needLeftBrakes);       \
            fprintf(outputFile, str_name);                  \
            printTex_rec(currNode->right, outputFile, needRighrBrakes);      \
            if(needBrackes) fprintf(outputFile, ") ");      \
            break;                                          \

        #define INFICS_OPER
        #include "codegen.cpp"
        #undef INFICS_OPER
        #undef GENER

        #define GENER(F_NAME, str_name)                         \
        case(F_NAME):                                           \
            fprintf(outputFile, "%s(", str_name);               \
            printTex_rec(currNode->right,  outputFile, 0);         \
            leftNodeFunk = currNode->left;                      \
        while(leftNodeFunk){                                    \
                fprintf(outputFile, ", ");                      \
                printTex_rec(leftNodeFunk->right, outputFile, 0);  \
                leftNodeFunk = leftNodeFunk->left;}             \
            fprintf(outputFile, ") ");                                       \
            break;                                              \

        #define ALL_FUNKS
        #include "codegen.cpp"
        #undef ALL_FUNKS
        #undef GENER

        case(DIV):                                        
            fprintf(outputFile, "\\frac{");   
            printTex_rec(currNode->left,  outputFile, 0);        
            fprintf(outputFile, "}{");                       
            printTex_rec(currNode->right, outputFile, 0);      
            fprintf(outputFile, "} "); 
            break;

        case(POW):
            printTex_rec(currNode->left,  outputFile, 0);        
            fprintf(outputFile, "^{");                       
            printTex_rec(currNode->right, outputFile, 0);      
            fprintf(outputFile, "} "); 
            break;

        default:
            fprintf(outputFile, " %s(", funcs[currNode->data.nodeData.func]);
            printTex_rec(currNode->right, outputFile, 0);
            fprintf(outputFile, ") ");
            break;
        }

        //__________________________________________ CODEGEN + CODE
    }
    return 1;
}

int makeTex(FILE *outputFile)
{
    fprintf(outputFile, "\\end{document}");
    fclose(outputFile);

    system("pdflatex C:\\MIPT\\Differenciator\\tex_file.tex");

    return 0;
}