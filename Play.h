#pragma once
#include "Billiard.h"
#include "Vec.h"
class CPlay
{
public:
	enum Result {
		BUSY, WHITE_IN, NEXT_SHOOT, OVER
	};
private:
	CBilliard m_billiard;

	CColor m_object;
	vector<IplImage*> m_img_colors;
	int m_i_player;
	int m_score[2];
public:
	CPlay ();
	void renew ();

	bool put_cue_ball (const CVec& p, bool init) { return m_billiard.put_cue_ball(p, init); }
	CVec get_cue_ball () const { return m_billiard.get_cue_ball(); }
	void shoot (const CVec& v) { m_billiard.shoot(v); }
		
	Result go (double time);
	void draw (IplImage* des);
private:
	int score () const;
	void add_score (int s) { m_score[m_i_player] += s; }
	void switch_player () { m_i_player = (m_i_player + 1) % 2; }
	bool white_in () const;
	CColor next_object () const;
	void draw_object (IplImage* des);
	void draw_score(IplImage* des);
	void respot ();
};