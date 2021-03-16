#include "bintree.h"


BinTree::BinTree(std::initializer_list<int> list)
{
    (void)list;
}

//====================================================================================================================//

BinTree::BinTree(const BinTree &tree)
{
    (void)tree;
}

//====================================================================================================================//

int NodeAlloc(int data, IntTreeNode **node)
{
    (void)data;
    (void)node;
    return -1;
}

//====================================================================================================================//

void BinTree::NodeFree(IntTreeNode *node)
{
    (void)node;
}

