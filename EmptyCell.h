#pragma once

#include "Cell.h"
#include "HeaderIncludes.h"
// this is empty box or the whitespace 
// symbol is not needed cuz we are not 
// going to print the maze 
class EmptyCell : public Cell
{
public:
	virtual ~EmptyCell(){}
	virtual Cell* clone() const { return new EmptyCell(*this); }
	virtual bool isPassable() const{ return true; }
	virtual void mapPrint(std::ostream& out) const
	{
		out << " ";
	}
};