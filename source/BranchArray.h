#ifndef __BRANCH_ARRAY_H
#define __BRANCH_ARRAY_H

#include "Branch.h"

typedef struct
{
    Branch** array;
    unsigned size;
    unsigned maxSize;
}BranchArray;

BranchArray* branchArrayCreate(unsigned startSize);
void branchArrayDelete(BranchArray* ba);
void branchArrayAdd(BranchArray* ba, Branch* branch);
void branchArraySort(BranchArray* ba);
void branchArrayExtend(BranchArray* dest, BranchArray* source);

#endif