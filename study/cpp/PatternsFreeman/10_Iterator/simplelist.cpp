#include "simplelist.h"

//====================================================================================================================//

SimpleList::SimpleList() : head_(nullptr)
{
}

//====================================================================================================================//

SimpleList::SimpleList(int size) : head_(nullptr)
{
    while (size--)
    {
        PushBack(SimpleList::NodeAlloc(rand() % 50 - 25));
    }
}

//====================================================================================================================//

SimpleList::SimpleList(std::initializer_list<int> init_list) : head_(nullptr)
{
    for (const int &item : init_list)
    {
        PushBack(SimpleList::NodeAlloc(item));
    }
}

//====================================================================================================================//

SimpleList::SimpleList(const SimpleList &list) : head_(nullptr)
{
    IntNode *node = list.head_;

    while (node)
    {
        PushBack(SimpleList::NodeAlloc(node->data));
        node = node->next;
    }
}

//====================================================================================================================//

SimpleList::~SimpleList()
{
    while (head_)
        Erase(head_);
}

//====================================================================================================================//

size_t SimpleList::Size() const
{
    IntNode *node;
    size_t size;

    for (size = 0, node = head_; node; node = node->next)
    {
        size++;
    }

    return size;
}

//====================================================================================================================//

int SimpleList::PushFront(IntNode *node)
{
    int res = -1;

    if (node)
    {
        node->next = head_;
        head_ = node;
        res = 0;
    }

    return res;
}

//====================================================================================================================//

int SimpleList::PushBack(IntNode *node)
{
    int res = -1;

    if (node)
    {
          if (head_)
          {
              IntNode *p = head_;

              while (p->next)
                  p = p->next;

               p->next = node;
          } else {
              head_ = node;
          }
    }

    return res;
}

//====================================================================================================================//

IntNode *SimpleList::Find(const int &d) const
{
    IntNode *p = head_;

    while (p)
    {
        if (p->data == d)
            break;

        p = p->next;
    }

    return p;
}

//====================================================================================================================//

int SimpleList::Remove(IntNode *node)
{
    int res = -1;

    if (node)
    {
        if (head_ == node)
        {
            head_ = node->next;
            node->next = nullptr;
            res = 0;
        } else {
            IntNode *p = head_;

            while (p)
            {
                if (p->next == node)
                {
                    p->next = node->next;
                    node->next = nullptr;
                    res = 0;
                    break;
                }

                p = p->next;
            }
        }
    }

    return res;
}

//====================================================================================================================//

int SimpleList::Erase(IntNode *node)
{
    int res = -1;

    if (!Remove(node))
    {
        SimpleList::NodeFree(node);
        res = 0;
    }

    return res;
}

//====================================================================================================================//

IntNode *SimpleList::GetMiddle() const
{
    int mid_idx = (Size() / 2) + 1;
    IntNode *cur = head_;

    if (!cur) return 0;

    if (!cur->next) return cur;

    while (--mid_idx)
    {
        cur = cur->next;
    }

    return cur;
}

//====================================================================================================================//

SimpleList &SimpleList::Reverse()
{
    IntNode *prev = nullptr, *cur = nullptr, *next;

    if (Size() < 2)
        goto _exit;

    prev = head_;
    cur = head_->next;
    prev->next = nullptr;

    do
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    } while(cur);

    head_ = prev;

_exit:
    return *this;
}

//====================================================================================================================//

IntNode *SimpleList::Head() const
{
    return head_;
}

//====================================================================================================================//

SimpleList &SimpleList::operator =(const SimpleList &list)
{
    IntNode *node = list.head_;

    while (head_)
        Erase(head_);

    while (node)
    {
        PushBack(SimpleList::NodeAlloc(node->data));
        node = node->next;
    }

    return *this;
}

//====================================================================================================================//

std::ostream & operator << (std::ostream &os, const SimpleList &list)
{
    IntNode *node = list.head_;
    os << "[";

    while (node)
    {
        os << node->data;

        node = node->next;

        if (node)
            os << ", ";
    }

    os << "]";

    return os;
}
