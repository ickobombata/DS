#include "Simplifier.h"


Simplifier::Simplifier()
{
}


std::string Simplifier::Simplify(std::string src) const
{
	expr start;
	start.first = src;
	start.second = 1;

	Tail<expr> qu1;
	Tail<expr> qu2;
	qu1.push(start);

	size_t step = src.length() / 2 + 1;
	Tail<expr>* pQu1 = &qu1;
	Tail<expr>* pQu2 = &qu2;

	bool isMultiplied = false;
	expr newTemp;
	newTemp.second = 0;
	// we start from the max step length / 2
	// and we use the doTheString method to 
	// cut on peaces every string that is possible to be cut
	// and then we do the same for those peaces 
	// also in the queue is pushed numbers of how many times the expresion is reapeating
	// and brackeds 
	while (--step)
	{
		while (!pQu1->empty())
		{
			expr temp = pQu1->front();
			pQu1->pop();

			if (temp.second > 1)
			{
				isMultiplied = true;
				newTemp.first = temp.second + '0';
				pQu2->push(newTemp);
				newTemp.first = '(';
				pQu2->push(newTemp);
			}
			
			if (temp.second == 0)
				pQu2->push(temp);
			else
				doTheString(temp.first, *pQu2, step);
			
			if (isMultiplied)
			{
				newTemp.first = ')';
				pQu2->push(newTemp);
			}
			
			isMultiplied = false;
		}
		std::swap(pQu1, pQu2);
	}

	// getting the result is kind of complicated
	// cuz if there is 2(2(2(RL))) it has to become 
	// 8(RL)
	std::string result = "";
	while (!pQu1->empty())
	{
		if (pQu1->front().second > 1)
			result += pQu1->front().second + '0';		
		else if (pQu1->front().second == 0)
		{
			// if we have an number 
			size_t expresionsCounter = 0;
			Tail<expr> tempyQueue;
			Stack<char> brackeds;

			// take all the brackets and all the numbers in a queue
			while (!brackeds.empty() || expresionsCounter == 0)
			{
				if (pQu1->front().second > 0) // it's expression
					expresionsCounter++;
				else if (pQu1->front().first[0] == '(')
					brackeds.push('(');
				else if (pQu1->front().first[0] == ')')
					brackeds.pop();

				tempyQueue.push(pQu1->front());
				pQu1->pop();
			}
			// if we have met more then 1 expresion then the simplificaten can't be done
			if (expresionsCounter > 1)
			{
				while (!tempyQueue.empty())
				{
					if (tempyQueue.front().second > 1)
						result += tempyQueue.front().second + '0';

					result += tempyQueue.front().first;
					tempyQueue.pop();
				}
			}
			else // otherwise we are left with the only expresion
			{ // we pop from the stack and multiply the numbers 
				size_t mul = 1;
				expr tempExpression;
				tempExpression = tempyQueue.front();
				tempyQueue.pop();
				
				while (tempExpression.second == 0) // is bracked or quantitiy
				{
					if (tempExpression.first[0] != '(')
						mul *= tempExpression.first[0] - '0';

					tempExpression = tempyQueue.front();
					tempyQueue.pop();
				}
				mul *= tempExpression.second;

				result += std::to_string( mul );
				bool needBrackeds = false;

				if (tempExpression.first.length() > 1)
				{
					needBrackeds = true;
					result += '(';
				}
				result += tempExpression.first;
				if (needBrackeds)
					result += ')';
			}
		}
		// on the end we are left with multipleid number and an expresion 
		if (pQu1->empty())
			break;

		result += pQu1->front().first;
		pQu1->pop();
	}
	return result;
}

void Simplifier::doTheString(std::string src, Tail<expr>& qu, size_t step) const
{
	size_t pos = 0, first, second, last = 0, counter = 1;
	bool equal , lastAddition = false;

	size_t srcLength = src.length();
	while (pos + 2 * step <= srcLength)
	{ // while there are substring that can be taken 
		equal = true;
		first = pos;
		second = pos + step;
		// check if the two substring are equal
		for (size_t i = 0; i < step; i++)
		{
			if (src[first + i] != src[second + i])
			{
				equal = false;
				break;
			}
		}
		// if they are equal
		if (equal)
		{// this is the first reapeating substring 
			if (counter == 1 && pos > 0)
			{
 				expr temp;
				temp.first = src.substr(last, pos - last);
				temp.second = 1;
				qu.push(temp);
			}
			last = pos;
			counter++;
			pos += step;
			lastAddition = true;
		}
		else
		{// if not equal
			// if we had an equal substirng before this char we add it 
			if (counter > 1)
			{
				expr temp;
				temp.first = src.substr(last, step);
				temp.second = counter;
				qu.push(temp);
				pos += step;
				last = pos;
			}
			else
			{// otherwise we just go one posion plus
				pos++;
			}
			lastAddition = false;
			counter = 1;
		}
	}
	expr temp;
	temp.second = counter;
	if (lastAddition)// test if we need to add some repeating substring plus
	{
		temp.first = src.substr(last, step);
		qu.push(temp);
		if (last + step + step < srcLength)
		{
			temp.first = src.substr(last + step + step, srcLength);
			temp.second = 1;
			qu.push(temp);
		}
	}
	else // or just substring from the last to the end
	{
		temp.first = src.substr(last, srcLength);
		qu.push(temp);
	}
}