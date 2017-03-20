#pragma once
#include "cv.h"
#include "Vec.h"

void draw (IplImage *src, IplImage *des, int x, int y);
void draw (IplImage *src, IplImage *des, int x, int y, CvScalar& mask);

inline bool in_segment (int x, int a, int b) { return a <= x && x <= b; }
inline bool in_rect (int x, int y, int left, int top, int right, int bottom)
{
	return left < x && x < right && top < y && y < bottom;
}