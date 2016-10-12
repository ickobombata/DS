#pragma once
#include "Cell.h"
#include "HeaderIncludes.h"

// this is the ending point 
// symbol for this element is not 
// needed cuz we are not going to print the maze
class EndPoint : public Cell
{
public:
	virtual ~EndPoint(){}
	virtual Cell* clone() const { return new EndPoint(*this); }
	virtual bool isPassable() const{ return false; } // this is not sure
	virtual void mapPrint(std::ostream& out) const
	{
		out << "X";
	}
};

