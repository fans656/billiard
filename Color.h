#pragma once
#include <utility>

using namespace std;

class CColor
{
	friend bool operator== (CColor a, CColor b);
public:
	enum Color {
		NONE = -2, COLOR = -1,
		WHITE = 0,
		BLACK, PINK, BLUE, BROWN, GREEN, YELLOW,
		RED
	};
	class Bad_score {};
public:
	Color m_c;
public:
	CColor (Color c = WHITE) { m_c = c; }
	CColor (int c) { m_c = Color(c); }

	operator int() { return int(m_c); }

	int penalty () const;
	int score () const;
};

inline bool operator== (CColor a, CColor::Color b)
{
	return a == CColor(b);
}
inline bool operator!= (CColor a, CColor::Color b)
{
	return !(a == b);
}
inline bool operator== (CColor a, CColor b)
{
	if (a.m_c != CColor::COLOR)
		return a.m_c == b.m_c;

	switch (b.m_c) {
	case CColor::NONE:
	case CColor::WHITE:
	case CColor::RED:
		return false;
	default:
		return true;
	}
}
inline bool operator!= (CColor a, CColor b)
{
	return !(a == b);
}