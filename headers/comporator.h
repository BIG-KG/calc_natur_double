#ifndef COMPORATOR_F
#define COMPORATOR_F

#include "tree_types.h"

node_t *getStr();
node_t *getG();
node_t *getE();
node_t *getT();
node_t *getP();
node_t *getN();
node_t *getS();
int    findfunc (char * const funcName);
int    findVar  (char * const valName);

#endif /*COMPORATOR_F*/