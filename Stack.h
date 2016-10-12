#ifndef STACK_H
#define STACK_H
#include <cstdio>
// Stack it says what it does ..
// the stack is constucted using one conneciton list of nodes
// where every node has data and pointer to the next node

// we keep pointer to the top of the stack so we can pop and push elements
// constructor to initialize the variables to default states
// push an elelemt pop and peek element
// we free the chain of nodes by poping each one of them
// and all of that in the header file because template is used
template <typename T>
class Stack
{
private:
	template<typename T>
	struct Node
	{
		T data;
		Node<T> *next;

		Node() : next(NULL) {}
		Node(const T & other) : data(other), next(NULL){}
	};
private:
	Node<T> * first;
	int length;
public:
	Stack() : length(0), first(NULL) {}
	~Stack(){ free(); }
	Stack(const Stack<T> & other)
		:length(0), first(NULL)
	{
		copy(other);
	}
	Stack& operator=(const Stack<T> & other)
	{
		if (this != &other)
		{
			length = 0;
			first = NULL;
			free();
			copy(other);
		}

		return *this;
	}

	void push(T el)
	{
		Node<T>* newElem = new Node<T>(el);
		newElem->next = first;
		first = newElem;
		++length;
	}
	void pop()
	{
		if (first)
		{
			Node<T> *tempE = first->next;
			delete first;
			first = tempE;
			--length;
		}
	}
	T& top()
	{
		if (first)
			return first->data;
		throw "no elements";
	}
	bool empty() const
	{
		if (first == NULL)
			return true;
		return false;
	}
	int getLength() const { return length; }
private:
	void copy(const Stack<T>& other)
	{
		if (!other.empty())
		{
			Node<T> * temp = new Node<T>(other.first->data);
			first = temp;

			Node<T>* theFirts = first;
			Node<T>* walking = other.first->next;

			while (walking != NULL)
			{
				Node<T>* tempy = new Node<T>(walking->data);
				theFirts->next = tempy;
				theFirts = tempy;
				walking = walking->next;
			}
			length = other.length;
		}
	}
	void free()
	{
		while (length)
			pop();
	}
};


#endif // STACK_H
