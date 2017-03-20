#include "Res.h"
#include "highgui.h"
#include "Color.h"

CRes::CRes(void)
	: m_img_balls(8),
	m_img_mini_balls(7)
{
	m_img_back					= cvLoadImage("back.jpg");
	m_img_strength_box			= cvLoadImage("strength_box.bmp");
	m_img_button_close			= cvLoadImage("button_close.bmp");
	m_img_button_new_game		= cvLoadImage("button_new_game.bmp");
	m_img_cursor				= cvLoadImage("cursor.bmp");
	m_img_balls[CColor::WHITE]	= cvLoadImage("ball_white.bmp");
	m_img_balls[CColor::BLACK]	= cvLoadImage("ball_black.bmp");
	m_img_balls[CColor::PINK]	= cvLoadImage("ball_pink.bmp");
	m_img_balls[CColor::BLUE]	= cvLoadImage("ball_blue.bmp");
	m_img_balls[CColor::BROWN]	= cvLoadImage("ball_brown.bmp");
	m_img_balls[CColor::GREEN]	= cvLoadImage("ball_green.bmp");
	m_img_balls[CColor::YELLOW]	= cvLoadImage("ball_yellow.bmp");
	m_img_balls[CColor::RED]	= cvLoadImage("ball_red.bmp");
	m_img_mini_balls[CColor::BLACK	- 1]		= cvLoadImage("mini_ball_black.bmp");
	m_img_mini_balls[CColor::PINK	- 1]		= cvLoadImage("mini_ball_pink.bmp");
	m_img_mini_balls[CColor::BLUE	- 1]		= cvLoadImage("mini_ball_blue.bmp");
	m_img_mini_balls[CColor::BROWN	- 1]		= cvLoadImage("mini_ball_brown.bmp");
	m_img_mini_balls[CColor::GREEN	- 1]		= cvLoadImage("mini_ball_green.bmp");
	m_img_mini_balls[CColor::YELLOW	- 1]		= cvLoadImage("mini_ball_yellow.bmp");
	m_img_mini_balls[CColor::RED	- 1]		= cvLoadImage("mini_ball_red.bmp");
	m_img_frame = cvCreateImage(cvGetSize(m_img_back),
		m_img_back->depth, m_img_back->nChannels);
}


CRes::~CRes(void)
{
	cvReleaseImage(&m_img_frame);
	cvReleaseImage(&m_img_back);
	cvReleaseImage(&m_img_cursor);
	for (int i = 0; i < m_img_balls.size(); i++)
		cvReleaseImage(&m_img_balls[i]);
}
