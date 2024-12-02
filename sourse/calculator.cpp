#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"
#include "..\headers\tree_data_base_funk.h"
#include "..\headers\tree_const.h"
#include "..\headers\calculator.h"

int dif_calc_f(node_t *currNode, tree_t *currTree)
{
    void *start = currNode;
    if ( !(currNode->data.nodeType == FUNC && currNode->data.data == DIF) )
    {   
        dif_calc_f(currNode->left,  currTree);
        dif_calc_f(currNode->right, currTree);
        return 0;
    }
    printf("\n\ndiff\n");

    if(currNode->right->data.nodeType == CONST)
    {
        dif_const(currNode, currTree);
        return 0;
    }

    if(currNode->right->data.nodeType == VARIABLE)
    {
        dif_var(currNode, currTree);
        return 0;
    }

    if ( (currNode->right->data.data == SUM) || (currNode->right->data.data == SUB) )
    {
        dif_sum_sub(currNode, currTree);        
    }

    if ( currNode->right->data.data == MUL)
    {   
        dif_mul(currNode, currTree);
    }

    if ( currNode->right->data.data == DIV)
    {   
        dif_div(currNode, currTree);
    }

    dif_calc_f(currNode->right, currTree);
    dif_calc_f(currNode->left , currTree);

    return 0;
}

node_t *dif_sum_sub(node_t *currNode,  tree_t *currTree)
{
    node_t *leftNew = make_element();
    leftNew->data.nodeType = FUNC;
    leftNew->data.data = DIF;
    leftNew->right = currNode->right->right;


    currNode->data.data = currNode->right->data.data;
    currNode->left = leftNew;
 
    currNode->right->data.data = DIF;
    currNode->right->right = currNode->right->left;
    currNode->right->left = NULL; 

    dif_calc_f(currNode->left,  currTree);
    dif_calc_f(currNode->right, currTree);

    return currNode;
}

node_t *dif_mul(node_t *currNode,  tree_t *currTree)
{
    node_t *right_o = currNode->right->right;
    node_t *left_o  = currNode->right->left ;

    node_t *right_c = copyNode(right_o, currTree);
    node_t *left_c  = copyNode(left_o,  currTree);

    node_t *dif_1 = make_dif_node();
    node_t *dif_2 = make_dif_node();
    node_t *mul_2 = make_mul_node();

    node_t *mul_1 = currNode->right;
    
    currNode->data.data = SUM;
    currNode->right = mul_2;
    currNode->left  = mul_1;

    mul_2->left  = right_o;
    mul_2->right = dif_2;
    dif_2->right = left_c;

    mul_1->right = dif_1;
    dif_1->right = right_c;
    mul_1->left  = left_o;

    printf("end_mul\n");
    return currNode;
}

node_t *dif_div(node_t *currNode,  tree_t *currTree)
{
    node_t *difL = make_dif_node();
    node_t *mulR = make_mul_node();
    
    mulR->right  = copyNode (currNode->right->right, currTree);
    mulR->left   = copyNode (mulR->right,            currTree);

    currNode->data.data = DIV;
    currNode->left      = mulR;

    difL->right     = currNode->right;
    currNode->right = difL;
    difL->right->data.data = MUL;

    dif_mul(difL, currTree);

    difL->data.data = SUB;

    return currNode;
}

node_t *dif_const(node_t *currNode,  tree_t *currTree)
{
    delete_tree (currNode->right);
    delete_tree (currNode->left );
    currNode->right = NULL;
    currNode->data.data = 0;
    currNode->data.nodeType = CONST;

    return currNode;
}

node_t *dif_var(node_t *currNode,  tree_t *currTree)
{
    delete_tree (currNode->right);
    delete_tree (currNode->left );
    currNode->right = NULL;
    currNode->data.data = 1;
    currNode->data.nodeType = CONST;

    return currNode; 
}

node_t *make_dif_node()
{
    node_t *dif_1 = make_element();
    dif_1->data.nodeType = FUNC;
    dif_1->data.data     = DIF;        

    return dif_1;
}

node_t *make_mul_node()
{
    node_t *dif_1 = make_element();
    dif_1->data.nodeType = FUNC;
    dif_1->data.data     = MUL;        

    return dif_1;
}