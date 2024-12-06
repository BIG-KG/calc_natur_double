#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\headers\tree_types.h"
#include "..\headers\tree_funck.h"
#include "..\headers\tree_data_base_funk.h"
#include "..\headers\tree_const.h"
#include "..\headers\calculator.h"
#include "..\headers\errors.h"

static node_t *dif_const (node_t *currNode,  tree_t *currTree);
static node_t *dif_geom  (node_t *currNode,  tree_t *currTree);
static node_t *dif_var   (node_t *currNode,  tree_t *currTree);

enum easer
{
    NO_CHANGES,
    HAS_CHANGES
};

int dif_calc_f(node_t *currNode, tree_t *currTree)
{
    if(currNode == NULL) return 0;
    generate_html(currTree);

    void *start = currNode;
    if ( !(currNode->data.nodeType == FUNC && currNode->data.nodeData.func == DIF) )
    {   
        dif_calc_f(currNode->left,  currTree);
                                                                                                                                                                                                                    (currNode->right, currTree);
        return 0;
    }
    //printf("\n\ndiff\n");

    if(currNode->right->data.nodeType == CONST)
    {
        dif_const(currNode, currTree);
        printf("^^^^^^^\n");
        return 0;
    }

    if(currNode->right->data.nodeType == VARIABLE)
    {
        dif_var(currNode, currTree);
        printf("\n********\n");
        return 0;
    }

    if ( (currNode->right->data.nodeData.func == SUM) || 
         (currNode->right->data.nodeData.func == SUB)         )
    {
        dif_sum_sub(currNode, currTree);        
    }

    if ( currNode->right->data.nodeData.func == MUL)
    {   
        dif_mul(currNode, currTree);
    }

    if ( currNode->right->data.nodeData.func == DIV)
    {   
        dif_div(currNode, currTree);
    } 

    if ( currNode->right->data.nodeData.func == COS || 
         currNode->right->data.nodeData.func == SIN              )
    {
        dif_geom(currNode, currTree);
    }

    dif_calc_f(currNode->right, currTree);
    dif_calc_f(currNode->left , currTree);

    return 0;
}

node_t *dif_sum_sub(node_t *currNode,  tree_t *currTree)
{
    printf("_________sum|sub__________");
    node_t *leftNew = make_element();
    leftNew->data.nodeType = FUNC;
    leftNew->data.nodeData.func = DIF;
    leftNew->right = currNode->right->right;


    currNode->data.nodeData = currNode->right->data.nodeData;
    currNode->left = leftNew;
 
    currNode->right->data.nodeData.func = DIF;
    currNode->right->right = currNode->right->left;
    currNode->right->left = NULL; 

    return currNode;
}

