#ifndef TREE_H
#define TREE_H
#include "MList.h"
#include <cstdlib>
#include <iomanip>
#include <stack>
#include "queue"

template <typename T>
struct NodeData
{
    T data;
    size_t level;

    NodeData(T d , size_t l) : data(d) , level(l){}
};

template <typename T>
class Tree
{
    struct Node
    {
        T data;
        MList<Node*> children;
		Node* parent;
        Node(T el) : parent(NULL) , data(el) {}
    };

typedef NodeData<Node*> node;

public:
    //iterators
    // iterate by length hierarchy
    class HierarchyIterator
    {
        friend class Tree<T>;
    private:
        std::queue<Node*> nodes; //
        HierarchyIterator(Node* curr){
            nodes.push(curr);
        }
    public:
        HierarchyIterator& operator++()
        {
            Node* temp = nodes.front();
            nodes.pop();

            for(typename MList<Node*>::ListIterator i = temp->children.begin() ; i ; i++)
                nodes.push(*i);
			return nodes.front();
        }
        HierarchyIterator operator++(int)
        {
            HierarchyIterator temp(*this);
            ++(*this);
            return temp;
        }
        T& operator*()
        {
            if(!nodes.empty())
                return nodes.front()->data;
            throw "Can't take the data out of NULL pointer";
        }
        T& operator*() const
        {
            if(!nodes.empty())
                return nodes.front()->data;
            throw "Can't take the data out of NULL pointer";
        }
        bool operator==(HierarchyIterator other)
        {
            return nodes.front() == other.nodes.front();
        }
        bool operator != (HierarchyIterator other)
        {
            return !(*this == other);
        }
        operator bool() const
        {
            return !nodes.empty();
        }
    };
    HierarchyIterator hierarchyBegin()
    {
        return HierarchyIterator(root);
    }
    // iterate by depth
    class DepthIterator
    {
        friend class Tree<T>;
    private:
        std::stack<Node*> nodes;
        DepthIterator(Node* current){
            nodes.push(current);
        }
    public:
        DepthIterator& operator++()
        {
            Node* temp = nodes.top();
            nodes.pop();

            for(typename MList<Node*>::ListIterator i = temp->children.end() ; i ; i--)
                nodes.push(*i);

			return *this;
        }
        DepthIterator operator++(int)
        {
            DepthIterator temp(*this);
            ++(*this);
            return temp;
        }
        T& operator*()
        {
            if(!nodes.empty())
            {
                return nodes.top()->data;
            }
            throw "Can't take the data out of NULL pointer";
        }
        T& operator*() const
        {
			if (!nodes.empty())
			{
				return nodes.top()->data;
			}
            throw "Can't take the data out of NULL pointer";
        }
        bool operator==(DepthIterator other)
        {
            return nodes.top() == other.nodes.top();
        }
        bool operator != (DepthIterator other)
        {
            return !(*this == other);
        }
        operator bool() const
        {
            return !nodes.empty();
        }
    };
    DepthIterator depthBegin() const
    {
        return DepthIterator(root);
    }
	class BottomUpIterator
	{
		friend class Tree < T > ;
	private:
		Node* node;
		BottomUpIterator(Node* di){
			node = di;
		}
	public:
		BottomUpIterator& operator++()
		{
			node = node->parent;
			return *this;
		}
		BottomUpIterator operator++(int)
		{
			BottomUpIterator temp(*this);
			++(*this);
			return temp;
		}
		T& operator*()
		{
			if (node)
			{
				return node->data;
			}
			throw "Can't take the data out of NULL pointer";
		}
		T& operator*() const
		{
			if (node)
			{
				return node->data;
			}
			throw "Can't take the data out of NULL pointer";
		}
		bool operator==(BottomUpIterator other)
		{
			return node == other.node;
		}
		bool operator != (BottomUpIterator other)
		{
			return !(*this == other);
		}
		operator bool() const
		{
			return node != NULL;
		}
	};
	BottomUpIterator bottomUpBegin() const
	{
		return BottomUpIterator(root);
	}
	BottomUpIterator bottomUpFrom(DepthIterator& iter) const
	{
		return BottomUpIterator(iter.nodes.top());
	}
private:
    Node* root;
public:
    Tree(): root(NULL){}
    ~Tree(){ if(root != NULL) deleteTree(root);}
    bool empty() const
    {
        return root == NULL;
    }
    void setTree(T theElement)
    {
        if(root == NULL)
        {
            Node* newNode = new Node(theElement);

            root = newNode;
        }
    }
    void insertElement(T inElement , T theElement)
    {
        insertAt(root , inElement , theElement);
    }
	BottomUpIterator insertElement(BottomUpIterator& i, T theElement)
    {
		if (i)
		{
			Node* newNode = new Node(theElement);
			newNode->parent = i.node;

			i.node->children.push_front(newNode);
			return BottomUpIterator(newNode);
		}
		else
			return i;
    }
    void deleteElement(T el)
    {
        if(root != NULL)
        {
            deleteIt(root , el);
        }
    }
    void printTreeRLR() const
    {
        if(root != NULL)
            printRLR(root , 0);
        else
            std::cout << "EMPTY" << std::endl;
    }
private:
    bool deleteIt(Node*& n , T& el)
    {
        if(n->data == el)
        {
           deleteTree(n);
           return true;
        }
        else
        {
            for(typename MList<Node*>::ListIterator i = n->children.begin() ; i ; i++)
            {
                 if(deleteIt((*i) , el))
                 {
                     n->children.removeAt(i);
                     i--;
                 }
            }
        }
        return false;
    }
    void printRLR(Node* n , size_t offset) const
    {
		std::cout << std::setw(offset);
		n->data.first->mapPrint(std::cout);
        for(typename MList<Node*>::ListIterator i = n->children.begin() ; i ; i++)
        {
            printRLR((*i) , offset);
        }
        //NOT NOT
        //std::cout <<std::setw(offset) << "" <<"</" <<  (n->data).getName() << ">" << std::endl;
    }
    bool insertAt(Node* at , T inElement , T theElement)
    {
        if(at->data == inElement)
        {
            Node* newNode = new Node(theElement);
			newNode->parent = at;
            at->children.push_back(newNode);

            return true;
        }
        else if(!at->children.isEmpty())
        {
            for(typename MList<Node*>::ListIterator i = at->children.begin() ; i ; i++)
            {
                if(insertAt((*i) , inElement , theElement) )
                    break;
            }
        }
        return false;
    }
    void deleteTree(Node*& n)
    {
        for(typename MList<Node*>::ListIterator i = n->children.begin() ; i ; i++)
            deleteTree(*i);

        delete n ;
        n = NULL;
    }
};

#endif // TREE_H
