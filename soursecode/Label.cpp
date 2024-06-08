#include "MarisaWin.hpp"
void MarisaWin::Label::Control_Draw(Graphics& graphics)
{
	SizeF size = GetTextBounds(font, 0, text);
	Bitmap buffer(size.Width, size.Height);
	Graphics buffergraphics(&buffer);
	
	for (auto i = UserDrawFuncBC.begin(); i != UserDrawFuncBC.end(); i++)
		(*i)(buffergraphics);
	SolidBrush tbrush(textcolor.textcolor);
	SolidBrush bbrush(textcolor.backgroundcolor);
	buffergraphics.FillRectangle(&bbrush,Rect());
	buffergraphics.DrawString(text.c_str(), text.length(), font, PointF(0, 0), &tbrush);
	for (auto i = UserDrawFuncAC.begin(); i != UserDrawFuncAC.end(); i++)
		(*i)(buffergraphics);
	graphics.DrawImage(&buffer, coord);
}