node_t *dif_mul(node_t *currNode,  tree_t *currTree)
{
    printf("_________mul__________");
    node_t *right_o = currNode->right->right;
    node_t *left_o  = currNode->right->left ;

    node_t *right_c = copyNode(right_o, currTree);
    node_t *left_c  = copyNode(left_o,  currTree);

    node_t *dif_1 = make_dif_node();
    node_t *dif_2 = make_dif_node();
    node_t *mul_2 = make_mul_node();

    node_t *mul_1 = currNode->right;
    
    currNode->data.nodeData.func = SUM;
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

node_t *dif_geom(node_t *currNode,  tree_t *currTree)
{
    printf("_________geom__________");
    node_t *right = NULL;
    node_t *Ldif  = make_dif_node();
    Ldif->right   = copyNode(currNode->right->right, currTree);
    currNode->left = Ldif;

    currNode->data.nodeData.func = MUL;
    if (currNode->right->data.nodeData.func = COS) currNode->right->data.nodeData.func = SIN;
    else                                           currNode->right->data.nodeData.func = COS;

    return currNode;
}

node_t *dif_div(node_t *currNode,  tree_t *currTree)
{
    printf("_________div__________");
    node_t *difL = make_dif_node();
    node_t *mulR = make_mul_node();
    
    mulR->right  = copyNode (currNode->right->right, currTree);
    mulR->left   = copyNode (mulR->right,            currTree);

    currNode->data.nodeData.func = DIV;
    currNode->left      = mulR;

    difL->right     = currNode->right;
    currNode->right = difL;
    difL->right->data.nodeData.func = MUL;

    dif_mul(difL, currTree);

    difL->data.nodeData.func = SUB;

    return currNode;
}

node_t *dif_const(node_t *currNode,  tree_t *currTree)
{
    printf("_________const__________");    
    delete_tree (currNode->right);
    delete_tree (currNode->left );
    currNode->right = NULL;
    currNode->data.nodeData.cnst = 0;
    currNode->data.nodeType = CONST;

    return currNode;
}

node_t *dif_var(node_t *currNode,  tree_t *currTree)
{
    printf("_________var_________");
    delete_tree (currNode->right);
    delete_tree (currNode->left );
    currNode->right = NULL;
    currNode->data.nodeData.cnst = 1;
    currNode->data.nodeType = CONST;

    return currNode; 
}

node_t *make_dif_node()
{
    node_t *dif_1 = make_element();
    dif_1->data.nodeType = FUNC;
    dif_1->data.nodeData.func     = DIF;        

    return dif_1;
}

node_t *make_mul_node()
{
    node_t *dif_1 = make_element();
    dif_1->data.nodeType = FUNC;
    dif_1->data.nodeData.func     = MUL;        

    return dif_1;
}

int isOperation(node_t *currNode)
{
    if(currNode == NULL) return NULL_PTR_VALUE;

    if( currNode->data.nodeType == FUNC       && 
       SUM <= currNode->data.nodeData.func    &&
       MUL >= currNode->data.nodeData.func          ) return 1;

    return 0;
}

int easy_const_val  (node_t *currNode)
{
    if (currNode == NULL)  return NO_CHANGES;
    if (currNode->left == NULL || currNode->right == NULL) return NO_CHANGES;

    if(  (currNode->left->data.nodeType  == CONST || 
          currNode->right->data.nodeType == CONST   ) &&   
          isOperation(currNode)                             )
    {
        double currConst = 0.0;
        node_t *notConst = NULL;

        if(currNode->left->data.nodeType  == CONST)
        {
            currConst = currNode->left->data.nodeData.cnst;
            notConst  = currNode->right;
        }

        else
        {
            currConst = currNode->right->data.nodeData.cnst;
            notConst  = currNode->left;
        }

        switch (currNode->data.nodeType)
        {
        case SUM:
            if (currConst != 0) break;
        case MUL:
            if (currConst != 1) break;
            currNode->data = notConst->data;
            currNode->left  = NULL;
            currNode->right = NULL;
        
        default:
            break;
        }
    }
}

int easy_const_const(node_t *currNode)
{   
    if (currNode == NULL)  return NO_CHANGES;
    if (currNode->left == NULL || currNode->right == NULL) return NO_CHANGES;

    if( currNode->left->data.nodeType  == CONST && 
        currNode->right->data.nodeType == CONST &&   
        isOperation(currNode)                       )
    {
        double righCons = currNode->right->data.nodeType;
        double leftCons = currNode->left-> data.nodeType;

        switch (currNode->data.nodeData.func)
        {
        case SUM:
            currNode->data.nodeData.cnst = righCons + leftCons;
            break;

        case SUB:
            currNode->data.nodeData.cnst = righCons - leftCons;
            break;

        case MUL:
            currNode->data.nodeData.cnst = righCons * leftCons;
            break;
        
        case DIV:
            currNode->data.nodeData.cnst = righCons / leftCons;
            break;
        
        default:
            break;
        }   


        delete_tree(currNode->right);
        delete_tree(currNode->left );
    
        return HAS_CHANGES;
    }

    return NO_CHANGES;
}

