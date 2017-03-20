#include "Mybutton.h"
#include "utils.h"

bool CMybutton::hit_test (int x, int y)
{
	return in_rect(x, y,
		m_pos.x, m_pos.y,
		m_pos.x + m_img->width,
		m_pos.y + m_img->height);
}
void CMybutton::draw (IplImage* des)
{
	if (m_on == false)
		return;
	::draw(m_img, des, int(m_pos.x), int(m_pos.y));
}
