#include "fuseTrees.h"

int* randomArgMax(double** table, int n)
{
    int i, j, k, maxNum = 0;
    double maxVal;
    int** maxIdx;
    int* res;

    maxVal = 0;
    maxNum = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (table[i][j] > maxVal) 
            { 
                maxVal = table[i][j]; 
                maxNum = 1;
            } else if (table[i][j] == maxVal) {
                maxNum++;
            }

        }
    }

    maxIdx = (int**)calloc(maxNum, sizeof(int*));
	for (i = 0; i < maxNum; i++)
	{
		maxIdx[i] = (int*)calloc(2, sizeof(int));
	}

    k = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (fabs(maxVal - table[i][j]) <= 1.192093e-06)
            {
                maxIdx[k][0] = i;
                maxIdx[k][1] = j;
                k++;
            }
        }
    }

    res = calloc(2, sizeof(int));
    if (maxNum == 1)
	{
		res[0] = maxIdx[0][0];
		res[1] = maxIdx[0][1];
	} else {
		j = rand() % maxNum;
		res[0] = maxIdx[j][0];
		res[1] = maxIdx[j][1];
	}

    for (i = 0; i < maxNum; i++)
	{
		free(maxIdx[i]);
	}
	free(maxIdx);

    return res;
} //randomArgMax

int** pairwiseLeafDistances(Tree* tree)
{
    int i, j, n;
    int** table;

    n = tree->leavesNum;
    table = calloc(n, sizeof(int*));
    for (i = 0; i < n; i++)
    {
        table[i] = calloc(n, sizeof(int));
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (j == i)
            {
                table[i][j] = 0;
            } else {
                table[i][j] = treeGetDist(tree, tree->leaves[i]->pos, tree->leaves[j]->pos);
            }
        }
    }
    return table;
} //pairwiseLeafDistances

double** relativeLeafDistances(int** table1, int** table2, int* permutation, int n)
{
    int i, j, d1, d2;
    double** table;
    table = (double**)calloc(n, sizeof(double*));
    for (i = 0; i < n; i++)
    {
        table[i] = (double*)calloc(n, sizeof(double));
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            d1 = table1[i][j];
            d2 = table2[permutation[i]][permutation[j]];
            if (i == j)
            {
                table[i][j] = 0;
            } else {
                if ((d1 + d2)==0) { printf("zero division here: %d %d", i, j); }
                table[i][j] = ((double)abs(d1 - d2)) / (d1 + d2);
            }
        }
        
    }
    return table;
} //relativeLeafDistances

BranchArray* treeToBranchFull(Tree* tree, int* permutation)
{
    INT p = 1;
    int i = 0;
    int j = 0;
    unsigned branchNum = tree->nodesNum;
    BranchArray* ba;
    NodeStack* stack;
    Node* curNode = 0;
    Node* nextNode = 0;

    ba = branchArrayCreate(branchNum);
    stack = nodeStackCreate(tree->nodesNum);

    treeWash(tree);

    for(i = 0; i < branchNum; ++i)
    {
        branchArrayAdd(ba, branchCreate(tree->leavesNum));
    }

    for(i = 0; i < tree->leavesNum; ++i)
    {
        p = 1;
        p = p << (permutation[i] & (intSize - 1));
        ba->array[tree->leaves[i]->pos]->branch[permutation[i] / intSize] |= p;
        tree->leaves[i]->color = BLACK;
    }

    if (tree->nodes[0]->name == 0)
    {
        curNode = tree->nodes[0];
    }
    else
    {
        curNode = tree->nodes[1];
    }

    nodeStackPush(stack, curNode);
    curNode->color = GREY;

    while (stack->curSize != 0)
    {
        curNode = nodeStackPeek(stack);
        nextNode = 0;
        for(i = 0; i < curNode->neiNum; ++i)
        {
            if (curNode->neighbours[i]->color == WHITE)
            {
                nextNode = curNode->neighbours[i];
                break;
            }
        }
        if (nextNode)
        {
            nodeStackPush(stack, nextNode);
            nextNode->color = GREY;
        }
        else
        {
            for(i = 0; i < curNode->neiNum; ++i)
            {
                for(j = 0; j < tree->leavesNum / 64 + 1; ++j)
                {
                    ba->array[curNode->pos]->branch[j] |= \
                            ba->array[curNode->neighbours[i]->pos]->branch[j];
                }
            }
            nodeStackPop(stack);
            curNode->color = BLACK;
        }
    }

    nodeStackDelete(stack);
    return ba;
}


