#pragma once
#include "cv.h"

using namespace std;

class CRes
{
public:
	IplImage* m_img_frame;
	IplImage* m_img_back;
	IplImage* m_img_strength_box;
	IplImage* m_img_button_close;
	IplImage* m_img_button_new_game;
	IplImage* m_img_cursor;
	vector<IplImage*> m_img_balls;
	vector<IplImage*> m_img_mini_balls;
public:
	CRes(void);
	~CRes(void);
};

