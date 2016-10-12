#pragma once
#include "HeaderIncludes.h"

typedef std::pair<std::string, size_t> expr;

class Simplifier
{
public:
	Simplifier();
	

	// start the simplifying 
	std::string Simplify(std::string src) const;

private:
	// on a given string with a given step 
	// it can return exrepsions of string and how many times 
	// that string is repeating 
	void doTheString(std::string src, Tail<expr>& qu, size_t step) const;
};

