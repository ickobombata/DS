#pragma once

#include "Cell.h"
#include "HeaderIncludes.h"

// this is empty box or the whitespace 
// symbol is not needed cuz we are not 
// going to print the maze 
class Wall : public Cell
{
public:
	Wall(){}
	virtual ~Wall(){}
	virtual Cell* clone() const { return new Wall(*this); }
	virtual bool isPassable() const{ return false; }
	virtual void mapPrint(std::ostream& out) const
	{
		out << "#";
	}
};