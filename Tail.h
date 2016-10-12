#ifndef TAIL_H
#define TAIL_H
#include <cstdlib>

template <typename T>
struct Node
{
    T data;
    Node<T>* next;

    Node(T el) : next(NULL)
    {
        data = el;
    }
};
// just a regular tail
// with one linked Node
// we have the first and the last elelemt of the tail
template <typename T>
class Tail
{
private:
    Node<T>* first;
    Node<T>* last;
    int length ;
public:
    // default constructor to set the values to null
    Tail():first(NULL) , last(NULL) , length(0){}
    ~Tail()
    {
        freeTail();
    }
    // copy construcot and operator=
    Tail(const Tail& other)
        :first(NULL) , last(NULL) , length(0)
    {
        copyTail(other);
    }
    Tail& operator=(const Tail& other)
    {
        if(this != &other)
        {
            freeTail();
            copyTail(other);
        }
        return *this;
    }
    bool empty()
    {
        return first == NULL;
    }
    // add element
    void push(const T& el)
    {
        Node<T>* newEl = new Node<T>(el);

        if(empty())
        {
            first = newEl;
        }
        else
        {
            last->next = newEl;
        }
        last = newEl;
        length++;
    }
    // get the length of the tail
    int getLength() const
    {
        return this->length;
    }
    // get the first element
    T& front() const
    {
        if(first != NULL)
            return first->data ;
        else
            throw "No Elements inside tail";
    }
    // remove element
    T pop()
    {
        if(!empty())
        {
            Node<T>* oldElem = first;
            first = first->next;

            T x = oldElem->data;
            oldElem->next = NULL;
            delete  oldElem;

            length--;
            return x;
        }
        else
        {
            throw "No Elements inside tail";
        }
    }

    // takes sub tail from a given tail and returns in the result variable
    // ex. got tail of integers   1 2 3 4 5 6 7 8 9 we give an iterator to the
    // element 5 then call subTail -> this( 1 2 3 4 ) result(5 6 7 8 9)
    // if we pass the same tail it would do nothing
private:
    // copy tail from another tail
    void copyTail(const Tail& other)
    {
        if(other.getLength())
        {
            length = 0;
            Node<T>* temp = other.first ;
            do
            {
                push(temp->data);
                temp = temp->next;

            }while(temp != NULL);
        }
    }
    // dequeue every other element
    void freeTail()
    {
        while(!empty())
            pop();
        first = NULL;
        last = NULL;
    }
};

#endif // TAIL_H
