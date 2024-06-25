#include "MarisaWin.hpp"

SizeF MarisaWin::Button::GetControlSize()
{
	return SizeF(picture->GetWidth(), picture->GetHeight());
}

void MarisaWin::Button::Control_Draw(Graphics& graphics)
{
	graphics.DrawImage(picture, PointF(0, 0));
}
