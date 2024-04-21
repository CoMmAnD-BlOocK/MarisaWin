#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void MarisaWin::Button::Print() {
	if (hide) return;
	PictureFrame(coord, id, picture, SIZE{ picture->GetImage().getwidth(),picture->GetImage().getheight() }).Print();
}