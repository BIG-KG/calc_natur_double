#include <string.h>
#include <strings.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"
#include "..\headers\tree_data_base_funk.h"
#include "..\headers\tree_const.h"
#include "..\headers\calculator.h"
#include "..\headers\comporator.h"

const int FUNK_NAME_SIZE      = 30;
const int NUM_OF_FUNK         = 20;
const int MAK_EXPRESSION_SIZE = 1024;

#define NAME_STR(a)  #a
extern char funcs[][20];

static node_t *getBrackets(char *parcingString, int *crrPtr);
static node_t *getSumSub(char *parcingString, int *crrPtr);
static node_t *getDouble(char *parcingString, int *crrPtr);
static node_t *getMulDiv(char *parcingString, int *crrPtr);
static node_t *getPow   (char *parcingString, int *crrPtr);
static node_t *getStr   (char *parcingString, int *crrPtr);


static node_t *getBrackets(char *parcingString, int *crrPtr)
{   
    printf("start Bracks\n");
    if(parcingString[*crrPtr] == '(')
    {
        *crrPtr = *crrPtr + 1 ;
        node_t *val = getSumSub(parcingString, crrPtr);
        if(parcingString[*crrPtr] != ')') 
        {
            printf("crrPtr = %d", *crrPtr);           
            assert(0);
        }
        *crrPtr = *crrPtr + 1 ;
        return val;
    }

    else
    {
        return getDouble(parcingString, crrPtr);
    }
 
}

static node_t *getSumSub(char *parcingString, int *crrPtr)
{
    printf("start sumsub\n");
    node_t *val  = getMulDiv(parcingString, crrPtr);

    node_t *prevOp = NULL;
    node_t *currOp = val ;

    while (parcingString[*crrPtr] == '+' || parcingString[*crrPtr] == '-')
    {
        char op = parcingString[*crrPtr];
        *crrPtr = *crrPtr + 1 ;        

        if (currOp != NULL)
        {
            prevOp = currOp;
        }

        currOp = make_element();
        currOp->data.nodeType = FUNC;
        if (op == '+') currOp->data.nodeData.func = SUM;
        else           currOp->data.nodeData.func = SUB;

        currOp->right = prevOp;

        currOp->left  = getMulDiv(parcingString, crrPtr);
    }

    return currOp;    
}


node_t *getMain(char *inputParcingString)
{   
    printf("start main\n");
    char parcingString[MAK_EXPRESSION_SIZE] = "d(ln(sin(x^3)^2))";
    int crrPtr = 0;

    if (inputParcingString != NULL) strcpy(parcingString, inputParcingString);

    node_t *val = getSumSub(parcingString, &crrPtr);
    if(parcingString[crrPtr] != '\0')  assert(0);
    return val;
}

node_t *getDouble(char *parcingString, int *crrPtr)
{   
    printf("start double\n");

    if( isalpha(parcingString[*crrPtr]) ) return getStr(parcingString, crrPtr);

    node_t *cnstNode = make_element();
    double val = 0;
    int startPos = *crrPtr;
    while ('0' <= parcingString[*crrPtr] && parcingString[*crrPtr] <= '9')
    {
        val = val * 10 + (double)(parcingString[*crrPtr] - '0');
        *crrPtr = *crrPtr + 1 ;
    }
    if(parcingString[*crrPtr] == '.')
    {   

        double step = 0.1;
        while ('0' <= parcingString[*crrPtr] && parcingString[*crrPtr] <= '9')
        {
            val += step * (double)(parcingString[*crrPtr] - '0');
            *crrPtr = *crrPtr + 1 ;
            step *= 0.1;
        }

    }

    if (startPos == *crrPtr) assert(0);

    cnstNode->data.nodeType      = CONST;
    cnstNode->data.nodeData.cnst = val;

    return cnstNode;
}

static node_t *getMulDiv(char *parcingString, int *crrPtr)
{
    printf("start mulDiv\n");

     node_t *val  = getPow(parcingString, crrPtr);

    node_t *prevOp = NULL;
    node_t *currOp = val ;

    while (parcingString[*crrPtr] == '*' || parcingString[*crrPtr] == '/')
    {
        char op = parcingString[*crrPtr];
        *crrPtr = *crrPtr + 1 ;        

        if (currOp != NULL)
        {
            prevOp = currOp;
        }

        currOp = make_element();
        currOp->data.nodeType = FUNC;
        if (op == '*') currOp->data.nodeData.func = MUL;
        else           currOp->data.nodeData.func = DIV;

        currOp->left  = prevOp;

        currOp->right = getPow(parcingString, crrPtr);
    }

    return currOp;
}

static node_t *getPow(char *parcingString, int *crrPtr)
{
    printf("start pow\n");

    node_t *val  = getBrackets(parcingString, crrPtr);
    node_t *val1 = 0;
    node_t *returningNode = val;

    if(parcingString[*crrPtr] == '^')
    {
        *crrPtr = *crrPtr + 1 ;
        val1 = getBrackets(parcingString, crrPtr);

        returningNode = make_element();

        returningNode->data.nodeType      = FUNC;
        returningNode->data.nodeData.func = POW;

        returningNode->left = val ;
        returningNode->right  = val1;       
    }

    return returningNode;
}




 
static node_t *getStr(char *parcingString, int *crrPtr)
{
    printf("start Str\n");
    char funk_name[FUNK_NAME_SIZE] = {};
    int i = 0;
    printf("etstr = %c, %d\n", parcingString[*crrPtr], *crrPtr);

    while( isalpha(parcingString[*crrPtr]) )
    {
        funk_name[i] = parcingString[*crrPtr];
        i++;
        *crrPtr = *crrPtr + 1 ;
    }

    if(i == 0) return NULL;

    if(parcingString[*crrPtr] == '(')
    {
        printf("crrPtr = %d\n", *crrPtr);
        *crrPtr = *crrPtr + 1 ;
        node_t *param = getSumSub(parcingString, crrPtr);
        node_t *func_node = make_element();
        
        func_node->data.nodeType      = FUNC;
        func_node->data.nodeData.func = findfunc(funk_name);
        func_node->right         = param;

        if (parcingString[*crrPtr] != ')') assert(0);
        *crrPtr = *crrPtr + 1 ;

        return func_node;
    }

    else
    {
        node_t *var_node  = make_element();

        var_node->data.nodeType     = VARIABLE;
        var_node->data.nodeData.var = findVar(funk_name);

        return var_node;
    }
}


int findfunc(char * const funcName)
{

    strcasecmp(NULL, NULL);

    printf("funk_name = %s\n", funcName);
    for(int i = 0; i < NUM_OF_FUNK; i ++)
    {
        printf("funk_cmp1 =%s\n\n", funcs[i]);
        if(!strcasecmp(funcName, funcs[i]))
        {
            return i;
        }
    }

    return -1;
}

char findVar(char * const valName)
{   
    return (*valName);
}

