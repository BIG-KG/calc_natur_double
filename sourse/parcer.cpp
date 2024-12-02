#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"
#include "..\headers\tree_data_base_funk.h"
#include "..\headers\tree_const.h"
#include "..\headers\calculator.h"
#include "..\headers\comporator.h"

const int FUNK_NAME_SIZE = 30;
const int NUM_OF_FUNK    = 20;

#define NAME_STR(a)  #a
extern char funcs[][20];

int p = 0;

//char s[100] = "d((x+1)/(x*x))$";

char s[100] = "d((x+1)/(x*x))$";

node_t *getP()
{

    if(s[p] == '(')
    {
        p++;
        node_t *val = getE();
        if(s[p] != ')') 
        {
            printf("p = %d", p);           
            assert(0);
        }
        p++;
        return val;
    }

    else
    {
        return getN();
    }
 
}

node_t *getE()
{
    //printf("start E\n");
    node_t *val  = getT();

    node_t *prevOp = NULL;
    node_t *currOp = val ;

    while (s[p] == '+' || s[p] == '-')
    {
        char op = s[p];
        p++;        

        if (currOp != NULL)
        {
            prevOp = currOp;
        }

        currOp = make_element();
        currOp->data.nodeType = FUNC;
        if (op == '+') currOp->data.data = SUM;
        else           currOp->data.data = SUB;

        currOp->right = prevOp;

        currOp->left  = getT();
    }

    return currOp;    
}


node_t *getG()
{
    node_t *val = getE();
    if(s[p] != '$')  assert(0);
    return val;
}

node_t *getN()
{
    //printf("start N\n");
    if( isalpha(s[p]) ) return getStr();

    node_t *cnstNode = make_element();
    double val = 0;
    int startPos = p;
    while ('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + (double)(s[p] - '0');
        p++;
    }
    if (startPos == p) assert(0);

    cnstNode->data.nodeType = CONST;
    cnstNode->data.data     = val;

    return cnstNode;
}

//getT->getS
node_t *getT()
{
     node_t *val  = getS();

    node_t *prevOp = NULL;
    node_t *currOp = val ;

    while (s[p] == '*' || s[p] == '/')
    {
        char op = s[p];
        p++;        

        if (currOp != NULL)
        {
            prevOp = currOp;
        }

        currOp = make_element();
        currOp->data.nodeType = FUNC;
        if (op == '*') currOp->data.data = MUL;
        else           currOp->data.data = DIV;

        currOp->left  = prevOp;

        currOp->right = getS();
    }

    return currOp;
}

node_t *getS()
{
    node_t *val  = getP();
    node_t *val1 = 0;
    node_t *returningNode = val;

    if(s[p] == '^')
    {
        p++;
        val1 = getP();

        returningNode = make_element();

        returningNode->data.nodeType = FUNC;
        returningNode->data.data     = POW;

        returningNode->right = val ;
        returningNode->left  = val1;       
    }

    return returningNode;
}




node_t *getStr()
{
    char funk_name[FUNK_NAME_SIZE] = {};
    int i = 0;
    printf("etstr = %c, %d\n", s[p], p);

    while( isalpha(s[p]) )
    {
        funk_name[i] = s[p];
        i++;
        p++;
    }

    if(i == 0) return NULL;

    if(s[p] == '(')
    {
        printf("p = %d\n", p);
        p++;
        node_t *param = getE();
        node_t *func_node = make_element();
        
        func_node->data.nodeType = FUNC;
        func_node->data.data     = findfunc(funk_name);
        func_node->right         = param;

        if (s[p] != ')') assert(0);
        p++;

        return func_node;
    }

    else
    {
        node_t *var_node  = make_element();

        var_node->data.nodeType = VARIABLE;
        var_node->data.data     = findVar(funk_name);

        return var_node;
    }
}

int findfunc(char * const funcName)
{
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

int findVar(char * const valName)
{ 
    return 1;
}