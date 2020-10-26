#ifndef MLIST_H
#define MLIST_H

#include <cstdlib>
#include <initializer_list>
#include <ostream>

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
struct Node
{
    Node(const T &d) : data(d), next(nullptr) {}

    T data;
    Node *next;
};

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
class MList
{
public:
    MList();
    MList(std::initializer_list<T> l);
    ~MList();

    size_t Size() const;

    int PushFront(const T &data);
    int PushBack(const T &data);

    template <typename ST>
    friend std::ostream & operator << (std::ostream &os, const MList<ST> &list);

private:

    Node<T> *NodeAllocate(const T &data) const {return new Node<T>(data);}
    void     NodeFree(Node<T> *node) const {delete node;}

    Node<T>  *head_;
};

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
MList<T>::MList() : head_(nullptr)
{

}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
MList<T>::MList(std::initializer_list<T> l) : head_(nullptr)
{
    for (auto &item : l)
    {
        PushBack(item);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
MList<T>::~MList()
{
    Node<T> *tmp;

    while (head_)
    {
        tmp = head_;
        head_ = head_->next;

        NodeFree(tmp);
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
size_t MList<T>::Size() const
{
    size_t size = 0;
    Node<T> *cur = head_;

    while (cur)
    {
        size++;
        cur = cur->next;
    }

    return size;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
int MList<T>::PushFront(const T &data)
{
    Node<T> *new_node = NodeAllocate(data);

    if (!new_node) return -1;

    new_node->next = head_;
    head_ = new_node;

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
int MList<T>::PushBack(const T &data)
{
    Node<T> *new_node, *cur;

    new_node = NodeAllocate(data);

    if (!new_node) return -1;

    if (!head_)
    {
        head_ = new_node;
    } else {
        cur = head_;

        while (cur->next)
            cur = cur->next;

        cur->next = new_node;
    }

    return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
std::ostream & operator << (std::ostream &os, const MList<T> &list)
{
    Node<T> *cur = list.head_;

    os << '[';

    while (cur)
    {
        os << cur->data;

        cur = cur->next;

        if (cur) os << ", ";
    }

    os << ']';

    return os;
}

/*--------------------------------------------------------------------------------------------------------------------*/


#endif // MLIST_H
