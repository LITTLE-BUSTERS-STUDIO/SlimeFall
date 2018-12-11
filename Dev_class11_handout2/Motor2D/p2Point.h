// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __P2POINT_H__
#define __P2POINT_H__

#include "p2Defs.h"
#include <math.h>

template<class TYPE>
class p2Point
{
public:

	TYPE x, y;

	p2Point()
	{}

	p2Point(const p2Point<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	p2Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	p2Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	p2Point operator -(const p2Point &v) 
	{
		//x = x - v.x;
		//y = y - v.y;

		p2Point<TYPE> point;
		point.x = x - v.x;
		point.y = y - v.y;

		return point;
	}

	p2Point& operator + (const p2Point &v)
	{
		//x = x + v.x;
		//y = y + v.y;

		p2Point<TYPE> point;
		point.x = x + v.x;
		point.y = y + v.y;

		return point;
	}

	p2Point&  operator * (const TYPE &v)
	{
		x *= v;
		y *= v;

		return (*this);
	}

	const p2Point& operator -=(const p2Point &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const p2Point& operator +=(const p2Point &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const p2Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const p2Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	p2Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	p2Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const p2Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const p2Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const p2Point& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}

	void Normalize() {
		float module = sqrt(x * x + y * y);
		x = x / module;
		y = y / module;
	}

	p2Point Normalized() {

		p2Point normalized;
		float module = sqrtf(x * x + y * y);
		float norm_x, norm_x;
		norm_x = x / module;
		norm_x = x / module;

		return p2Point(norm_x, norm_y)
	}

};

typedef p2Point<int> iPoint;
typedef p2Point<float> fPoint;

#endif // __P2POINT_H__