size_t* getLeavesToTransfer(Tree* reference, char* name1, char* name2, int* pairIdx)
{
    int i, j, k, pos1, pos2, minSize;
    int* permutation;
    size_t* leavesPos;
    Branch* mask;
    Branch* andBranch;
    Branch* maxBranch;
    BranchArray* ba;
    BranchArray* revBa;

    for (i = 0; i < reference->leavesNum; i++)
    {
        if (strcmp(reference->leaves[i]->name, name1) == 0) { pos1 = i; }
        else if (strcmp(reference->leaves[i]->name, name2) == 0) { pos2 = i; }
    }
   
    // technical branch containing only two needed leaves
    mask = branchCreate(reference->leavesNum);
    branchAddLeafUnsafe(mask, pos1);
    branchAddLeafUnsafe(mask, pos2);
    branchCalculateLeavesPosNum(mask);

    permutation = getRange(0, reference->leavesNum);
    ba = treeToBranchFull(reference, permutation);
    free(permutation);
    revBa = branchArrayCreate(ba->maxSize);

    // maximum branch for array purposes
    maxBranch = branchCreate(reference->leavesNum);
    for (i = 0; i < maxBranch->size; i++)
    {
        branchAddLeafUnsafe(maxBranch, i); 
    }

    for (i = 0; i < ba->maxSize; i++)
    {
        branchArrayAdd(revBa, branchXor(maxBranch, ba->array[i]));
    }
    branchDelete(maxBranch);

    j = 0;
    minSize = reference->leavesNum;

    // get minimal of branches that have two selected leaves
    // from up and down perspective
    for (i = 0; i < ba->size; i++)
    {
        // first array
        branchCalculateLeavesPosNum(ba->array[i]);
        k = branchGetLeavesPosNum(ba->array[i]);

        andBranch = branchAnd(ba->array[i], mask);
        branchCalculateLeavesPosNum(andBranch);
        if ((k < minSize) && (branchGetLeavesPosNum(andBranch) == 2))
        {
            minSize = k;
            maxBranch = ba->array[i];
			j = i;
        }
        branchDelete(andBranch);

        // second array
        branchCalculateLeavesPosNum(revBa->array[i]);
        k = branchGetLeavesPosNum(revBa->array[i]);

        andBranch = branchAnd(revBa->array[i], mask);
        branchCalculateLeavesPosNum(andBranch);
        if ((k < minSize) && (branchGetLeavesPosNum(andBranch) == 2))
        {
            minSize = k;
            maxBranch = revBa->array[i];
			j = i;
        }
        branchDelete(andBranch);

    }

    /*
    if (minSize == reference->leavesNum)
    {
        printf("failed to catch branch!\n");
        printf("mask branch: ");
        branchPrint(mask);
        printf("with %d leaves\n", branchGetLeavesPosNum(mask));
        printf("\n");
        Branch* rev;
        rev = branchReverse(mask);
        branchCalculateLeavesPosNum(rev);
        printf("reversed branch: ");
        branchPrint(rev);
        printf("with %d leaves\n", branchGetLeavesPosNum(rev));
        exit(1);
    } */

    // store size for later 
    pairIdx[0] = minSize;
    pairIdx[1] = reference->leavesNum - minSize;

    // create leaves names array
    size_t leavesPosSize = 0;
    leavesPos = branchGetLeavesPos(maxBranch, &leavesPosSize, reference->leavesNum);

    branchDelete(mask);
    for(i = 0; i < ba->size; ++i)
    {
        branchDelete(ba->array[i]);
        branchDelete(revBa->array[i]);
    }
    branchArrayDelete(ba);
    branchArrayDelete(revBa);
    return leavesPos;
} //getLeavesToTransfer

