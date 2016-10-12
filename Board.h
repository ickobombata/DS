#pragma once 

class Board;

#include "Cell.h"

// the board is matrix of cells 
// and the cells can be doors or key or ...
class Board
{
private:
	Cell*** pppBoard;
	
	size_t rows;
	size_t columns;
	Point cursor;
public:
	Board();
	~Board();

	bool LoadFromFile(const char* Filename);
	void unMarkCells();
	Cell* getStart() const;
	Cell* getEnd() const;
	Cell* getCellAt(int x, int y);
	void printBoard() const ;
private:
	Cell*** allocateNewBoard(size_t rowsCount, size_t colsCount);
	void freeBoard(Cell***& ppBoard, size_t rowsCount ,size_t columnsCount);

	bool GetBoardDImension(std::istream& InputFile, size_t& r, size_t& c);
	void ReadBoardFromFile(std::ifstream & InputFile, Cell ***& ppBoard);

	Cell* generateMeCell(char cellCode) const;
};

