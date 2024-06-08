#include "MarisaWin.hpp"

void MarisaWin::Screen::PushControlFront(void* control)
{
	if (control == NULL) return;
	ControlList.push_front(control);
}

void MarisaWin::Screen::PushControlFront(size_t controlcount, ...)
{
	va_list controls;
	va_start(controls, controlcount);
	for (size_t i = 0; i < controlcount; i++) {
		void* buffer = va_arg(controls, void*);
		if (buffer != NULL)  ControlList.push_front(buffer);
	}
}

void MarisaWin::Screen::PushControlBack(void* control)
{
	if (control == NULL) return;
	ControlList.push_back(control);
}

void MarisaWin::Screen::PushControlBack(size_t controlcount, ...)
{
	va_list controls;
	va_start(controls, controlcount);
	for (size_t i = 0; i < controlcount; i++) {
		void* buffer = va_arg(controls, void*);
		if (buffer != NULL)  ControlList.push_back(buffer);
	}
}

void MarisaWin::Screen::PopControlFront()
{
	ControlList.pop_front();
}

void MarisaWin::Screen::PopControlBack()
{
	ControlList.pop_back();
}

void MarisaWin::Screen::AddControl(void* control, size_t where_)
{
	if (where_ >= ControlList.size()) return;
	if (control == NULL) return;

	list<void*>::iterator i = ControlList.begin();
	for (; where_ != 0; where_--) i++;
	ControlList.insert(i, control);
}

void MarisaWin::Screen::RemoveControl(void* control)
{
	for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++) {
		if (*i == control) {
			ControlList.erase(i);
			return;
		}
	}
}

void MarisaWin::Screen::RemoveControl(const wstring& controlname)
{
	for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++) {
		if (((Control*)(*i))->controlname == controlname) {
			ControlList.erase(i);
			return;
		}
	}
}

void MarisaWin::Screen::RemoveControl(size_t where_)
{
	if (where_ >= ControlList.size()) return;
	list<void*>::iterator i = ControlList.begin();
	for (; where_ != 0; where_--)i++;
	ControlList.erase(i);
}

void MarisaWin::Screen::SortControl(void* control, size_t where_)
{
	if (where_ >= ControlList.size()) return;
	list<void*>::iterator i = ControlList.begin();
	list<void*>::iterator newplace = ControlList.begin();

	for (; i != ControlList.end(); i++) {
		if (*i == control) break;
		if (i == ControlList.end()--) return;
	}

	for (; where_ != 0; where_--)newplace++;
	ControlList.erase(i);
	ControlList.insert(newplace, control);
}

void MarisaWin::Screen::SortControl(const wstring& controlname, size_t where_)
{
	if (where_ >= ControlList.size()) return;
	list<void*>::iterator i = ControlList.begin();
	list<void*>::iterator newplace = ControlList.begin();

	for (; i != ControlList.end(); i++) {
		if (((Control*)(*i))->controlname == controlname) break;
		if (i == ControlList.end()--) return;
	}

	for (; where_ != 0; where_--)newplace++;
	void* control = *i;
	ControlList.erase(i);
	ControlList.insert(newplace, control);
}

size_t MarisaWin::Screen::LookUpControl(void* control)
{
	size_t ret = 0;
	for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++,ret++) {
		if (*i == control) break;
	}
	return ret;
}

size_t MarisaWin::Screen::LookUpControl(const wstring& controlname)
{
	size_t ret = 0;
	for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++, ret++) {
		if (((Control*)(*i))->controlname == controlname) break;
	}
	return ret;
}

void* MarisaWin::Screen::LookUpControl(size_t where_)
{
	if (where_ >= ControlList.size()) return NULL;
	list<void*>::iterator i = ControlList.begin();
	for (; where_ != 0; where_--)i++;
	return *i;
}