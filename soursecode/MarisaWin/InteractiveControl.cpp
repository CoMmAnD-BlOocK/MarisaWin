#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void InteractiveControl::HandleMessage(ExMessage* msg) {
	if (HandleMsgFunctions.empty()) return;
	list<function<void(void* pcontrol, ExMessage* msg)>>::iterator it = HandleMsgFunctions.begin();
	for (; it != HandleMsgFunctions.end(); it++) {
		if (*it != NULL) {
			(*it)(this, msg);
		}
	}
}