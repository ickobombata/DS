#pragma once

#include "Board.h"
#include "Cell.h"
#include "Tree.h"
#include "EmptyCell.h"
#include "Door.h"
#include "StartPoint.h"
#include "EndPoint.h"
#include "Key.h"
#include "Wall.h"

typedef std::pair<Cell*, size_t> box;
class Solver
{
public:
	Solver();
	~Solver();

	// it calls all the submethods and orginasies the algorightm 
	bool startAlgorithm(Board& b, std::string& result) const;

private:
	// front setted tree it returns the path that has minimum 
	// weight calculating the size_t from the box
	Stack<Cell*> getClosestPath(Tree<box>&) const;

	//
	std::string ToString(Board& pBoard, Stack<Cell*>&) const;
	std::string toString_It(Stack<box>& cells) const;

	// finds the path from one cell to another and returns the result 
	// in a stack
	Stack<box> bfsForCellToCell(Cell* from, Cell* to) const;

	// true if the boxes are neighbours
	bool boxesFromThePath(box a, box b) const;
	
	// returns a char R,U,L,D according to the points a and b 
	char determineDirection(box a, box b) const;

	// we check the four neighbours if they are passible or important element
	void NeighboursCheck(Tail<box>& qu, Stack<box>& st) const;
	
	// this is the BFS part to find every important element in a room	
	void littleBFS(Stack<box>&) const;
	
	// it's a DFS for the important elements like DOOR , KEY , START and END
	bool recursiveDFS(Board& b, Tree<box>& tree, Tree<box>::BottomUpIterator, box item) const;

	// looks in a tree using the BootomUpIterator if the given cell
	// is allready been looked up
	bool containCellAsParent(Tree<box>::BottomUpIterator, Cell* c) const;

	// looks in a tree using the BootomUpIterator if there is a 
	// key that can open the given door
	bool canBeOpened(Tree<box>::BottomUpIterator, Door* d) const;
};

