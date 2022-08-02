#ifndef ITERATOR_H
#define ITERATOR_H

#include <initializer_list>

#include "simplelist.h"

class Iterator
{
public:
    virtual bool HasNext() = 0;
    virtual void Begin() = 0;
    virtual void Next() = 0;
    virtual int  Current() = 0;
    virtual ~Iterator() {}
};

/*====================================================================================================================*/

class IAgregate
{
public:
    virtual Iterator *CreateIterator() = 0;
};

/*====================================================================================================================*/

class List : public IAgregate
{
    SimpleList _list;

    friend class ListIterator;
public:
    List(std::initializer_list<int> init_list)
    {
        for (const int &item : init_list)
        {
            _list.PushBack(SimpleList::NodeAlloc(item));
        }
    }

    Iterator *CreateIterator() override;

};

/*====================================================================================================================*/

class Array : public IAgregate
{
    int *_array;
    int _size;

    friend class ArrayIterator;
public:
    Array(std::initializer_list<int> init_list)
    {
        int i = 0;

        _size = init_list.size();
        _array = new int[_size];

        for (const int &item : init_list)
        {
            _array[i++] = item;
        }
    }

    ~Array()
    {
        delete [] _array;
    }

    Iterator *CreateIterator() override;
};

/*====================================================================================================================*/

class ListIterator : public Iterator
{
    List *_list;
    IntNode *_current;

public:
    ListIterator(List *list) : _list(list), _current(nullptr)
    {
    }

    void Begin()
    {
        _current = _list->_list.Head();
    }

    bool HasNext()
    {
        if (_current && _current->next)
            return true;
        else
            return false;
    }

    void Next()
    {
        _current = _current->next;
    }

    int Current()
    {
        return _current->data;
    }
};

/*====================================================================================================================*/

class ArrayIterator : public Iterator
{
    Array *_array;
    int _current;

public:
    ArrayIterator(Array *array) : _array(array), _current(0)
    {
    }

    void Begin()
    {
        _current = 0;
    }

    bool HasNext()
    {
        if (_current + 1 < _array->_size)
            return true;
        else
            return false;
    }

    void Next()
    {
        _current++;
    }

    int Current()
    {
        return _array->_array[_current];
    }
};

/*====================================================================================================================*/


Iterator *List::CreateIterator()
{
    return new ListIterator(this);
}

/*====================================================================================================================*/

Iterator *Array::CreateIterator()
{
    return new ArrayIterator(this);
}

#endif // ITERATOR_H
