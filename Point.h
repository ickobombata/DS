#pragma once

///
/// This is two dimensional point
///
class Point
{
	int x;
	int y;
public:
	Point() : x(0), y(0){}
	Point(int xx, int yy){ setPoint(xx, yy); }
	
	int getX() const { return x; }
	int getY() const { return y; }
	void setPoint(int xx, int yy)
	{
		if (xx >= 0 && yy >= 0)
		{
			x = xx;
			y = yy;
		}
		else
		{
			x = 0;
			y = 0;
		}
	}
};