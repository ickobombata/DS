#ifndef MLIST_H
#define MLIST_H
#include <cstdlib>
#include <iostream>

template <typename T>
struct LNode
{
    T data;
    LNode* next;
    LNode* prev;

    LNode(T el) : data(el) , next(NULL) , prev(NULL){}
};
// Double linked list using iterator
// has pointer to the first and last elemnt of the list can push pop
// and remove at a given position using iterator
template <typename T>
class MList
{
private:
    LNode<T>* first;
    LNode<T>* last;
    int length;

public:
    // default cconstructor to init the start variables
    MList() : first(NULL) , last(NULL) , length(0) {}
    // destructor to delete every element from the list
    ~MList() { deleteList(); }
public:
    //iterator to go in any way through the list
    class ListIterator
    {
        friend class MList<T>;
    private:
        LNode<T>* current;

    private:
        ListIterator(LNode<T>* c)
            :current(c)
        {}
    public:
        ListIterator& operator++()
        {
            if(current)
                current = current->prev;

            return *this;
        }
        ListIterator operator++(int)
        {
            ListIterator temp(*this);
            ++(*this);
            return temp;
        }
        ListIterator& operator--()
        {
            if(current)
                current = current->next;
            return *this;
        }
        ListIterator operator--(int)
        {
            ListIterator temp(*this);
            --(*this);
            return temp;
        }
        T& operator*()
        {
            if(!current)
                throw "Can't take the data out of Null Pointer";
            return current->data;
        }
        const T& operator*() const
        {
            if(!current)
                throw "Can't take the data out of Null Pointer";
            return current->data;
        }
        bool operator==(ListIterator other)
        {
            return current == other.current;
        }
        bool operator!=(ListIterator other)
        {
            return !(*this == other);
        }
        operator bool() const
        {
            return current != NULL;
        }
        bool isValid() const
        {
            return current != NULL;
        }
    };
    ListIterator begin() const
    {
        return ListIterator(first);
    }
    ListIterator end() const
    {
        return ListIterator(last);
    }

    void removeAt(ListIterator& iter)
    {
        if(isEmpty())
            throw "can't remove element from empty list";
        if(!iter)
            return;

        LNode<T>* temp = iter.current;

        if(temp == first)
            first = first->prev;
        if(temp == last)
            last = last->next;
        if(temp->next != NULL)
            temp->next->prev = temp->prev ;
        if(temp->prev != NULL)
            temp->prev->next = temp->next ;

        if(temp->prev)
            iter.current = temp->prev;

        temp->prev = NULL;
        temp->next = NULL;

        delete temp;

        length--;
    }
public:
    bool isEmpty() const
    {
        return first == NULL ;
    }
    int getLength()
    {
        return length;
    }
    void push_front(T data)
    {
        LNode<T>* newElem = new LNode<T>(data);

        if(!isEmpty())
        {
            newElem->prev = first;
            first->next = newElem;
        }
        else
        {
            last = newElem;
        }
        first = newElem;
        length++;
    }
    void push_back(T data)
    {
        LNode<T>* newElem = new LNode<T>(data);

        if(!isEmpty())
        {
            newElem->next = last;
            last->prev = newElem;
        }
        else
        {
            first = newElem;
        }
        last = newElem;
        length++;
    }
    T pop_front()
    {
        if(!isEmpty())
        {
            LNode<T>* newElem = first;

            T el = first->data;

            if(first == last)
                last = first->prev;

            first = first->prev;

            delete newElem;

            length--;
            return el;
        }
        else
        {
            throw "There are no elements in this list";
        }
    }
    T pop_back()
    {
        if(!isEmpty())
        {
            LNode<T>* newElem = last;

            T el = last->data;

            if(first == last)
                first = first->next;

            last = last->next;

            delete newElem;

            length--;
            return el;
        }
        else
            throw "There are no elements in this list";
    }

private:
    void deleteList()
    {
        while(!isEmpty())
        {
            pop_front();
        }
    }

    // it's not allowed to copy list or set list = something;
    // not needed right now (write them later)
    MList(const MList<T>& other);
    MList& operator=(const MList<T>& other);
};

#endif // MLIST_H
