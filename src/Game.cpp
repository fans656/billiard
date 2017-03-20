#include "Game.h"
#include "utils.h"
#include "Res.h"
#include "highgui.h"

#define APP_NAME	"Snooker"

bool g_debug_step = false;
bool g_debug_velocities = false;
bool g_debug_pockets = false;
bool g_debug_tips = false;
bool g_debug_edges = false;
bool g_debug_wont_pot = false;

void on_mouse (int event, int x, int y, int flags, void* p);

CGame::CGame(void)
	: m_buttons(2)
	//m_button_new_clicked(false)
{
	// read settings
	FILE *fp = fopen("settings.txt", "r");
	fscanf(fp, " ms_per_frame		%d", &m_ms_per_frame);
	fscanf(fp, " delta_strength		%d", &m_delta_strength);
	fscanf(fp, " max_velocity		%lf", &m_max_v);
	fclose(fp);
	// set images
	extern CRes g_res;
	m_img_back			= g_res.m_img_back;
	m_img_cue_ball		= g_res.m_img_balls[0];
	m_img_cursor		= g_res.m_img_cursor;
	m_img_strength_box	= g_res.m_img_strength_box;
	m_img_frame			= g_res.m_img_frame;
	// set buttons
	m_buttons[I_BUTTON_NEW_GAME].set_image(g_res.m_img_button_new_game);
	m_buttons[I_BUTTON_NEW_GAME].set_position(945, 5);
	m_buttons[I_BUTTON_CLOSE].set_image(g_res.m_img_button_close);
	m_buttons[I_BUTTON_CLOSE].set_position(1001, 5);
	// create window
	cvNamedWindow(APP_NAME);
	cvSetMouseCallback(APP_NAME, on_mouse, this);
	HWND hwnd = (HWND)cvGetWindowHandle(APP_NAME);
	CWnd* pwnd = CWnd::FromHandle(hwnd)->GetParent();
	pwnd->ModifyStyle(WS_OVERLAPPEDWINDOW, WS_OVERLAPPED | WS_CAPTION);
	// new game
	renew();
}
CGame::~CGame(void)
{
	cvDestroyAllWindows();
}
void CGame::move_cue_ball (int x, int y)
{
	if (in_rect(x, y, 42, 68, 984, 546))
		m_cue_ball = CVec(x, y);
}
void CGame::move_cursor (int x, int y)
{
	if (in_rect(x, y, 14, 37, 1009, 573))
		m_cursor = CVec(x, y);
}
bool CGame::can_shoot (int x, int y)
{
	if (!in_rect(x, y, 14, 37, 1009, 573))
		return false;
	m_strength = 100;
	return true;
}
void CGame::shoot ()
{
	m_play.shoot(m_max_v * m_strength / 100 * (m_cursor - m_cue_ball).unit());
}
void CGame::go ()
{
	int ch = -1;
	switch (state()) {
	case SHOOT:
		accumulate();
		break;
	case MOVE:
		switch (m_play.go(m_ms_per_frame)) {
		case CPlay::WHITE_IN:
			set_state(FREE_PUT);
			break;
		case CPlay::NEXT_SHOOT:
			m_cue_ball = m_play.get_cue_ball();
			set_state(AIM);
			break;
		case CPlay::OVER:
			set_state(OVER);
			break;
		default:
			if (g_debug_step)
				ch = cvWaitKey(0);
			break;
		}
		break;
	}
	if (ch == -1)
		ch = cvWaitKey(m_ms_per_frame);
	switch (ch) {
	case 'q':
		set_state(TERMINATED);
		break;
	case 'r':
		renew();
		break;
	// debug switchers
	case 's':
		g_debug_step = !g_debug_step;
		break;
	case 'v':
		g_debug_velocities = !g_debug_velocities;
		break;
	case 'p':
		g_debug_pockets = !g_debug_pockets;
		break;
	case 't':
		g_debug_tips = !g_debug_tips;
		break;
	case 'e':
		g_debug_edges = !g_debug_edges;
		break;
	case 'w':
		g_debug_wont_pot = !g_debug_wont_pot;
		break;
	}
}
void CGame::draw ()
{
	// draw back
	cvCopyImage(m_img_back, m_img_frame);
	// draw buttons
	for (int i = 0; i < m_buttons.size(); i++)
		m_buttons[i].draw(m_img_frame);
	// draw play (balls, scores etc..)
	m_play.draw(m_img_frame);
	// draw optional stuff
	switch (state()) {
	case INIT_PUT:
	case FREE_PUT:
		draw_cue_ball();
		break;
	case SHOOT:
		draw_strength();
				// fall through
	case AIM:
		draw_aim();
		break;
	}
	// debug_step
	extern CvFont g_font;
	if (g_debug_step)
		cvPutText(m_img_frame, "S", cvPoint(5, 19), &g_font, cvScalar(255,255,255));
	// show this frame
	cvShowImage(APP_NAME, m_img_frame);
}
void CGame::button_handler (int i, Button_event be)
{
	switch (be) {
	case BE_ON:
		m_buttons[i].turn_on();
		break;
	case BE_OFF:
		m_buttons[i].turn_off();
		break;
	case BE_CLICKED:
		switch (i) {
		case I_BUTTON_NEW_GAME:
			renew();
			break;
		case I_BUTTON_CLOSE:
			set_state(TERMINATED);
			break;
		}
		break;
	}
}
void CGame::draw_cue_ball ()
{
	::draw(m_img_cue_ball, m_img_frame,
		int(m_cue_ball.x - CBall::radius + 1),
		int(m_cue_ball.y - CBall::radius + 1),
		cvScalar(0, 255, 255));
}
void CGame::draw_strength ()
{
	::draw(m_img_strength_box, m_img_frame, 428, 6);
	CVec beg(436, 14);
	CVec end = beg + 145 * m_strength / 100.0 * CVec(1, 0);
	cvLine(m_img_frame, beg, end, cvScalar(231, 205, 128), 8);
}
void CGame::draw_aim ()
{
	cvLine(m_img_frame, m_cue_ball, m_cursor,
		cvScalar(255, 255, 255));
	::draw(m_img_cursor, m_img_frame,
		int(m_cursor.x - CBall::radius + 1),
		int(m_cursor.y - CBall::radius + 1),
		cvScalar(0, 255, 255));
}
void CGame::accumulate ()
{
	m_strength += m_delta_strength;
	if (m_strength < 0 || m_strength > 100) {
		m_delta_strength = -m_delta_strength;
		m_strength += m_delta_strength;
	}
}

