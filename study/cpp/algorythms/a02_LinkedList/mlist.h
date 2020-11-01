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
    MList(int size);
    MList(std::initializer_list<T> l);
    MList(const MList<T> &list);
    ~MList();

    size_t Size() const;

    int  PushFront(const T &data);
    int  PushBack(const T &data);
    bool Find(const T &data) const;
    int  Erase(const T &data);
    T    GetMiddle() const;
    MList<T> &Reverse();

    template <typename ST>
    friend std::ostream & operator << (std::ostream &os, const MList<ST> &list);

    MList<T> &operator =(const MList<T> &list);

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
MList<T>::MList(int size) : head_(nullptr)
{
    while (size--)
    {
        PushBack((rand() % 50 - 25));
    }
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
MList<T>::MList(const MList<T> &list) : head_(nullptr)
{
    Node<T> *cur = list.head_;

    while (cur)
    {
        PushBack(cur->data);
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
bool MList<T>::Find(const T &data) const
{
    Node<T> *cur = head_;

    while (cur)
    {
        if (cur->data == data)
            return true;

        cur = cur->next;
    }

    return false;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
int MList<T>::Erase(const T &data)
{
    Node<T> *prev, *cur = head_;
    int res = -1;

    if (!head_) goto _exit;

    if (head_->data == data)
    {
        cur = head_;
        head_ = head_->next;

        NodeFree(cur);
        res = 0;
        goto _exit;
    }

    cur = head_->next;
    prev = head_;

    while (cur)
    {
        if (cur->data == data)
        {
            prev->next = cur->next;
            NodeFree(cur);
            res = 0;
            goto _exit;
        }

        prev = cur;
        cur = cur->next;
    }

_exit:
    return res;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template<typename T>
T MList<T>::GetMiddle() const
{
    int mid_idx = (Size() / 2) + 1;
    Node<T> *cur = head_;

    if (!cur) return 0;

    if (!cur->next) return cur->data;

    while (--mid_idx)
    {
        cur = cur->next;
    }

    return cur->data;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template <typename T>
MList<T> &MList<T>::Reverse()
{
    Node<T> *prev, *cur, *next;

    if (!head_) goto _exit;

    cur = head_->next;
    prev = head_;
    prev->next = nullptr;

    while (cur)
    {
        next = cur->next;
        cur->next = prev;

        prev = cur;
        cur = next;
    }

    head_ = prev;

_exit:
    return *this;
}

/*--------------------------------------------------------------------------------------------------------------------*/

template<typename T>
MList<T> &MList<T>::operator =(const MList<T> &list)
{
    Node<T> *cur;

    if (this == &list)
        return *this;

    while (!Erase(head_->data));

    cur = list.head_;

    while (cur)
    {
        PushBack(cur->data);
        cur = cur->next;
    }

    return *this;
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
