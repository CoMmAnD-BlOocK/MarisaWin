#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void Window::RunInstance() {
	initgraph(size.cx, size.cy);
	created = true;
	RECT rect;
	GetWindowRect(GetHWnd(), &rect);
	MoveWindow(GetHWnd(), rect.left, rect.top, size.cx, size.cy, true);
	HWND winHWnd = GetHWnd();
	SetWindowText(winHWnd, windowname.c_str());
	if (sizebox) {
		SetWindowLongPtr(winHWnd, GWL_STYLE, GetWindowLongPtrW(winHWnd, GWL_STYLE) | WS_SIZEBOX);
	}

	BeginBatchDraw();
	list<UpdateFunc>::iterator i;
	list<UpdateFunc>::iterator i_delete;
	ExMessage msg{};
	bool has_new_msg;
	clock_t counter;

	while (!exit) {
		counter = clock();
		for (list<function<void()>>::iterator i = highfreq_functionlist.begin(); i != highfreq_functionlist.end(); i++) {
			(*i)();
		}
		//Handle Message
		do {
			if (scrnow == NULL) break;
			has_new_msg = peekmessage(&msg);
			if (!has_new_msg) break;
			for (list<HandleMessageFunc>::iterator i = HandleMessage.begin(); i != HandleMessage.end(); i++) {
				i->function(&msg);
			}
			scrnow->DispatchMessageEW(&msg);
		} while (true);

		//Update Window
		if (updatefunctionlist.empty() == false) {
			i = updatefunctionlist.begin();
			while (i != updatefunctionlist.end()) {
				if ((*i).frame_countdown == 0) {
					(*i).function();
					if ((*i).repeat_times != 0) {
						if (!(*i).static_) (*i).repeat_times--;
						(*i).frame_countdown = (*i).repeat_interval;
					}
					else {
						i_delete = i;
						i++;
						updatefunctionlist.erase(i_delete);
						continue;
					}
				}
				else (*i).frame_countdown--;
				i++;
			}
		}

		//Redraw Window
		SetWorkingImage(NULL);
		if (scrnow != NULL) scrnow->Print();
		FlushBatchDraw();
		if (smooth_frame)
			while (clock() - counter < 1000 / fps) {
				for (list<function<void()>>::iterator i = highfreq_functionlist.begin(); i != highfreq_functionlist.end(); i++) {
					(*i)();
				}
			}
		else {
			counter = clock();
			while (clock() - counter < 1000 / fps) {
				for (list<function<void()>>::iterator i = highfreq_functionlist.begin(); i != highfreq_functionlist.end(); i++) {
					(*i)();
				}
			}
		}

		real_fps = 1000.0f / (clock() - counter);
		cleardevice();
	}
	EndBatchDraw();
	closegraph();
}


void Window::CloseWindow() {
	exit = true;
}

void Window::ChangeScreen(Screen* screen, bool clearfunc) {
	scrnow = screen;
	if (clearfunc) ClearUpdateFunction();
}

void Window::PushUpdateFunction(UpdateFunc function) {
	updatefunctionlist.push_back(function);
}

bool Window::DeleteUpdateFunction(IDentifier id) {
	if (updatefunctionlist.empty()) return false;
	list<UpdateFunc>::iterator i = updatefunctionlist.begin();
	while (i != updatefunctionlist.end()) {
		if ((*i).id == id) {
			updatefunctionlist.erase(i);
			return true;
		}
	}
	return false;
}

void Window::ClearUpdateFunction() {
	updatefunctionlist.clear();
}

void MarisaWin::Window::PushHandleMessageFunction(const HandleMessageFunc& function) {
	HandleMessage.push_back(function);
}

void MarisaWin::Window::DeleteHandleMessageFunction(IDentifier id) {
	for (list<HandleMessageFunc>::iterator i = HandleMessage.begin(); i != HandleMessage.end(); i++) {
		if (i->id == id) {
			list<HandleMessageFunc>::iterator buffer = i;
			buffer = i;
			buffer++;
			HandleMessage.erase(i);
			i = buffer;
		}
	}
}

void MarisaWin::Window::ClearHandleMessageFunction() {
	HandleMessage.clear();
}

void Window::UsingSmoothFrame(bool smooth_frame_) {
	smooth_frame = smooth_frame_;
}

void Window::SetFPS(USHORT fps_) {
	fps = fps_;
}

USHORT Window::GetFps() {
	return fps;
}

float Window::GetRealFps() {
	return real_fps;
}

void Window::EnableSizeBox(bool sizebox_) {
	bool sizebox = sizebox_;
	if (!created) return;
	HWND winHWnd = GetHWnd();
	if (sizebox) {
		SetWindowLongPtr(winHWnd, GWL_STYLE, GetWindowLongPtrW(winHWnd, GWL_STYLE) | WS_SIZEBOX);
	}
	else {
		SetWindowLongPtr(winHWnd, GWL_STYLE, GetWindowLongPtrW(winHWnd, GWL_STYLE) ^ WS_SIZEBOX);
	}
}

void Window::SetWindowName(wstring& windowname_) {
	windowname = windowname_;
	if (!created) return;
	SetWindowText(GetHWnd(), windowname_.c_str());
}

void MarisaWin::Window::SetWindowName(const wchar_t* windowname_) {
	windowname = windowname_;
	if (!created) return;
	SetWindowText(GetHWnd(), windowname_);
}

void Window::SetWindowSize(SIZE windowsize_) {
	size = windowsize_;
	if (!created) return;
	RECT rect;
	GetWindowRect(GetHWnd(), &rect);
	MoveWindow(GetHWnd(), rect.left, rect.top, windowsize_.cx, windowsize_.cy, true);
}

UpdateFunc* Window::GetFunction(IDentifier id) {
	if (updatefunctionlist.empty()) return NULL;
	list<UpdateFunc>::iterator i = updatefunctionlist.begin();
	while (i != updatefunctionlist.end()) {
		if ((*i).id == id) {
			return &*i;
		}
	}
	return NULL;
}