Tree* fuseTrees(Tree* intree1, Tree* intree2)
{
    int i, j, k, found;
    int* pairIdx;
    int* permutation;
    int** dist1;
    int** dist2;
    double** relDist;
    char* name1;
    char* name2;
    size_t* leavesToTransferIdx;
    size_t* leavesToKeepIdx;
    char** leavesToTransfer;
    char** leavesToKeep;
    Tree* reference;
    Tree* rugged;
    Tree* tmp1;
    Tree* tmp2;
    Tree* result; 
    // get two relatively distant leaves
    permutation = calculateLeavesPermutation(intree1, intree2);

	dist1 = pairwiseLeafDistances(intree1);
	dist2 = pairwiseLeafDistances(intree2);
    
	relDist = relativeLeafDistances(dist1, dist2, permutation, intree1->leavesNum);
    
	pairIdx = randomArgMax(relDist, intree1->leavesNum);
	i = pairIdx[0];
	j = pairIdx[1];
	name1 = intree1->leaves[i]->name;
	name2 = intree1->leaves[j]->name;

    // decide which tree is a donor
    if (dist1[i][j] < dist2[permutation[i]][permutation[j]])
    {
        reference = treeCopy(intree1, 1);
        rugged = treeCopy(intree2, 1);
    } else {
        reference = treeCopy(intree2, 1);
        rugged = treeCopy(intree1, 1);
    }
    // get minimal branch in reference, return leaf positions and store size in pairIdx
    leavesToTransferIdx = getLeavesToTransfer(reference, name1, name2, pairIdx);
    leavesToKeepIdx = calloc(pairIdx[1], sizeof(size_t));
    k = 0;
    for (i = 0; i < reference->leavesNum; i++)
    {
        found = 0;
        for (j = 0; j < pairIdx[0]; j++)
        {
            if (leavesToTransferIdx[j] == i)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            leavesToKeepIdx[k] = i;
            k++;
        }
    }
    
    if (k != pairIdx[1]) 
    {
        fprintf(stderr, "ERROR: number of leaves to keep in rugged was not estimated properly! %d %d \n", k, pairIdx[1]); 
    }
    // randomly select fuse point
    k = rand() % pairIdx[1];
    found = leavesToKeepIdx[k];

    leavesToTransfer = (char**)calloc(sizeof(char*), pairIdx[0] + 1);
    leavesToKeep = (char**)calloc(sizeof(char*), pairIdx[1]);
    
    for (i = 0; i < pairIdx[0]; i++)
    {
        leavesToTransfer[i] = reference->leaves[leavesToTransferIdx[i]]->name;
    }
    
    leavesToTransfer[pairIdx[0]] = reference->leaves[found]->name; //!!!
    for (i = 0; i < pairIdx[1]; i++)
    {
        leavesToKeep[i] = reference->leaves[leavesToKeepIdx[i]]->name;
    }
    // crop trees
    tmp1 = treePrune(reference, leavesToTransfer, pairIdx[0] + 1, 1);
    tmp2 = treePrune(rugged, leavesToKeep, pairIdx[1], 1);

    // fuse trees:
    Node* curnode;
    Node* nei;
    Node* leaf;
    // realloc tmp2 nodes and leafs
    tmp2->nodes = realloc(tmp2->nodes, sizeof(Node*)*intree1->nodesNum);
    tmp2->leaves = realloc(tmp2->leaves, sizeof(Node*)*intree1->leavesNum);
    // create node to insert in tmp2
    curnode = nodeCreate();
    tmp2->nodes[tmp2->nodesNum] = curnode;
    curnode->pos = tmp2->nodesNum;
    tmp2->nodesNum++;
    // select leaf found by name and link to newnode
    // select leaf's nei from tmp2 and link to it
    for (i = 0; i < tmp2->leavesNum; i ++)
    {
        if (strcmp(tmp2->leaves[i]->name, reference->leaves[found]->name) == 0)
        {
            nei = tmp2->leaves[i]->neighbours[0];
            for (j = 0; j < nei->neiNum; j ++)
            {
                if (nei->neighbours[j]->pos == tmp2->leaves[i]->pos)
                {
                    break;
                }
            }
            nei->neighbours[j] = curnode;
            nodeAddNeighbour(curnode, nei, 0);
            tmp2->leaves[i]->neighbours[0] = curnode;
            nodeAddNeighbour(curnode, tmp2->leaves[i], 0);
            break;
        }
    }
    // link node from tmp1 to it
    for (i = 0; i < tmp1->leavesNum; i++)
    {
        if (strcmp(tmp1->leaves[i]->name, reference->leaves[found]->name) == 0)
        {
            nei = tmp1->leaves[i]->neighbours[0];
            leaf  = tmp1->leaves[i];
            for (j = 0; j < nei->neiNum; j++)
            {
                if (nei->neighbours[j]->pos == leaf->pos)
                {
                    break;
                }
            }
            nei->neighbours[j] = curnode;
            nodeAddNeighbour(curnode, nei, 0);
            break;
        }
    }
    // for node in tmp1: push node to tmp2 list, 
    // rebind pos, if leaf -- do same
    for (i = 0; i < tmp1->nodesNum; i++)
    {
        if (i != leaf->pos)
        {
            nei = tmp1->nodes[i];
            tmp2->nodes[tmp2->nodesNum] = nei;
            nei->pos = tmp2->nodesNum;
            tmp2->nodesNum++;
            if ((nei->name != 0) && (nei->neiNum == 1)) // a leaf
            {
                tmp2->leaves[tmp2->leavesNum] = nei;
                tmp2->leavesNum++;
            }
        }
    }
    // remove tmp1 manually, not deleting nodes themselves
    nodeDelete(leaf); // we only need to delete this leaf here?
    free(tmp1->nodes);
    free(tmp1->leaves);
    if (tmp1->lcaFinder != 0)
    {
        lcaFinderDelete(tmp1->lcaFinder);
    }
    free(tmp1);
    // copy tmp2 to result
    result =  treeCopy(tmp2, 0);
    treeLCAFinderCalculate(result);
    // free arrays
    free(permutation);
    free(pairIdx);
    free(leavesToKeepIdx);
    free(leavesToTransferIdx);
    for (i = 0; i < intree1->leavesNum; i++)
    {
        free(dist1[i]);
        free(dist2[i]);
        free(relDist[i]);
    }
    free(dist1);
    free(dist2);
    free(relDist);
    // free reference, rugged, tmp2
    treeDelete(reference);
    treeDelete(rugged);
    treeDelete(tmp2);
    return result;
} //fuseTrees

/*
int main()
{
    srand(time(NULL));
    char str1[] = "((((A,B),C),D),(E,F));";
    char str2[] = "((((A,C),F),B),(E,D));";
    char* newick1 = str1;
    char* newick2 = str2;
    printf("Trees ready to read\n");
    Tree* tree1 = treeFromNewick(newick1);
    Tree* tree2 = treeFromNewick(newick2);
    printf("Starting\n");
    Tree* result = fuseTrees(tree1, tree2);
    printf("printing result:\n");
    printf("%s\n", treeToString(result));
    treeDelete(tree1);
    treeDelete(tree2);
    treeDelete(result);
    return 0;
} 
*/