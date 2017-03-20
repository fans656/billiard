#include "Billiard.h"
#include "Color.h"
#include "Res.h"

#define COLOR_WHITE	cvScalar(255, 255, 255)

extern CvFont g_font;

CBilliard::CBilliard ()
	: m_balls(22),
	m_init_pos(21),
	m_tips(12),
	m_pockets(6)
{
	double acceleration;
	int ball_radius;
	int x, y;
	FILE *fp = fopen("data.txt", "r");
	// read acceleration
	fscanf(fp, " acceleration		%lf", &acceleration);
	CBall::set_acceleration(acceleration);
	// read ball radius
	fscanf(fp, " ball_radius		%d", &ball_radius);
	CBall::set_radius(ball_radius);
	// read pocket radius
	fscanf(fp, " pocket_radius		%d", &m_pocket_radius);
	// read D radius
	fscanf(fp, " D_radius			%d", &m_D_radius);
	// read D center
	fscanf(fp, " D_center			%d %d", &x, &y);
	m_D_center = CVec(x, y);
	// read corner
	fscanf(fp, " corner				%d %d %d %d",
		&m_left, &m_top, &m_right, &m_bottom);
	// read balls
	for (int i = 0; i < m_init_pos.size(); i++) {
		fscanf(fp, "%d %d", &x, &y);
		m_init_pos[i] = CVec(x, y);
	}
	// read pockets
	for (int i = 0; i < m_pockets.size(); i++) {
		fscanf(fp, "%d %d", &x, &y);
		m_pockets[i] = CVec(x, y);
	}
	// read tips
	for (int i = 0; i < m_tips.size(); i++) {
		fscanf(fp, "%d %d", &x, &y);
		m_tips[i] = CVec(x, y);
	}
	m_edge_x[0] = pair<int, int>(m_tips[1].x, m_tips[2].x);
	m_edge_x[1] = pair<int, int>(m_tips[3].x, m_tips[4].x);
	m_edge_y	= pair<int, int>(m_tips[0].y, m_tips[6].y);
	fclose(fp);
	// set images
	extern CRes g_res;
	for (int i = 0; i < CColor::RED; i++) {
		m_balls[i].set_color(CColor::WHITE + i);
		m_balls[i].set_image(g_res.m_img_balls[i]);
	}
	for (int i = CColor::RED; i < m_balls.size(); i++) {
		m_balls[i].set_color(CColor::RED);
		m_balls[i].set_image(g_res.m_img_balls[CColor::RED]);
	}
}
void CBilliard::renew ()
{
	n_reds = 15;
	m_balls[0].pot();
	for (int i = 0; i < m_init_pos.size(); i++) {
		m_balls[i + 1].set_position(m_init_pos[i]);
		m_balls[i + 1].set_velocity(CVec(0, 0));
		m_balls[i + 1].unpot();
	}
}
bool CBilliard::put_cue_ball (const CVec& p, bool init)
{
	if (init && (m_D_center.x < p.x || dis(p, m_D_center) > m_D_radius))
		return false;

	for (int i = 1; i < m_balls.size(); i++)
		if (!m_balls[i].is_potted() && is_touched(CBall(p), m_balls[i]))
			return false;
	m_balls[0].set_position(p);
	m_balls[0].set_velocity(CVec(0, 0));
	m_balls[0].unpot();
	return true;
}
vector<CColor> CBilliard::get_remained_colors () const
{
	vector<CColor> ret;
	for (int i = CColor::BLACK; i <= CColor::YELLOW; i++)
		if (!m_balls[i].is_potted())
			ret.push_back(m_balls[i].color());
	return ret;
}
void CBilliard::shoot (const CVec& v)
{
	m_balls[0].set_velocity(v);
	m_is_still = false;
	shoot_potted.clear();
	first_impact = get_first_impact();
}
void CBilliard::respot (CColor object)
{
	if (object == CColor::WHITE)
		return;
	CVec pos = m_init_pos[object - 1];
	if (get_occupiers(pos).size()) {
		int i;
		for (i = 0; i < 6; i++) {
			if (!get_occupiers(m_init_pos[i]).size()) {
				pos = m_init_pos[i];
				break;
			}
		}
		if (i == 6)
			find_respot_position(pos);
	}
	m_balls[object].set_position(pos);
	m_balls[object].set_velocity(CVec(0, 0));
	m_balls[object].unpot();
}
void CBilliard::go (double time)
{
	while (time > 0) {
		for (int i = 0; i < m_balls.size(); i++) {
			if (m_balls[i].is_potted())
				continue;
			edge_test(m_balls[i]);
			tip_test(m_balls[i]);
			extern bool g_debug_wont_pot;
			if (pot_test(m_balls[i])) {
				if (g_debug_wont_pot) {
					m_balls[i].reverse_vx();
					m_balls[i].reverse_vy();
				} else {
					m_balls[i].pot();
					shoot_potted.push_back(m_balls[i].color());
					if (m_balls[i].color() == CColor::RED)
						n_reds--;
				}
			}
		}
		impact_test();
		double v = get_max_velocity();
		if (v == 0) {
			m_is_still = true;
			return;
		}
		double t = 1;
		for (int i = 0; i < m_balls.size(); i++)
			m_balls[i].go(t);
		time -= t;
	}
}
void CBilliard::draw (IplImage* des)
{
	for (int i = 0; i < m_balls.size(); i++)
		m_balls[i].draw(des);
	// debug
	extern bool g_debug_pockets;
	if (g_debug_pockets) {
		for (int i = 0; i < m_pockets.size(); i++)
			cvCircle(des, m_pockets[i], m_pocket_radius, COLOR_WHITE);
		cvPutText(des, "P", cvPoint(35, 19), &g_font, COLOR_WHITE);
	}
	extern bool g_debug_tips;
	if (g_debug_tips) {
		for (int i = 0; i < m_tips.size(); i++)
			cvCircle(des, m_tips[i], 1, cvScalar(255,255,255));
		cvPutText(des, "T", cvPoint(50, 19), &g_font, COLOR_WHITE);
	}
	extern bool g_debug_edges;
	if (g_debug_edges) {
		//
		cvLine(des,	CVec(m_edge_x[0].first, m_top),
			CVec(m_edge_x[0].second, m_top), COLOR_WHITE);
		cvLine(des,	CVec(m_edge_x[1].first, m_top),
			CVec(m_edge_x[1].second, m_top), COLOR_WHITE);
		cvLine(des,	CVec(m_edge_x[0].first, m_bottom),
			CVec(m_edge_x[0].second, m_bottom), COLOR_WHITE);
		cvLine(des,	CVec(m_edge_x[1].first, m_bottom),
			CVec(m_edge_x[1].second, m_bottom), COLOR_WHITE);
		cvLine(des,	CVec(m_left, m_edge_y.first),
			CVec(m_left, m_edge_y.second), COLOR_WHITE);
		cvLine(des,	CVec(m_right, m_edge_y.first),
			CVec(m_right, m_edge_y.second), COLOR_WHITE);
		cvPutText(des, "E", cvPoint(65, 19), &g_font, COLOR_WHITE);
	}
	extern bool g_debug_wont_pot;
	if (g_debug_wont_pot) {
		cvPutText(des, "W", cvPoint(80, 19), &g_font, COLOR_WHITE);
	}
}
CColor CBilliard::get_first_impact ()
{
	if (CBall::acceleration >= 0)
		return CColor::NONE;

	CBall b = m_balls[0];
	while (!b.is_still()) {
		edge_test(b);
		tip_test(b);
		if (pot_test(b))
			return CColor::NONE;
		for (int i = 1; i < m_balls.size(); i++)
			if (will_impact(b, m_balls[i]))
				return m_balls[i].color();
		b.go(1.0 / b.velocity());
	}
	return CColor::NONE;
}
void CBilliard::edge_test (CBall& b)
{
	if (b.will_impact_left(m_left, m_edge_y)
		|| b.will_impact_right(m_right, m_edge_y))
		b.reverse_vx();
	if (b.will_impact_top(m_top, m_edge_x[0], m_edge_x[1])
		|| b.will_impact_bottom(m_bottom, m_edge_x[0], m_edge_x[1]))
		b.reverse_vy();
}
void CBilliard::tip_test (CBall& b)
{
	for (int i = 0; i < m_tips.size(); i++) {
		if (b.will_impact(m_tips[i])) {
			b.impact(m_tips[i]);
			return;
		}
	}
}
bool CBilliard::pot_test (CBall& b)
{
	for (int i = 0; i < m_pockets.size(); i++) {
		if (b.will_pot(m_pockets[i], m_pocket_radius))
			return true;
	}
	return false;
}
void CBilliard::impact_test ()
{
	for (int i = 0; i < m_balls.size(); i++) {
		for (int j = i + 1; j < m_balls.size(); j++) {
			if (will_impact(m_balls[i], m_balls[j])) {
				impact(m_balls[i], m_balls[j]);
			}
		}
	}
}
double CBilliard::get_max_velocity () const
{
	double v = 0;
	for (int i = 0; i < m_balls.size(); i++) {
		if (m_balls[i].is_potted())
			continue;
		if (v < m_balls[i].velocity())
			v = m_balls[i].velocity();
	}
	return v;
}
vector<CVec> CBilliard::get_occupiers (const CVec& p)
{
	vector<CVec> ret;
	for (int i = 0; i < m_balls.size(); i++)
		if (!m_balls[i].is_potted() && is_touched(CBall(p), m_balls[i]))
			ret.push_back(m_balls[i].position());
	return ret;
}
void CBilliard::find_respot_position (CVec& p)
{
	while (p.x >= m_left + CBall::radius) {
		p = find_next_respot_position(p);
		if (get_occupiers(p).size() == 0)
			return;
	}
	p.x = m_right - CBall::radius;
	while (1) {
		p = find_next_respot_position(p);
		if (get_occupiers(p).size() == 0)
			return;
	}
}
CVec CBilliard::find_next_respot_position (const CVec& p)
{
	vector<CVec> ocs = get_occupiers(p);
	if (ocs.size() == 0)
		return p;

	CVec min_oc = ocs[0];
	for (int i = 1; i < ocs.size(); i++) {
		if (ocs[i].x < min_oc.x)
			min_oc = ocs[i];
	}
	double c = 2 * CBall::radius;
	double y = abs(min_oc.y - p.y);
	double x = sqrt(c * c - y * y);
	return CVec(min_oc.x - x, p.y);
}