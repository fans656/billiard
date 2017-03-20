#pragma once
#include "cv.h"
#include "Vec.h"
class CMybutton
{
	IplImage* m_img;
	bool m_on;
	CVec m_pos;
public:
	CMybutton () : m_on(false) {}

	bool hit_test (int x, int y);

	void set_image (IplImage* img) { m_img = img; }
	void set_position (int x, int y) { m_pos = CVec(x, y); }
	void turn_on () { m_on = true; }
	void turn_off () { m_on = false; }
	void draw (IplImage* des);
};