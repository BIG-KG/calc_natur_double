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

static int easy_const_val     (node_t *currNode);
static int set_to_zero_node_t (node_t *settingNode);
static int dell_const_node_t  (node_t *destNode, node_t *sourseNode);
static int easy_const_const   (node_t *currNode);



enum easer
{
    NO_CHANGES,
    DELL_ALL_NODE,
    DELL_CONST_NODE
};

int dif_calc_f(node_t *currNode, tree_t *currTree)
{
    if(currNode == NULL) return 0;

    void *start = currNode;
    if ( !(currNode->data.nodeType == FUNC && currNode->data.nodeData.func == DIF) )
    {   
        dif_calc_f(currNode->left,  currTree);
        dif_calc_f(currNode->right, currTree);
        return 0;
    }
    //printf("\n\ndiff\n");

    if(currNode->right->data.nodeType == CONST)
    {
        dif_const(currNode, currTree);
        printf("^^^^^^^\n");
        generate_html(currTree);
        return 0;
    }

    if(currNode->right->data.nodeType == VARIABLE)
    {
        dif_var(currNode, currTree);
        generate_html(currTree);
        return 0;
    }
    //__________________________________________ CODEGEN
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

    generate_html(currTree);
    return 0;
}

node_t *dif_sum_sub(node_t *currNode,  tree_t *currTree)
{
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

    mul_2->left  = left_o;
    mul_2->right = dif_2;
    dif_2->right = right_c;

    mul_1->right = dif_1;
    dif_1->right = left_c;
    mul_1->left  = right_o;

    return currNode;
}

node_t *dif_geom(node_t *currNode,  tree_t *currTree)
{
    node_t *right = NULL;
    node_t *Ldif  = make_dif_node();
    Ldif->right   = copyNode(currNode->right->right, currTree);
    currNode->left = Ldif;

    currNode->data.nodeData.func = MUL;
    if (currNode->right->data.nodeData.func == COS) currNode->right->data.nodeData.func = SIN;
    else                                            currNode->right->data.nodeData.func = COS;

    return currNode;
}

node_t *dif_div(node_t *currNode,  tree_t *currTree)
{
    node_t *difL = make_dif_node();
    node_t *mulR = make_mul_node();
    
    mulR->right  = copyNode (currNode->right->right, currTree);
    mulR->left   = copyNode (mulR->right,            currTree);

    difL->right     = currNode->right;
    currNode->data.nodeData.func = DIV;
    currNode->right   = mulR;

    currNode->left = difL;
    difL->right->data.nodeData.func = MUL;
    generate_html(currTree);
    dif_mul(difL, currTree);
    generate_html(currTree);

    difL->data.nodeData.func = SUB;

    

    return currNode;
}

node_t *dif_const(node_t *currNode,  tree_t *currTree)
{
    delete_tree (currNode->right);
    delete_tree (currNode->left );
    currNode->right = NULL;
    currNode->data.nodeData.cnst = 0;
    currNode->data.nodeType = CONST;

    return currNode;
}

node_t *dif_LN(node_t *currNode,  tree_t *currTree)
{
    currNode->data.nodeData.func = DIV;
    currNode->left = currNode->right;
    currNode->left->data.nodeData.func = DIV;

    currNode->right = currNode->right->right;
    
    currNode->left->right = copyNode(currNode->right, currTree);

    return currNode;    
}


