#include "BranchArray.h"

BranchArray* branchArrayCreate(unsigned startSize)
{
    BranchArray* ba = malloc(sizeof(BranchArray));
    ba->array = calloc(startSize, sizeof(Branch*));
    ba->maxSize = startSize;
    ba->size = 0;
    return ba;
}

void branchArrayDelete(BranchArray* ba)
{
    //int i = 0;
    free(ba->array);
    free(ba);
}

void branchArrayAdd(BranchArray* ba, Branch* branch)
{
    if (ba->size && (ba->array[0]->size != branch->size))
    {
        fprintf(stderr, "branchArrayAdd: Branches are not of the same size\n");
        exit(1);

/*        raiseError("Branches are not of the same size", __FILE__, __FUNCTION__, __LINE__);*/
    }

    if (ba->size == ba->maxSize)
    {
        ba->maxSize = ba->maxSize * 3 / 2 + 1;
        ba->array = realloc(ba->array, sizeof(Branch*) * ba->maxSize);
    }
    ba->array[ba->size++] = branch;
}



void branchArrayExtend(BranchArray* dest, BranchArray* source)
{
    unsigned newSize = dest->size + source->size;
    if (newSize >= dest->maxSize)
    {
        dest->maxSize = newSize * 3 / 2 + 1;
        dest->array = realloc(dest->array, sizeof(Branch*) * dest->maxSize);
    }

    memcpy(dest->array + dest->size, source->array, source->size * sizeof(Branch*));
    dest->size += source->size;
}

void branchArraySort(BranchArray* ba)
{
    qsort(ba->array, ba->size, sizeof(Branch*), vBranchCompare);
    return;
}