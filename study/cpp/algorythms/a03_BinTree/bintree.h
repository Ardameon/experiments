#ifndef BINTREE_H
#define BINTREE_H

#include <initializer_list>

struct IntTreeNode
{
    IntTreeNode(const int &data) : data_(data), parent_(nullptr), left_(nullptr), right_(nullptr) {}

    int data_;
    IntTreeNode *parent_;
    IntTreeNode *left_;
    IntTreeNode *right_;
};

class BinTree
{
public:
    BinTree() : root_(nullptr) {}
    BinTree(std::initializer_list<int> list);
    BinTree(const BinTree &tree);

    static int NodeAlloc(int data, IntTreeNode **node);
    static void NodeFree(IntTreeNode *node);

private:
    IntTreeNode *root_;
};

#endif // BINTREE_H
