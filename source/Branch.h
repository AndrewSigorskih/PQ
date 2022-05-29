#include <stdlib.h>
#include "add.h"

#ifndef __BRANCH__H
#define __BRANCH__H

typedef struct
{
    unsigned size;
    int leavesNum; // -1 for uncalculated
    INT* branch;
}Branch;

static const char intSize = sizeof(INT) * 8;

size_t getBitMaskIntSize(unsigned size);

size_t branchGetIntSize(Branch* br);

Branch* branchCreate(unsigned size);

void branchDelete(Branch* branch);

unsigned countZeroRightNum_(INT p);

unsigned countZeroRightNum(INT p);

size_t* branchGetLeavesPos(Branch* br, size_t* leavesNum, size_t maxNum);

void branchNormalize(Branch* br);

char* branchToString(Branch* br);

char branchContradict(Branch* br1, Branch* br2);

void branchPrint(Branch* br);

char branchIsSubset(Branch* br1, Branch* br2);

Branch* branchAnd(Branch* br1, Branch* br2);

void branchAndDest(Branch* br1, Branch* br2, Branch* andBranch);

Branch* branchOr(Branch* br1, Branch* br2);

void branchOrSumDest(Branch* br1, Branch* br2, Branch* orBranch);

void branchOrDest(Branch* br1, Branch* br2, Branch* orBranch);

Branch* branchReverse(Branch* br);

Branch* branchXor(Branch* br1, Branch* br2);

Branch* branchCopy(Branch* br);

Branch* branchCopyToDest(Branch* br, Branch* dest);

char branchIsZero(Branch* br);

void branchAddLeafUnsafe(Branch* br, int leafPos);

int branchCompare(Branch* br1, Branch* br2);

int vBranchCompare(const void* branch1, const void* branch2);

void branchCalculateLeavesPosNum(Branch* br);

int branchGetLeavesPosNum(Branch* br);

#endif
