#pragma once
#include "Cell.h"
#include "HeaderIncludes.h"


// this is the starting point 
// symbol for this element is not 
// needed cuz we are not going to print the maze
class StartPoint : public Cell
{
public:
	virtual ~StartPoint(){}
	virtual Cell* clone() const { return new StartPoint(*this); }
	virtual bool isPassable() const{ return true; }
	virtual void mapPrint(std::ostream& out) const
	{
		out << 'S';
	}
};

