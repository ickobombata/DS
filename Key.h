#pragma once
#include "Cell.h"
#include "Door.h"
#include "HeaderIncludes.h"

// Every key opens a door 
// and he know which door it is 
// keys are vary by their symbol 
class Key : public Cell
{
	char keySymbol;
	Door *masterDoor;
public:
	Key(char ks = '`', Door* canOpen = NULL)
	{
		setDoor(canOpen);
		keySymbol = ks;
	}
	virtual ~Key(){}
	virtual Cell* clone() const { return new Key(*this); }
	virtual bool isPassable() const{ return false; }
	
	void setDoor(Door* canOpen)
	{
		if (canOpen != NULL)
			masterDoor = canOpen;
	}
	void tryToOpen(const Door& d)
	{
		if(masterDoor == &d)
			masterDoor->open();
	}
	bool canOpen(const Door& d)
	{
		if (masterDoor == &d)
			return true;
		return false;
	}
	virtual void mapPrint(std::ostream& out) const
	{
		out << keySymbol;
	}
	void setSymbol(char sym)
	{
		keySymbol = sym;
	}
	char getSymbol() const 
	{
		return keySymbol;
	}
};

