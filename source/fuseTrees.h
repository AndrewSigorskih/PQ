#ifndef _FUSE_TREES_H_
#define _FUSE_TREES_H_
#include <stdlib.h>
#include "Tree.h"
#include "consensus.h"

int* randomArgMax(double** table, int n);
int** pairwiseLeafDistances(Tree* tree);
double** relativeLeafDistances(int** table1, int** table2, int* permutation, int n);
size_t* getLeavesToTransfer(Tree* reference, char* name1, char* name2, int* pairIdx);
Tree* fuseTrees(Tree* intree1, Tree* intree2);
#endif