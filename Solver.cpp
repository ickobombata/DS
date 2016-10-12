#include "Solver.h"

#include "EmptyCell.h"
#include "Door.h"
#include "StartPoint.h"
#include "EndPoint.h"
#include "Key.h"
#include "Wall.h"

Solver::Solver()
{
}
Solver::~Solver()
{
}

Stack<Cell*> Solver::getClosestPath(Tree<box>& tree) const
{
	Tree<box>::DepthIterator dIter = tree.depthBegin();
	size_t min = -1;
	Tree<box>::BottomUpIterator buIter = tree.bottomUpBegin();

	while (dIter)
	{
		EndPoint* end = dynamic_cast<EndPoint*>((*dIter).first);
		if (end)
		{
			if (min < 0 || min > (*dIter).second)
			{
				min = (*dIter).second;
				buIter = tree.bottomUpFrom(dIter);
			}
		}
		dIter++;
	}
	Stack<Cell*> result;
	
	while (buIter)
	{
		result.push((*buIter).first);
		buIter++; // going upwards
	}

	return result;
}

void Solver::NeighboursCheck(Tail<box>& qu, Stack<box>& st) const
{
	box workable = qu.front();
	qu.pop();
	workable.first->MarkCell();

	Tail<Cell*> neighbours;
	workable.first->getNeighbours(neighbours);
	box temp;
	while (!neighbours.empty())
	{
		Cell* el = neighbours.front();
		neighbours.pop();
		if (el->isMarked())
			continue;
		el->MarkCell();

		temp.first = el;
		temp.second = workable.second + 1;

		if (el->isPassable())
		{
			// if the elemetns is passible then we add it to the queue and then see what can we do with him
			qu.push(temp);
		}
		else
		{
			// else the elements can be end point then we pop everytnig from the queue and push the end to the stack
			// so we are practicly done 
			EndPoint* theEnd = dynamic_cast<EndPoint*>(el);
			if (theEnd)
			{
				while (!qu.empty())
					qu.pop();
				st.push(temp);
				return;
			}
			else
			{
				// if the elements is a key or a door 
				// then we push it to the stack 
				Key* theKey = dynamic_cast<Key*>(el);
				Door* theDoor = dynamic_cast<Door*>(el);
				if (theKey || theDoor)
				{
					if (theDoor)
						theDoor->setCameFrom(workable.first);
					st.push(temp);
				}
			}
		}
	}
}
void Solver::littleBFS(Stack<box>& globalStack) const
{
	box workable = globalStack.top();
	globalStack.pop();
	Tail<box> roomCells;
	roomCells.push(workable);
	// for the first element in the globalStack trying to find the room important Elements
	while (!roomCells.empty())
	{
		NeighboursCheck(roomCells, globalStack);
	}
}
bool Solver::recursiveDFS(Board& pBoard, Tree<box>& tree, Tree<box>::BottomUpIterator iter, box item) const
{
	// for every important element KEY START and DOOR
	// we make the DFS 
	// from every key to it's door and so on
	Stack<box> nextIterations;
	nextIterations.push(item);
	bool endReachedFlag = false;

	pBoard.unMarkCells();
	Door* tempyDoor = dynamic_cast<Door*>(item.first);
	
	if (tempyDoor)
		tempyDoor->getCameFrom()->MarkCell();
	
	// the little BFS is room searching 
	// it finds if there are any important elements in a door
	littleBFS(nextIterations);

	// it becomes complexed because there are keys and door and walls ..
	while (!nextIterations.empty())
	{
		box temp = nextIterations.top();
		nextIterations.pop();

		// we have reached an and 
		EndPoint* theEnd = dynamic_cast<EndPoint*>(temp.first);
		if (theEnd)
		{
			tree.insertElement(iter, temp);
			return true;
		}

		Key* theKey = dynamic_cast<Key*>(temp.first);
		Door* theDoor = dynamic_cast<Door*>(temp.first);

		// we have new key so we add it in the iterator and call the same method for him
		if ( theKey && !containCellAsParent(iter , temp.first))
	 	{/*
			if (theKey)
				std::cout << "Adding Key at " << theKey->getCoordinates().getX() << " , " << theKey->getCoordinates().getY()<<") "<< temp.second;
			if (theDoor)
				std::cout << "Adding door at " << theDoor->getCoordinates().getX() << " , " << theDoor->getCoordinates().getY() << ") " << temp.second;
			(*iter).first->mapPrint(std::cout);
			std::cout << "  " << (*iter).first->getCoordinates().getX() << " , " <<(*iter).first->getCoordinates().getY() << std::endl;
			*/
			Tree<box>::BottomUpIterator newIter = tree.insertElement(iter, temp);
			if (recursiveDFS(pBoard, tree, newIter, temp))
				endReachedFlag = true;
		}
		else if (theDoor )
		{
			if (containCellAsParent(iter, temp.first))
			{
				// if we have the door as Parent that means that door is opened
				// so we can go through the door in the adjencent room
				nextIterations.push(temp);
				littleBFS(nextIterations);
			}
			else
			{
				// else we add the door to the tree and try to add another paths through that door
				if (canBeOpened(iter, theDoor))
				{
					Tree<box>::BottomUpIterator newIter = tree.insertElement(iter, temp);
					if (recursiveDFS(pBoard, tree, newIter, temp))
						endReachedFlag = true;
				}
			}
		}
	}
	return endReachedFlag;
}
bool Solver::startAlgorithm(Board& b, std::string& result) const
{
	Tree<box> tree;
	std::pair<Cell*, size_t> start;
	start.first = b.getStart();
	start.second = 0;

	// if we have starting point the algorightm starts
	if (start.first)
	{
		// sets the tree up and calls the recursiveDFS which is a DFS algorithm
		tree.setTree(start);
		Tree<box>::BottomUpIterator iter = tree.bottomUpBegin();
		if (recursiveDFS(b, tree, iter, start))
		{
			// if true there is a path 
			// we take the closestPath
			Stack<Cell*> path = getClosestPath(tree);
			result = ToString(b, path);
			return true;
		}
	}
	return false;
}
bool Solver::containCellAsParent(Tree<box>::BottomUpIterator bui, Cell* c) const
{
	while (bui)
	{
		if ((*bui).first == c)
			return true;
		bui++;
	}
	return false;
}
bool Solver::canBeOpened(Tree<box>::BottomUpIterator bui, Door* d) const
{
	while (bui)
	{
		Key* k = dynamic_cast<Key*>((*bui).first);
		if (k && k->canOpen(*d))
			return true;
		bui++;
	}
	return false;
}

