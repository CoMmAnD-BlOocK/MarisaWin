#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void Screen::AddControl(void* control) {
	controllist.push_back(control);
}

void Screen::AddControl(void* control, USHORT layer) {
	if (layer >= controllist.size()) return;
	list<void*>::iterator i = controllist.begin();
	for (USHORT i_ = 0; i_ < layer; i_++) i++;
	controllist.insert(i, control);
}

void Screen::RemoveControl(IDentifier IDentifier) {
	list<void*>::iterator i = controllist.begin();
	while (i != controllist.end()) {
		if (static_cast<Control*>(*i)->GetIDentifier() == IDentifier) {
			list<void*>::iterator buffer = i;
			buffer++;
			controllist.erase(i);
			i = buffer;
		}
		i++;
	}
}

void Screen::RemoveControl(USHORT layer) {
	if (layer >= controllist.size()) return;
	list<void*>::iterator i = controllist.begin();
	for (USHORT i_ = 0; i_ < layer; i_++) i++;
	controllist.erase(i);
}

void Screen::AdjustControl(USHORT layer, USHORT newlayer) {
	if (layer == newlayer) return;
	if ((layer >= controllist.size()) || (newlayer >= controllist.size())) return;
	list<void*>::iterator i = controllist.begin();
	list<void*>::iterator i_new = controllist.begin();
	for (USHORT i_ = 0; i_ < layer; i_++) i++;
	for (USHORT i_ = 0; i_ < newlayer - 1; i_++) i_new++;
	void* buffer = *i;
	controllist.erase(i);
	controllist.insert(i_new, buffer);
}

void Screen::AdjustControl(IDentifier layer, IDentifier newlayer) {
	if (layer == newlayer) return;
	list<void*>::iterator i = controllist.begin();
	list<void*>::iterator i_new = controllist.begin();
	while (i != controllist.end()) {
		if (static_cast<Control*>(*i)->GetIDentifier() == layer) break;
		i++;
	}
	while (i_new != controllist.end()) {
		if (static_cast<Control*>(*i_new)->GetIDentifier() == layer) break;
		i_new++;
	}
	if (i == i_new) return;
	void* buffer = *i;
	controllist.erase(i);
	controllist.insert(i_new, buffer);
}

void Screen::Print() {
	if (controllist.empty()) return;
	list<void*>::reverse_iterator i = controllist.rbegin();
	while (i != controllist.rend()) {
		static_cast<Control*>(*i)->Print();
		i++;
	}
}

void Screen::AddX(SHORT x) {
	if (controllist.empty()) return;
	list<void*>::reverse_iterator i = controllist.rbegin();
	while (i != controllist.rend()) {
		static_cast<Control*>(*i)->AddX(x);
		i++;
	}

}

void Screen::AddY(SHORT y) {
	if (controllist.empty()) return;
	list<void*>::reverse_iterator i = controllist.rbegin();
	while (i != controllist.rend()) {
		static_cast<Control*>(*i)->AddY(y);
		i++;
	}
}

void Screen::SetAlpha(BYTE alpha) {
	if (controllist.empty()) return;
	list<void*>::reverse_iterator i = controllist.rbegin();
	while (i != controllist.rend()) {
		static_cast<Control*>(*i)->SetAlpha(alpha);
		i++;
	}
}

void Screen::AddAlpha(BYTE alpha) {
	if (controllist.empty()) return;
	list<void*>::reverse_iterator i = controllist.rbegin();
	while (i != controllist.rend()) {
		static_cast<Control*>(*i)->AddAlpha(alpha);
		i++;
	}
}

void Screen::SubAlpha(BYTE alpha) {
	if (controllist.empty()) return;
	list<void*>::reverse_iterator i = controllist.rbegin();
	while (i != controllist.rend()) {
		static_cast<Control*>(*i)->SubAlpha(alpha);
		i++;
	}
}

void Screen::DispatchMessageEW(ExMessage* msg) {
	if (controllist.empty()) return;
	list<void*>::iterator i = controllist.begin();
	for (; i != controllist.end(); i++)
		if (dynamic_cast<InteractiveControl*>(static_cast<Control*>(*i)) != NULL)
			static_cast<InteractiveControl*>(*i)->HandleMessage(msg);
}