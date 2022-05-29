#ifndef _GENITOR_H_
#define _GENITOR_H_
#include "maxAgreement.h"
#include "Tree.h"
#include "TreeWS.h"
#include "countScore.h"
#include "growTree.h"
#include "fuseTrees.h"

unsigned treeIsUnique(TreeWithScore* intree, int pos, TreeWithScore** trees, unsigned treeNum);
TreeWithScore* umast_cross(TreeWithScore* tree1, TreeWithScore* tree2,\
			HashAlignment* alignment, int alpha,\
			 GapOpt gapOpt, PWM* pwmMatrix, INT**** hashScore);
TreeWithScore* fuse_cross(TreeWithScore* tree1, TreeWithScore* tree2,\
			HashAlignment* alignment, int alpha,\
			 GapOpt gapOpt, PWM* pwmMatrix, INT**** hashScore);
TreeWithScore* crossover(TreeWithScore* tree1, TreeWithScore* tree2, HashAlignment* alignment,\
		int alpha, GapOpt gapOpt, PWM* pwmMatrix, INT**** hashScore, unsigned char crossType);
TreeWithScore* genitor(TreeWithScore** trees, unsigned treeNum, HashAlignment* alignment,\
					int alpha, GapOpt gapOpt, PWM* pwmMatrix, INT**** hashScore, unsigned iterNum, \
					unsigned iterNew, unsigned iterLim, unsigned char crossType);
#endif