std::string Solver::ToString(Board& pBoard, Stack<Cell*>& path) const
{
	std::string result;
	Stack<box> countedPath;

	while (!path.empty())
	{
		Cell* a = path.top();
		path.pop();

		if (path.empty())
			break; // only the end has left

		Cell* b = path.top();
		pBoard.unMarkCells();
		countedPath = bfsForCellToCell(a, b);
		result += toString_It(countedPath);
	}

	return result; 
}
///// neeeed to set the doors opened when i pass throw them
std::string Solver::toString_It(Stack<box>& cells) const
{
	std::string result = "";
	
	while (!cells.empty())
	{
		box a = cells.top();
		cells.pop();
		if (cells.empty())
			break;

		box b = cells.top();

		result += determineDirection(a, b);
	}
	return result;
}

Stack<box> Solver::bfsForCellToCell(Cell* from, Cell* to) const
{
	Tail<box> qu;
	Stack<box> out;

	qu.push(box(from, 0));
	out.push(box(from, 0));

	while (!qu.empty())
	{
		box x = qu.front();
		qu.pop();

		x.first->MarkCell();
		Tail<Cell*> neig;
		x.first->getNeighbours(neig);

		while (!neig.empty())
		{
			Cell* n = neig.front();
			neig.pop();
			if (n->isMarked())
				continue;
			n->MarkCell();

			if (n == to) // we found the element we were looking for
			{
				Door* thisIsDoor = dynamic_cast<Door*>(n);
				if (thisIsDoor)
					thisIsDoor->open();

				out.push(box(n, x.second + 1));
				while (!qu.empty())
					qu.pop();
				break;
			}
			if (n->isPassable())
			{
				qu.push(box(n, x.second + 1));
				out.push(box(n, x.second + 1));
			}
			Key* theKey = dynamic_cast<Key*>(n);
			if (theKey)
			{
				qu.push(box(n, x.second + 1));
				out.push(box(n, x.second + 1));
			}
		}
	}
	Stack<box> result;
	result.push(out.top());
	out.pop();

	while (!out.empty())
	{
		box a = result.top();
		box b = out.top();
		out.pop();

		if (boxesFromThePath(a, b))
			result.push(b);
	}
	return result;
}
bool Solver::boxesFromThePath(box a, box b) const
{
	if (std::abs((int)a.second - (int)b.second <= 1))
	{
		if (a.first->getCoordinates().getX() == b.first->getCoordinates().getX() &&
			std::abs((double)a.first->getCoordinates().getY() - (double)b.first->getCoordinates().getY()) <= 1)
			return true;
		if (a.first->getCoordinates().getY() == b.first->getCoordinates().getY() &&
			std::abs((double)a.first->getCoordinates().getX() - (double)b.first->getCoordinates().getX()) <= 1)
			return true;
	}
	return false;
}

char Solver::determineDirection(box a, box b) const
{
	if (a.first->getCoordinates().getX() == b.first->getCoordinates().getX())
	{
		if (a.first->getCoordinates().getY() < b.first->getCoordinates().getY())
			return 'R';
		else
			return 'L';
	}
	else if (a.first->getCoordinates().getY() == b.first->getCoordinates().getY())
	{
		if (a.first->getCoordinates().getX() < b.first->getCoordinates().getX())
			return 'D';
		else
			return 'U';
	}
	return 'E'; // NOT NOT
}