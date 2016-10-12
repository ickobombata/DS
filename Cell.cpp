#include "Cell.h"

Cell::Cell(Board* b, int x, int y)
{
	setOwner(b);
	point.setPoint(x, y);
}
Point Cell::getCoordinates() const
{
	return point;
}
void Cell::setCoordinates(int x, int y)
{
	point.setPoint(x, y);
}
void Cell::MarkCell()
{
	visited = true;
}
void Cell::UnMarkCell()
{
	visited = false;
}
bool Cell::isMarked() const
{
	return visited;
}
void Cell::setOwner(Board* b)
{
	owner = b;
}
Cell* Cell::getLeftNeighbour() const
{
	return owner->getCellAt(point.getX(), point.getY() - 1);
}
Cell* Cell::getRightNeighbour() const
{
	return owner->getCellAt(point.getX(), point.getY() + 1);
}
Cell* Cell::getTopNeighbour() const
{
	return owner->getCellAt(point.getX() - 1, point.getY());
}
Cell* Cell::getBottomNeighbour() const
{
	return owner->getCellAt(point.getX() + 1, point.getY());
}

void Cell::getNeighbours(Tail< Cell* >& result) const
{
	Cell* left = getLeftNeighbour();
	Cell* right = getRightNeighbour();
	Cell* top = getTopNeighbour();
	Cell* bottom = getBottomNeighbour();

	if (left)
		result.push(left);
	if (right)
		result.push(right);
	if (top)
		result.push(top);
	if (bottom)
		result.push(bottom);
}
