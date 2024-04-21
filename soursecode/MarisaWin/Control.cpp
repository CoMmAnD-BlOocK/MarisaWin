#include "..\MarisaWin.hpp"
using namespace MarisaWin;

IDentifier Control::GetIDentifier() {
	return id;
}

void Control::SetIDentifer(IDentifier id) {
	id = id;
}

void Control::SetCoord(COORD coord_) {
	coord = coord_;
}

COORD Control::GetCoord() {
	return coord;
}

void Control::SetAlpha(BYTE alpha_) {
	alpha = alpha_;
}

void Control::AddAlpha(BYTE alpha_) {
	if (0xff - alpha < alpha_) alpha = 255;
	else alpha += alpha_;
}

void Control::SubAlpha(BYTE alpha_) {
	if (alpha < alpha_) alpha = 0;
	else alpha -= alpha_;
}

BYTE Control::GetAlpha() {
	return alpha;
}

void MarisaWin::Control::AddX(SHORT x_) {
	coord.X += x_;
}

void MarisaWin::Control::AddY(SHORT y_) {
	coord.Y += y_;
}

void Control::SetHide(bool ishide) {
	hide = ishide;
}