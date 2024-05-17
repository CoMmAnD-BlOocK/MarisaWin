#include "MarisaWin.hpp"
void MarisaWin::Label::Control_Draw(Graphics& graphics)
{
	//Bitmap buffer()
	
	SolidBrush tbrush(textcolor.textcolor);
	SolidBrush bbrush(textcolor.backgroundcolor);
	graphics.FillRectangle(&bbrush,Rect())
	graphics.DrawString(text.c_str(), text.length(), font, coord, &tbrush);
}