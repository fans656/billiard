#include "Vec.h"

CVec& CVec::normalize ()
{
	if (x == 0 && y == 0)
		return *this;
	double r = sqrt(x * x + y * y);
	x /= r;
	y /= r;
	return *this;
}
pair<CVec, CVec> CVec::resolve (const CVec& a, const CVec& b)
{
	double deno = a.x * b.y - b.x * a.y;
	double k1 = (x * b.y - b.x * y) / deno;
	double k2 = (a.x * y - x * a.y) / deno;
	return pair<CVec, CVec>(k1 * a, k2 * b);
}
// helpers
double dis (const CVec& a, const CVec& b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;
	return sqrt(x * x + y * y);
}
CVec normal_vector (const CVec& a, const CVec& b)
{
	return (b - a).unit();
}