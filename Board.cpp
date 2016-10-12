#include "Board.h"

#include "EmptyCell.h"
#include "Door.h"
#include "StartPoint.h"
#include "EndPoint.h"
#include "Key.h"
#include "Wall.h"

Board::Board()
{
}

Board::~Board()
{
	freeBoard(pppBoard, rows , columns);
}

Cell*** Board::allocateNewBoard(size_t rowsCount, size_t columnsCount)
{
	Cell*** result = NULL;

	result = new(std::nothrow) Cell**[rowsCount];

	if (!result)
		return NULL;

	for (size_t i = 0; i < rowsCount; i++)
	{
		result[i] = NULL;

		result[i] = new(std::nothrow) Cell*[columnsCount];

		if (!result[i])
		{
			for (size_t j = 0; j < rowsCount; j++)
			{
				delete [] result[j];
			}
			delete []  result;

			return NULL;
		}
	}
	rows = rowsCount;
	columns = rowsCount;

	return result;
}
void Board::freeBoard(Cell***& b, size_t rowsCount , size_t columnsCount)
{
	if (!b)
		return;

	for(size_t i = 0 ; i < rowsCount ; i++)
		for(size_t j = 0 ; j < columnsCount ; j++)
			delete b[i][j];

	for (size_t i = 0; i < rowsCount; i++)
		delete[] b[i];

	delete[] b;

	b = NULL;
}

bool Board::LoadFromFile(const char* Filename)
{
	std::ifstream input(Filename);

	if (!input)
	{
		return false;
	}

	size_t Rows = 0;
	size_t Cols = 0;

	if (!GetBoardDImension(input, Rows, Cols))
	{
		return false;
	}
	Cell *** pppNewBoard = allocateNewBoard(Rows, Cols);

	if (!pppNewBoard)
	{
		return false;
	}

	ReadBoardFromFile(input, pppNewBoard);

	// Free the old contents of the board (if any)
	freeBoard(pppBoard, rows , columns);

	pppBoard = pppNewBoard;
	rows = Rows;
	columns = Cols;

	return true;
}
bool Board::GetBoardDImension(std::istream& InputFile, size_t& r, size_t& column)
{
	InputFile.clear();
	InputFile.seekg(0, std::ios::beg);

	int rows = 0; // Number of rows in the board
	int cols = 0; // Number of columns in the board

	char c = 0;
	int counter = 0;

	// Find the number of columns in the board
	while (InputFile.get(c) && c != '\n')
		cols++;


	// Find the number of rows in the board and also
	// verify that all rows have the same number of columns
	if (cols > 0)
	{
		rows++; // at leas one row was read from the file

		while (InputFile.get(c))
		{
			if (c == '\n')
			{
				// the number of columns on each line must be the same
				if (counter == 0)
					break;
				if (cols != counter)
					return false;

				rows++;
				counter = 0;
			}
			else
			{
				counter++;
			}
		}

		// The last row of the labyrinth may or may not be followed by a blank line
		// Thus if we just count the number of new lines, we may count one row less
		if (c != '\n')
			rows++;
	}

	r = rows;
	column = cols;

	return true;
}

// the reading is kind of complexed because while we read the map 
// we don't know which door can be opened with which key 
// for that we push in a list all of the strange simbols that can represent
// key or door and then when we read the pairs of key,door 
// looking through the list is done and the keys and doors are set on the map
void Board::ReadBoardFromFile(std::ifstream & InputFile, Cell ***& pppTheBoard)
{
	InputFile.clear();
	InputFile.seekg(0, std::ios::beg);

	int row = 0;
	int col = 0;
	char c;
	list< pair<Point, char> > left;
 	do
	{
		InputFile.get(c);

		if (c == '\n')
		{
			row++;
			col = 0;
		}
		else
		{
			Cell* temp = generateMeCell(c);
			if (temp)
			{
				temp->setCoordinates(row, col);
				temp->setOwner(this);
				pppTheBoard[row][col] = temp->clone();
			}
			else
			{
				pair<Point, char> l;
				Point p;
				p.setPoint(row, col);
				l.first = p;
				l.second = c;
				left.push_back(l);
			}
			delete temp;
			col++;
		}
	} while (row < rows);

	char key, door;
	while (InputFile.get(key))
	{
		if (key == '\n')
			continue;

		if (InputFile.get(door))
		{
			Key k;
			Door d;
			for (list< pair<Point, char> >::iterator i = left.begin(); i != left.end(); i++)
			{
				if (key == (*i).second)
				{
					k.setSymbol((*i).second);
					k.point = (*i).first;
					k.setOwner(this);
					left.erase(i);
					break;
				}
			}
			for (list< pair<Point, char> >::iterator i = left.begin(); i != left.end(); i++)
			{
				if (door == (*i).second)
				{
					d.setSymbol((*i).second);
					d.point = (*i).first;
					d.setOwner(this);
					left.erase(i);
					break;
				}
			}
			pppTheBoard[d.getCoordinates().getX()][d.getCoordinates().getY()] = d.clone();
			Door* theDoor = dynamic_cast<Door*>(pppTheBoard[d.getCoordinates().getX()][d.getCoordinates().getY()]);
			k.setDoor(theDoor);
			pppTheBoard[k.getCoordinates().getX()][k.getCoordinates().getY()] = k.clone();
		}
	}
}
void Board::printBoard() const
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			pppBoard[i][j]->mapPrint(std::cout);
		}
		std::cout << std::endl;
	}
}
Cell* Board::generateMeCell(char cellCode) const
{
	switch (cellCode)
	{
	case '#':{
		Wall* res = new Wall();
		return res;
			 }
	case ' ':{
		EmptyCell* res = new EmptyCell();
		return res;
	}
	case 'S':{
		StartPoint* res = new StartPoint();
		return res;
	}
	case 'X':{
		EndPoint* res = new EndPoint();
		return res;
	}
	default:{
		return NULL;
	}
	}
}
void Board::unMarkCells()
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			pppBoard[i][j]->UnMarkCell();
		}
	}
}
Cell* Board::getStart() const
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			StartPoint* sp = dynamic_cast<StartPoint*>(pppBoard[i][j]);
			if (sp)
				return pppBoard[i][j];
		}
	}
	return NULL;
}
Cell* Board::getCellAt(int x, int y)
{
	if (x >= 0 && x < rows && y >= 0 && y < columns)
	{
		return pppBoard[x][y];
	}
	else
	{
		return NULL;
	}
}
Cell* Board::getEnd() const
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			EndPoint* ep = dynamic_cast<EndPoint*>(pppBoard[i][j]);
			if (ep)
				return pppBoard[i][j];
		}
	}
	return NULL;
}