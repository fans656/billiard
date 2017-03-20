#pragma once
#include "Vec.h"
#include "cv.h"
#include "Ball.h"
#include "Color.h"
class CBilliard
{
private:
	bool			m_is_still;
	vector<CBall>	m_balls;
	vector<CVec>	m_init_pos;
	int				m_left, m_top, m_right, m_bottom;
	pair<int, int>	m_edge_x[2];
	pair<int, int>	m_edge_y;
	vector<CVec>	m_tips;
	vector<CVec>	m_pockets;
	int				m_pocket_radius;
	int				m_D_radius;
	CVec			m_D_center;
public:
	CColor first_impact;
	vector<CColor> shoot_potted;
	int n_reds;
public:
	CBilliard ();

	void renew ();

	bool put_cue_ball (const CVec& p, bool init);
	CVec get_cue_ball () const { return m_balls[0].position(); }
	vector<CColor> get_remained_colors () const;
	void shoot (const CVec& v);
	void respot (CColor object);

	void go (double time);
	void draw (IplImage* des);

	bool is_still () const { return m_is_still; }
private:
	CColor get_first_impact ();
	void edge_test (CBall& b);
	void tip_test (CBall& b);
	bool pot_test (CBall& b);
	void impact_test ();
	double get_max_velocity () const;
	vector<CVec> get_occupiers (const CVec& p);
	void find_respot_position (CVec& p);
	CVec find_next_respot_position (const CVec& p);
};

