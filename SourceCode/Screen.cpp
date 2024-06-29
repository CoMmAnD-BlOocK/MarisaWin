#include "MarisaWin.hpp"
using namespace MarisaWin;

void Screen::SortControl(void* control, size_t where_)
{
	if (ControlList.empty()) return;
	if (where_ >= ControlList.size()) return;
	auto i = ControlList.begin();
	auto newplace = ControlList.begin();

	for (; i != ControlList.end(); i++) {
		if (*i == control) break;
		if (i == --ControlList.end()) return;
	}

	for (; where_ != 0; where_--)newplace++;
	ControlList.erase(i);
	ControlList.insert(newplace, control);
}

void Screen::SortControl(const wstring& controlname, size_t where_)
{
	if (ControlList.empty()) return;
	if (where_ >= ControlList.size()) return;
	auto i = ControlList.begin();
	auto newplace = ControlList.begin();

	for (; i != ControlList.end(); i++) {
		if (((Control*)(*i))->controlname == controlname) break;
		if (i == --ControlList.end()) return;
	}

	for (; where_ != 0; where_--)newplace++;
	void* control = *i;
	ControlList.erase(i);
	ControlList.insert(newplace, control);
}

size_t Screen::LookUpControl(void* control)
{
	if (ControlList.empty()) return -1;
	size_t ret = 0;
	for (auto i = ControlList.begin(); i != ControlList.end(); i++,ret++) {
		if (*i == control) break;
		if (i == --ControlList.end()) return -1;
	}
	return ret;
}

size_t Screen::LookUpControl(const wstring& controlname)
{
	if (ControlList.empty()) return -1;
	size_t ret = 0;
	for (auto i = ControlList.begin(); i != ControlList.end(); i++, ret++) {
		if (((Control*)(*i))->controlname == controlname) break;
		if (i == --ControlList.end()) return -1;
	}
	return ret;
}

void* Screen::LookUpControl(size_t where_)
{
	if (ControlList.empty()) return NULL;
	if (where_ >= ControlList.size()) return NULL;
	auto i = ControlList.begin();
	for (; where_ != 0; where_--)i++;
	return *i;
}

bool Screen::DeleteControl(void* control)
{
	if (ControlList.empty()) return false;
	for (auto i = ControlList.begin(); i != ControlList.end(); i++) {
		if ((*i) = control) {
			delete ((Control*)control);
			ControlList.erase(i);
			return true;
		}
	}
	return false;
}

bool Screen::DeleteControl(const wstring& controlname)
{
	if (ControlList.empty()) return false;
	for (auto i = ControlList.begin(); i != ControlList.end(); i++) {
		if (((Control*)(*i))->controlname == controlname) {
			delete ((Control*)(*i));
			ControlList.erase(i);
			return true;
		}
	}
	return false;
}

bool Screen::DeleteControl(size_t where_)
{
	if (ControlList.empty()) return false;
	auto i = ControlList.begin();
	for (; where_ != 0; where_--) {
		if (i == ControlList.end()) return false;
		i++;
	}
	delete ((Control*)(*i));
	ControlList.erase(i);
	return true;
}

void Screen::DrawScreen(Graphics& g)
{
	if (!ControlList.empty())
		for (auto i = ControlList.begin(); i != ControlList.end(); i++) {
			((Control*)(*i))->Draw(g);
		}
}

void Screen::DeleteScreen()
{
	if (ControlList.empty()) return;
	for (auto i = ControlList.begin(); i != ControlList.end(); i++) {
		delete ((Control*)(*i));
	}
	ControlList.clear();
}

void Screen::EraseControl(void* control)
{
	if (ControlList.empty()) return;
	for (auto i = ControlList.begin(); i != ControlList.end(); i++) {
		if ((*i) = control) {
			//The control has been destructed just now.
			ControlList.erase(i);
			return;
		}
	}
}

void Screen::MW_DispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!ControlList.empty())
		for (auto i = ControlList.begin(); i != ControlList.end(); i++) {
			((Control*)(*i))->HandleMessage(hWnd, message, wParam, lParam);
		}
}

bool Screen::AddControl(void* control)
{
	if (LookUpControl(((Control*)control)->controlname) != -1) return false;
	ControlList.push_front(control);
	return true;
}