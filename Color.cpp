#include "Color.h"

int CColor::penalty () const
{
	switch (m_c) {
	case NONE:
	case WHITE:
	case RED:
	case YELLOW:
	case GREEN:
	case BROWN:
		return 4;
	case BLUE:
		return 5;
	case PINK:
		return 6;
	case BLACK:
		return 7;
	}
}
int CColor::score () const
{
	switch (m_c) {
	case RED:
		return 1;
	case YELLOW:
		return 2;
	case GREEN:
		return 3;
	case BROWN:
		return 4;
	case BLUE:
		return 5;
	case PINK:
		return 6;
	case BLACK:
		return 7;
	default:
		throw Bad_score();
	}
}