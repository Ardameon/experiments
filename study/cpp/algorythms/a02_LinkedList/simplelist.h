#ifndef SIMPLELIST_H
#define SIMPLELIST_H


#include <cstdlib>
#include <initializer_list>
#include <ostream>

/*--------------------------------------------------------------------------------------------------------------------*/

struct IntNode
{
    IntNode(const int &d) : data(d), next(nullptr) {}

    int data;
    IntNode *next;
};

/*--------------------------------------------------------------------------------------------------------------------*/

class SimpleList
{
public:
    SimpleList();
    SimpleList(int size);
    SimpleList(std::initializer_list<int> init_list);
    SimpleList(const SimpleList &list);
    ~SimpleList();

    size_t Size() const;

    int      PushFront(IntNode *node);
    int      PushBack(IntNode *node);
    IntNode *Find(const int &d) const;
    int      Remove(IntNode *node);
    int      Erase(IntNode *node);
    IntNode *GetMiddle() const;
    SimpleList &Reverse();

    friend std::ostream & operator << (std::ostream &os, const SimpleList &list);

    SimpleList &operator =(const SimpleList &list);

    static IntNode *NodeAlloc(int data) { return new IntNode(data); }
    static void     NodeFree(IntNode *node) { if (node) delete node; }

private:
    IntNode  *head_;
};

#endif // SIMPLELIST_H