void on_mouse (int event, int x, int y, int flags, void* p)
{
	CGame& g = *(CGame*)p;
	switch (event) {
	case CV_EVENT_MOUSEMOVE:
		for (int i = 0; i < g.m_buttons.size(); i++) {
			if (g.m_buttons[i].hit_test(x, y))
				g.button_handler(i, CGame::BE_ON);
			else
				g.button_handler(i, CGame::BE_OFF);
		}
		switch (g.state()) {
		case CGame::INIT_PUT:
		case CGame::FREE_PUT:
			g.move_cue_ball(x, y);
			break;
		case CGame::AIM:
		case CGame::SHOOT:
			g.move_cursor(x, y);
			break;
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		switch (g.state()) {
		case CGame::INIT_PUT:
			if (g.put_cue_ball(true))
				g.set_state(CGame::AIM);
			break;
		case CGame::FREE_PUT:
			if (g.put_cue_ball(false))
				g.set_state(CGame::AIM);
			break;
		case CGame::AIM:
			if (g.can_shoot(x, y))
				g.set_state(CGame::SHOOT);
			break;
		}
		for (int i = 0; i < g.m_buttons.size(); i++) {
			if (g.m_buttons[i].hit_test(x, y))
				g.button_handler(i, CGame::BE_CLICKED);
		}
		break;
	case CV_EVENT_LBUTTONUP:
		switch (g.state()) {
		case CGame::SHOOT:
			g.shoot();
			g.set_state(CGame::MOVE);
			break;
		}
		break;
	}
}