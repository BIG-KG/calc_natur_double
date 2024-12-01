#ifndef TREE_TYPES
#define TREE_TYPES

#include <stdint.h>
#include "tree_const.h"

enum dataTypes
{
    FUNC = 2,
    CONST = 3,
    VARIABLE = 4
};

enum funcs
{
    SIN,
    COS,
    LOGN,
    DIF,
    SUM,
    SUB,
    DIV,
    MUL,
    POW,
    LOG
};

struct calc_node
{
    int nodeType = -1; //no data type has code -1
    int data     = 0;
};


typedef calc_node elem_t;

struct node_t
{
    elem_t data   = {};
    node_t *right = NULL; 
    node_t *left  = NULL;

};

struct tree_t
{
    //void   *occupiedMemStart = NULL; // remove
    node_t *treeStart       = NULL;
    //int     capacity         = 0;
    //int     currentElement   = 0;

};

struct node_way_t
{
    int     depth       = 0;
    int    *way         = NULL;
    node_t *node_addres = NULL;

};

#endif /*TREE_TYPES*/
