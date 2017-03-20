#pragma once
#define _AFXDLL
#include <afxwin.h>
#include "cv.h"
#include "Play.h"
#include "Mybutton.h"

class CGame
{
private:
	enum IButton {
		I_BUTTON_CLOSE = 0, I_BUTTON_NEW_GAME
	};
public:
	enum State {
		TERMINATED = 0,
		INIT_PUT, FREE_PUT,
		AIM, SHOOT, MOVE,
		OVER
	};
	enum Button_event {
		BE_ON, BE_OFF, BE_CLICKED
	};
private:
	int m_ms_per_frame;
	State m_state;
	CPlay m_play;
	CVec m_cue_ball;
	CVec m_cursor;
	int m_strength;
	int m_delta_strength;
	double m_max_v;

	IplImage* m_img_back;
	IplImage* m_img_cue_ball;
	IplImage* m_img_cursor;
	IplImage* m_img_strength_box;
	IplImage* m_img_frame;
public:
	vector<CMybutton> m_buttons;
public:
	CGame(void);
	~CGame(void);

	void renew () { m_play.renew(); set_state(INIT_PUT); }

	State state () const { return m_state; }
	void set_state (State s) { m_state = s; }

	void move_cue_ball (int x, int y);
	void move_cursor (int x, int y);
	bool put_cue_ball (bool init) { return m_play.put_cue_ball(m_cue_ball, init); }
	bool can_shoot (int x, int y);
	void shoot ();

	void go ();
	void draw ();

	void button_handler (int i, Button_event be);
private:
	void draw_cue_ball ();
	void draw_strength ();
	void draw_aim ();
	void accumulate ();
};

