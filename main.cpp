#include <iostream>
#include "HeaderIncludes.h"
#include "Board.h"
#include "Solver.h"
#include "Cell.h"
#include "Simplifier.h"

void StartTheGame(char* filePath)
{
	Board b;
	Solver s;
	std::string endResult;
	Simplifier simplif;

	bool hasPath = b.LoadFromFile(filePath);
	b.printBoard();

	if (hasPath)
	hasPath = s.startAlgorithm(b, endResult);

	if (hasPath)
		std::cout << simplif.Simplify(endResult) << std::endl;
	else
		std::cout << "No path" << std::endl;
	std:cout << std::endl;
}

int main(int argc , char* argv[])
{
	for (size_t i = 1; i < argc; i++)
		StartTheGame(argv[i]);
	
	system("PAUSE");
	return 0;
}
