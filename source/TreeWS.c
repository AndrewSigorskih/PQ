#include "TreeWS.h"

TreeWithScore* treeWithScoreCreate(Tree* tree, INT score)
{
    TreeWithScore* treeWS;
    treeWS = (TreeWithScore*)malloc(sizeof(TreeWithScore));
    treeWS->tree = tree;
    treeWS->score = score;
    return treeWS;
}

void treeWithScoreDelete(TreeWithScore* treeWS)
{
    treeDelete(treeWS->tree);
    free(treeWS);
}

int treeWithScoreCompare(const void* treeWS1, const void* treeWS2)
{
    return (*(TreeWithScore**)treeWS1)->score - (*(TreeWithScore**)treeWS2)->score;
}

void treeWithScoreSort(TreeWithScore** treeWSArray, size_t size)
{
    qsort(treeWSArray, size, sizeof(TreeWithScore*), treeWithScoreCompare);
    return;
}

void treesWrite(TreeWithScore** trees, char* outFileName, int n)
{
    FILE* outFile;
    char* result; 
    int i;
    int j;
    int k;

    if(strcmp(outFileName, "stdout") == 0)
    {
        outFile = stdout;
    }
    else 
    {
        outFile = fopen(outFileName, "w");
        if(outFile == NULL)
        {
            fprintf(stderr, "Cannot open %s for writing\n", outFileName);
            exit(1);
        }
    }

    for (k = 0; k < n; k++)
    {
        i = 0;
        j = 0;
        result = treeToString(trees[k]->tree);
        
        while ( result[i] != '\0') {
            if ( j > 60 && result[i] == ',' ) 
            {
                putc(result[i], outFile);
                putc('\n', outFile);
                j = 0;
            }
            else 
            {
                putc(result[i], outFile);
            }
            i++;
            j++;
        }
        putc('\n', outFile);
        free(result);
    }
    fclose(outFile);
    return;
} /* treesWrite */
