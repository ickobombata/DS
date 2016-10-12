#pragma once
#include "Cell.h"
#include "HeaderIncludes.h"
// Door is a Box 
// it has state opened or closed
// and we can open the door with a key and so on
class Door : public Cell
{
private:
	char doorSymbol;
	bool state;
	Cell* cameFrom;
public:
	virtual ~Door(){}
	virtual Cell* clone() const { return new Door(*this); }
	
	bool isOpen() const { return state; }
	void open() { state = true; }
	void close() { state = false; }
	virtual bool isPassable() const
	{
		if (state)
			return true;
		return false;
	}
	virtual void mapPrint(std::ostream& out) const
	{
		out << doorSymbol;
	}
	void setSymbol(char sym)
	{
		doorSymbol = sym;
	}
	char getSymbol() const
	{
		return doorSymbol;
	}
	Cell* getCameFrom() const
	{
		return cameFrom;
	}
	void setCameFrom(Cell* came)
	{
		cameFrom = came;
	}
};

