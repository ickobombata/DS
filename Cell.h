#pragma once

class Cell;
#include "HeaderIncludes.h"
// Cell is a box and can contain ........

#include "Board.h"

class Cell
{
private:
	friend class Board;
	Point point;

	bool visited;

	Board* owner;
public:
	virtual ~Cell(){}

    // clone in order to duplicate the things inside the box
	virtual Cell* clone() const = 0;
    // mapPrint show how the thing inside the box is drawn on the screen
    virtual void mapPrint(std::ostream& out) const = 0;
public:
	Cell(Board* owner = NULL,int x = 0, int y = 0);
	Point getCoordinates() const;
	void setCoordinates(int x, int y);
	virtual bool isPassable() const = 0; // is passible ? but by whom 
	void MarkCell();
	void UnMarkCell();
	bool isMarked() const;
	void setOwner(Board* b);
	Cell* getLeftNeighbour() const;
	Cell* getRightNeighbour() const;
	Cell* getTopNeighbour() const;
	Cell* getBottomNeighbour() const;
	
	// they are four neighbours or less
	void getNeighbours(Tail< Cell* >& result) const;
};

// end