node_t *dif_var (node_t *currNode,  tree_t *currTree)
{
    if(currNode->right->data.nodeData.var == 'x')
    {    
        delete_tree (currNode->right);
        delete_tree (currNode->left );
        currNode->right = NULL;
        currNode->data.nodeData.cnst = 1;
        currNode->data.nodeType = CONST;
    }
    else  
    {
        dif_const (currNode, currTree);
    }
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

int ease_tree (node_t *currNode)
{   
    if(currNode == NULL) return NO_CHANGES;

    ease_tree (currNode->left);
    ease_tree (currNode->right);

    int nodeChanges = NO_CHANGES;

    if ( (nodeChanges = easy_const_val   (currNode)) != NO_CHANGES) return nodeChanges;
    if ( (nodeChanges = easy_const_const (currNode)) != NO_CHANGES) return nodeChanges;

    return NO_CHANGES;
}

int easy_const_val (node_t *currNode)
{   
    if (currNode == NULL)                                  return NO_CHANGES;
    if (currNode->left == NULL || currNode->right == NULL) return NO_CHANGES;
    if (currNode->left->data.nodeType  == CONST   &&
          currNode->right->data.nodeType == CONST     )      return NO_CHANGES;

    double  currConst = 0.0;
    node_t *notConst = NULL;
    int     changeType = NO_CHANGES;

    if (  (currNode->left->data.nodeType  == CONST || 
           currNode->right->data.nodeType == CONST   ) &&   
          isOperation(currNode)                             )
    {
        if (currNode->left->data.nodeType == CONST)
        {
            currConst = currNode->left->data.nodeData.cnst;
            notConst  = currNode->right;
        }

        else
        {
            //printf("start ease===============r\n\n");
            currConst = currNode->right->data.nodeData.cnst;
            notConst  = currNode->left;
        }

        switch (currNode->data.nodeData.func)
        {
        //__________________________________________ CODEGEN + CODE
        case SUM:
            printf("const = %lf\n", currConst);
            if (currConst == 0.0) 
            {
                changeType = DELL_CONST_NODE;
                printf("dcsc\n");
            }
            break;
        case SUB:
            if (currConst == 0.0 && notConst == currNode->left) changeType = DELL_CONST_NODE;
            break;
        case MUL:
            printf("MUL");
            if (currConst == 1.0) changeType = DELL_CONST_NODE;
            if (currConst == 0.0) changeType = DELL_ALL_NODE;
            break;
        case DIV:
            if (currConst == 0.0 && notConst == currNode->right) changeType = DELL_ALL_NODE;
            if (currConst == 1.0 && notConst == currNode->left)  changeType = DELL_CONST_NODE;
            break;
        //__________________________________________ CODEGEN + CODE      
        default:
            return NO_CHANGES;
        }
    }

      if (changeType == DELL_ALL_NODE  ) return set_to_zero_node_t (currNode);
      if (changeType == DELL_CONST_NODE) return dell_const_node_t  (currNode, notConst);
      return NO_CHANGES;

    
}

static int set_to_zero_node_t (node_t *settingNode)
{
    if (settingNode == NULL) return NULL_PTR_VALUE;

    delete_tree(settingNode->left );
    delete_tree(settingNode->right);

    settingNode->left  = NULL;
    settingNode->right = NULL;

    settingNode->data.nodeType      = CONST;
    settingNode->data.nodeData.cnst = 0.0;

    return 0;
}

static int dell_const_node_t (node_t *destNode, node_t *sourseNode)
{
    if (destNode == NULL || sourseNode == NULL) return NO_CHANGES;

    node_t *right = destNode->right;
    node_t *left  = destNode->left;

    destNode->data  = sourseNode->data;
    destNode->left  = sourseNode->left;
    destNode->right = sourseNode->right;

    if (right != NULL) free (right);
    if (left  != NULL) free(left);

    return 0;
}

int easy_const_const (node_t *currNode)
{   
    if (currNode == NULL)  return NO_CHANGES;
    if (currNode->left == NULL || currNode->right == NULL) return NO_CHANGES;

    if ( currNode->left->data.nodeType  == CONST && 
        currNode->right->data.nodeType == CONST &&   
        isOperation(currNode)                       )
    {
        double righCons = currNode->right->data.nodeData.cnst;
        double leftCons = currNode->left-> data.nodeData.cnst;

        switch (currNode->data.nodeData.func)
        {

        //__________________________________________ CODEGEN
        #define GENER(F_NAME, str_name)                         \
        case F_NAME:
            currNode->data.nodeData.cnst = righCons #str_name leftCons;
            break;
        
        case DIV:
            currNode->data.nodeData.cnst = righCons / leftCons;
            break;
        //__________________________________________ CODEGEN
        
        default:
            break;
        }   


        delete_tree(currNode->right);
        delete_tree(currNode->left );

        currNode->data.nodeType = CONST; 

        currNode->left  = NULL;
        currNode->right = NULL;
    
        return DELL_CONST_NODE;
    }

    return NO_CHANGES;
}
