// ----------------------------------------------------
// MAT_Point class    -----------
// ----------------------------------------------------

#ifndef __MATPOINT_H__
#define __MATPOINT_H__

#include "p2Defs.h"
#include <math.h>
#include "SDL/include/SDL_rect.h"

template<class TYPE>
class MAT_Point
{
public:

	TYPE x, y;

	MAT_Point()
	{}

	~MAT_Point()
	{}

	MAT_Point(const MAT_Point<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	MAT_Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	MAT_Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	MAT_Point operator -(const MAT_Point& v) const
	{
		MAT_Point r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	MAT_Point operator + (const MAT_Point& v) const
	{
		MAT_Point r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const MAT_Point& operator -=(const MAT_Point& v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const MAT_Point& operator +=(const MAT_Point& v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const MAT_Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const MAT_Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	//Operators w/ Template -----------

	MAT_Point operator - (const TYPE& v) const
	{
		MAT_Point r;

		r.x = x - v;
		r.y = y - v;

		return(r);
	}

	MAT_Point operator + (const TYPE& v) const
	{
		MAT_Point r;

		r.x = x + v;
		r.y = y + v;

		return(r);
	}


	MAT_Point operator * (const TYPE& v) const
	{
		MAT_Point r;

		r.x = x * v;
		r.y = y * v;

		return(r);
	}

	MAT_Point operator / (const TYPE& v) const
	{
		MAT_Point r;

		r.x = x / v;
		r.y = y / v;

		return(r);
	}


	const MAT_Point& operator +=(const TYPE& v)
	{
		x += v;
		y += v;

		return(*this);
	}

	const MAT_Point& operator -=(const TYPE& v)
	{
		x -= v;
		y -= v;

		return(*this);
	}

	const MAT_Point& operator *=(const TYPE& v)
	{
		x *= v;
		y *= v;

		return(*this);
	}

	const MAT_Point& operator /=(const TYPE& v)
	{
		x /= v;
		y /= v;

		return(*this);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return ((x <= 0.01f && x >= -0.01f) && (y <= 0.01f && y >= -0.01f));
	}

	MAT_Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	MAT_Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	MAT_Point& Abs()
	{
		x = abs(x);
		y = abs(y);

		return(*this);
	}

	MAT_Point GetInverse()
	{
		MAT_Point r;
		r.x = -x;
		r.y = -y;

		return(r);
	}

	double Dot(MAT_Point& v)
	{
		return((x * v.x) + (y * v.y));
	}

	double Module()
	{
		if ((x <= 0.01f && x >= -0.01f) && (y <= 0.01f && y >= -0.01f))//if the vector is nearly 0
			return 0;
		else
			return sqrtf((x * x) + (y * y));
	}

	void Normalize()
	{
		double mod = Module();
		if (mod != 0)
		{
			x = (x / mod);
			y = (y / mod);
		}
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const MAT_Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx * fx) + (fy * fy));
	}

	TYPE DistanceNoSqrt(const MAT_Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx * fx) + (fy * fy);
	}

	TYPE DistanceManhattan(const MAT_Point& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}

	bool PointInRect( const SDL_Rect* r)
	{
		return ((x >= r->x) && (x < (r->x + r->w)) &&
			(y >= r->y) && (y < (r->y + r->h))) ? true : false;
	}

	bool InsideCircle(MAT_Point tile, float radius)
	{
		float distance_squared = this->DistanceNoSqrt(tile);

		float maxDist = (radius + 0.5f) * (radius + 0.5f);
		return distance_squared < maxDist;
	}
};

typedef MAT_Point<int> iMPoint;
typedef MAT_Point<float> fMPoint;
typedef MAT_Point<double> dMPoint;

#endif // __MATPOINT_H__