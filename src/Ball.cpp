#include "Ball.h"
#include "utils.h"
#include <algorithm>

extern CvFont g_font;

using namespace std;

int CBall::radius;
double CBall::acceleration;

void CBall::go (double time)
{
	if (is_potted() || is_still())
		return;

	double v = m_v.mod();
	double vt = max(0.0, v + acceleration * time);
	double s = (v + vt) * time / 2;
	m_p += s * m_v.unit();
	m_v = vt * m_v.unit();
}
void CBall::draw (IplImage* des)
{
	if (is_potted())
		return;

	::draw(m_img, des,
		int(m_p.x - radius + 1),
		int(m_p.y - radius + 1),
		cvScalar(0, 255, 255));
	// debug
	extern bool g_debug_velocities;
	if (g_debug_velocities) {
		cvLine(des, m_p, m_p + 100 * m_v, cvScalar(255,255,255));
		cvPutText(des, "V", cvPoint(20, 19), &g_font, cvScalar(255,255,255));
	}
}
void CBall::impact (const CVec& p)
{
	CVec nv = normal_vector(m_p, p);
	CVec tv = tangent_vector(nv);
	pair<CVec, CVec> v = m_v.resolve(nv, tv);
	v.first = -v.first;
	m_v = v.first + v.second;
}
bool CBall::will_impact_left (int x, const pair<int, int>& y) const
{
	if (!in_segment(m_p.y, y.first, y.second))
		return false;
	return is_touched_left(x) && m_v.x < 0;
}
bool CBall::will_impact_right (int x, const pair<int, int>& y) const
{
	if (!in_segment(m_p.y, y.first, y.second))
		return false;
	return is_touched_right(x)	&& m_v.x > 0;
}
bool CBall::will_impact_top (int y, const pair<int, int>& x1, const pair<int, int>& x2) const
{
	if (!in_segment(m_p.x, x1.first, x1.second) && !in_segment(m_p.x, x2.first, x2.second))
		return false;
	return is_touched_top(y)	&& m_v.y < 0;
}
bool CBall::will_impact_bottom (int y, const pair<int, int>& x1, const pair<int, int>& x2) const
{
	if (!in_segment(m_p.x, x1.first, x1.second) && !in_segment(m_p.x, x2.first, x2.second))
		return false;
	return is_touched_bottom(y)	&& m_v.y > 0;
}
bool CBall::will_impact (const CVec& p) const
{
	return is_touched(p) && m_v * normal_vector(m_p, p) > 0;
}
bool will_impact (const CBall& a, const CBall& b)
{
	if (a.is_potted() || b.is_potted())
		return false;
	if (a.is_still() && b.is_still())
		return false;
	if (!is_touched(a, b))
		return false;

	CVec nv = normal_vector(a.m_p, b.m_p);
	double va = a.m_v * nv;
	double vb = b.m_v * nv;
	if (va <= 0 && vb >= 0)
		return false;
	else 
		return va - vb > 0;
}
void impact (CBall& a, CBall& b)
{
	CVec nv = normal_vector(a.m_p, b.m_p);
	CVec tv = tangent_vector(nv);
	pair<CVec, CVec> va = a.m_v.resolve(nv, tv);
	pair<CVec, CVec> vb = b.m_v.resolve(nv, tv);
	swap(va.first, vb.first);
	a.m_v = va.first + va.second;
	b.m_v = vb.first + vb.second;
}