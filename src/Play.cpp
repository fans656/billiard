#include "Play.h"
#include "utils.h"
#include "Res.h"
#include "Ball.h"

CvFont g_font = cvFont(1, 2);

CPlay::CPlay ()
	: m_img_colors(7)
{
	extern CRes g_res;
	for (int i = 0; i < m_img_colors.size(); i++)
		m_img_colors[i] = g_res.m_img_mini_balls[i];
}

void CPlay::renew ()
{
	m_object = CColor::RED;
	m_i_player = 0;
	m_score[0] = m_score[1] = 0;
	m_billiard.renew();
}
CPlay::Result CPlay::go (double time)
{
	m_billiard.go(time);

	if (!m_billiard.is_still())
		return BUSY;

	int dscore = score();
	if (dscore > 0) {
		add_score(dscore);
		m_object = next_object();
		if (m_object == CColor::NONE)
			return OVER;
	} else if (dscore < 0) {
		if (m_object == CColor::COLOR) {
			if (m_billiard.n_reds)
				m_object = CColor::RED;
			else
				m_object = CColor::YELLOW;
		} else if (m_billiard.n_reds == 0 && white_in()) {
			m_object = CColor::YELLOW;
		}
		switch_player();
		add_score(-dscore);
	} else {
		if (m_object == CColor::COLOR)
			m_object = CColor::RED;
		switch_player();
	}
	respot();
	return white_in() ? WHITE_IN : NEXT_SHOOT;
}
void CPlay::draw (IplImage* des)
{
	m_billiard.draw(des);
	draw_object(des);
	draw_score(des);
}
int CPlay::score () const
{
	CColor fi = m_billiard.first_impact;
	const vector<CColor>& po = m_billiard.shoot_potted;

	if (po.size() == 0) {
		if (m_object == fi)
			return 0;
		else
			return -fi.penalty();
	} else if (po.size() == 1) {
		if (m_object == fi && m_object == po[0])
			return po[0].score();
		else
			return -po[0].penalty();
	} else {
		int i;
		for (i = 0; i < po.size(); i++)
			if (po[i] != CColor::RED)
				break;
		if (i == po.size())
			return po.size() * po[0].score();
		int max_penalty = 0;
		for (int i = 0; i < po.size(); i++)
			if (max_penalty < po[i].penalty())
				max_penalty = po[i].penalty();
		if (m_object == fi)
			return -max_penalty;
		else
			return -max(fi.penalty(), max_penalty);
	}
}
bool CPlay::white_in () const
{
	for (int i = 0; i < m_billiard.shoot_potted.size(); i++)
		if (m_billiard.shoot_potted[i] == CColor::WHITE)
			return true;
	return false;
}
CColor CPlay::next_object () const
{
	if (m_billiard.n_reds) {
		switch (m_object.m_c) {
		case CColor::COLOR:
			return CColor::RED;
		case CColor::RED:
			return CColor::COLOR;
		}
	} else {
		switch (m_object.m_c) {
		case CColor::RED:
			return CColor::COLOR;
		case CColor::COLOR:
			return CColor::YELLOW;
		case CColor::YELLOW:
			return CColor::GREEN;
		case CColor::GREEN:
			return CColor::BROWN;
		case CColor::BROWN:
			return CColor::BLUE;
		case CColor::BLUE:
			return CColor::PINK;
		case CColor::PINK:
			return CColor::BLACK;
		case CColor::BLACK:
			return CColor::NONE;
		}
	}
}
void CPlay::draw_object (IplImage* des)
{
	CVec beg = m_i_player ? CVec(787, 4) : CVec(217, 4);
	if (m_object == CColor::RED) {
		::draw(m_img_colors[CColor::RED - 1], des, int(beg.x), int(beg.y), cvScalar(0, 255, 255));
	} else if (m_object == CColor::COLOR) {
		if (m_i_player == 1)
			beg.x -= 5 * 21;
		for (int i = 0; i < 6; i++) {
			::draw(m_img_colors[i], des,
				int(beg.x), int(beg.y), cvScalar(0, 255, 255));
			beg.x += 21;
		}
	} else if (m_object == CColor::NONE) {
		return;
	} else {
		::draw(m_img_colors[m_object - 1], des,
			int(beg.x), int(beg.y), cvScalar(0, 255, 255));
	}
}
void CPlay::draw_score (IplImage* des)
{
	static char buf[4];
	sprintf(buf, "%d", m_score[0]);
	cvPutText(des, buf, cvPoint(180, 19), &g_font, cvScalar(0, 0, 0));
	sprintf(buf, "%d", m_score[1]);
	cvPutText(des, buf, cvPoint(870, 19), &g_font, cvScalar(0, 0, 0));
}
void CPlay::respot ()
{
	if (m_object == CColor::COLOR || m_object == CColor::RED) {
		for (int i = 0; i < m_billiard.shoot_potted.size(); i++)
			if (CColor(CColor::COLOR) == m_billiard.shoot_potted[i]
			&& m_billiard.shoot_potted[i] < m_object.m_c)
				m_billiard.respot(m_billiard.shoot_potted[i]);
	} else {
		for (int i = 0; i < m_billiard.shoot_potted.size(); i++)
			if (m_object != m_billiard.shoot_potted[i]
			&& m_billiard.shoot_potted[i] != CColor::RED
				&& m_billiard.shoot_potted[i] < m_object.m_c)
				m_billiard.respot(m_billiard.shoot_potted[i]);
	}
}