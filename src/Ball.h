#pragma once
#include "Vec.h"
#include "cv.h"
#include "Color.h"
class CBall
{
	friend bool is_touched (const CBall& a, const CBall& b);
	friend bool is_touched (const CVec& p, const CBall& b);
	friend bool will_impact (const CBall& a, const CBall& b);
	friend void impact (CBall& a, CBall& b);
	friend void impact (CBall& a, CBall& b, CBall& c);
private:
	bool m_is_potted;
	CColor m_color;
	CVec m_p;
	CVec m_v;
	IplImage* m_img;

public:
	static int radius;
	static double acceleration;
public:
	CBall () : m_is_potted(true) {}
	CBall (const CVec& p) : m_p(p) {}

	void pot () { m_is_potted = true; }
	void unpot () { m_is_potted = false; }
	void set_color (CColor color) { m_color = color; }
	void set_position (const CVec& p) { m_p = p; }
	void set_velocity (const CVec& v) { m_v = v; }
	void set_image (IplImage* img) { m_img = img; }

	static void set_radius (int r) { radius = r; }
	static void set_acceleration (double a) { acceleration = a; }

	void go (double time);
	void draw (IplImage* des);

	bool is_potted () const { return m_is_potted; }
	CColor color () const { return m_color; }
	bool is_still () const { return m_v == CVec(0, 0); }
	double velocity () const { return m_v.mod(); }
	CVec position () const { return m_p; }

	void reverse_vx () { m_v.x = -m_v.x; }
	void reverse_vy () { m_v.y = -m_v.y; }
	void impact (const CVec& p);
	
	// edge
	bool will_impact_left	(int x, const pair<int, int>& y) const;
	bool will_impact_right	(int x, const pair<int, int>& y) const;
	bool will_impact_top	(int y, const pair<int, int>& x1, const pair<int, int>& x2) const;
	bool will_impact_bottom	(int y, const pair<int, int>& x1, const pair<int, int>& x2) const;
	// tip
	bool will_impact (const CVec& p) const;
	// pocket
	bool will_pot (const CVec& p, int r) const { return dis(m_p, p) < r; }
private:
	// edge
	bool is_touched_left	(int x) const { return m_p.x - x <= radius; }
	bool is_touched_right	(int x) const { return x - m_p.x <= radius; }
	bool is_touched_top		(int y) const { return m_p.y - y <= radius; }
	bool is_touched_bottom	(int y) const { return y - m_p.y <= radius; }
	// tip
	bool is_touched (const CVec& p) const { return dis(m_p, p) <= radius; }
};

inline bool is_touched (const CBall& a, const CBall& b) { return dis(a.m_p, b.m_p) < 2 * CBall::radius; }
inline bool is_touched (const CVec& p, const CBall& b) { return dis(p, b.m_p) <= CBall::radius; }
bool will_impact (const CBall& a, const CBall& b);

void impact (CBall& a, CBall& b);
void impact (CBall& a, CBall& b, CBall& c);