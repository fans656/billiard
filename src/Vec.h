#pragma once
#include "cv.h"
#include <cmath>
#include <utility>

using namespace std;

class CVec
{
public:
	double x, y;
public:
	CVec (double xx = 0, double yy = 0) : x(xx), y(yy) {}

	operator CvPoint() { return cvPoint(x, y); }
	// addition & subtraction
	CVec& operator+= (const CVec& v) { x += v.x; y += v.y; return *this; }
	CVec& operator-= (const CVec& v) { x -= v.x; y -= v.y; return *this; }
	// scalar multiplication
	CVec& operator*= (double k) { x *= k; y *= k; return *this; }
	// unit vector
	CVec& normalize ();
	CVec unit () const { return CVec(x, y).normalize(); }
	// mod
	double mod () const { return sqrt(x * x + y * y); }
	// resolve
	pair<CVec, CVec> resolve (const CVec& a, const CVec& b);
};
// logical
inline bool operator== (const CVec& a, const CVec& b) { return a.x == b.x && a.y == b.y; }
// negative
inline CVec operator- (const CVec& v) { return CVec(-v.x, -v.y); }
// addition & subtraction
inline CVec operator+ (const CVec& a, const CVec& b) { return CVec(a) += b; }
inline CVec operator- (const CVec& a, const CVec& b) { return CVec(a) -= b; }
// scalar multiplication
inline CVec operator* (double k, const CVec& v) { return CVec(v) *= k; }
inline CVec operator* (const CVec& v, double k) { return CVec(v) *= k; }
// dot production
inline double operator* (const CVec& a, const CVec& b) { return a.x * b.x + a.y * b.y; }
// helpers
double dis (const CVec& a, const CVec& b);
	// a->b
CVec normal_vector (const CVec& a, const CVec& b);
	// rotate normal vector counterclockwise
inline CVec tangent_vector (const CVec& nv) { return CVec(-nv.y, nv.x); }