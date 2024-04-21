#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void Button::SetImage(Picture* picture_) {
	picture = picture_;
}

const Picture* MarisaWin::Button::GetImage() {
	return picture;
}

void MarisaWin::Button::Print() {
	PictureFrame(coord, id, picture, SIZE{ picture->GetImage().getwidth(),picture->GetImage().getheight() }).Print();
}