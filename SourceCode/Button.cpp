#include "MarisaWin.hpp"
using namespace MarisaWin;

SizeF Button::GetControlSize()
{
	return SizeF(picture->GetWidth(), picture->GetHeight());
}

void Button::Control_Draw(Graphics& graphics)
{
	graphics.DrawImage(picture, PointF(0, 0));
}
