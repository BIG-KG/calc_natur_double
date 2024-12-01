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
    if(currNode == NULL) return 0;

    generate_html(currTree);

    void *start = currNode;
    if ( !(currNode->data.nodeType == FUNC && currNode->data.data == DIF) )
    {   
        printf("\n\nno_dif\n");
        printf("type = %d", currNode->data.nodeType);
        if(currNode->data.nodeType == FUNC) 
        {
            printf("test\n");
            printf("\nf = %d\n", currNode->data.data); 
        }
        
        dif_calc_f(currNode->left,  currTree);
        dif_calc_f(currNode->right, currTree);

        return 0;
    }
    printf("\n\ndiff\n");

    if(currNode->right->data.nodeType == CONST)
    {
        printf("case2\n");
        delete_tree (currNode->right);
        currNode->right = NULL;
        currNode->data.data = 0;
        currNode->data.nodeType = CONST;

        return 0;
    }

    if(currNode->right->data.nodeType == VARIABLE)
    {
        printf("case1\n");
        delete_tree (currNode->right);
        currNode->right = NULL;
        currNode->data.data = 1;
        currNode->data.nodeType = CONST;

        return 0;
    }

    if ( (currNode->right->data.data == SUM) || (currNode->right->data.data == SUB) )
    {
        printf("case0\n");
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
        return 0;
    }

    if ( currNode->right->data.data == MUL)
    {   
        printf("startMUL\n");
        dif_div(currNode, currTree);

        printf("11111111111\n");
        dif_calc_f(currNode->left, currTree);
        generate_html(currTree);
        dif_calc_f(currNode->right, currTree);

        generate_html(currTree);
        printf("22222222222\n");

        printf("st = %p , curr= %p", start, currNode);
        printf("type = %d, data = %d", currNode->data.nodeType, currNode->data.data);
        return 0;
    }
    printf("bliat\n");
    return 0;
}

node_t *dif_div(node_t *currNode,  tree_t *currTree)
{
    node_t *right_o = currNode->right->right;
    node_t *left_o  = currNode->right->left ;

    node_t *right_c = copyNode(right_o, currTree);
    node_t *left_c  = copyNode(left_o,  currTree);

    node_t *dif_1 = make_element();
    dif_1->data.nodeType = FUNC;
    dif_1->data.data     = DIF;        

    node_t *dif_2 = make_element();
    dif_2->data.nodeType = FUNC;
    dif_2->data.data     = DIF;

    node_t *mul_1 = currNode->right;

    node_t *mul_2 = make_element();
    mul_2->data.nodeType = FUNC;
    mul_2->data.data     = MUL;


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