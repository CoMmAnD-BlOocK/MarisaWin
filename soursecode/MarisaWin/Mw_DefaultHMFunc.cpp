#include "..\Mw_DefaultHMFunc.hpp"

void Mw_DefaultHMFunc::RefreshWindowSize(ExMessage* msg) {
	if (msg->message == WM_SIZE) {
		RECT window_rect;
		GetClientRect(GetHWnd(), &window_rect);
		if (window_rect.right - window_rect.left == 0) return;
		Resize(NULL, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top);
	}
}