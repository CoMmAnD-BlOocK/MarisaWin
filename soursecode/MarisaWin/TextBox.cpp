#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void TextBox::SetSize(SIZE size_) {
	size = size_;
}

void TextBox::Settcolor(COLORARGB color) {
	tcolor = color;
}

void TextBox::SetText(wstring& text_){
	text = text_;
}

void TextBox::SetFont(LOGFONT& font_) {
	font = font_;
}

const wstring& TextBox::GetText(){
	return text;
}

void TextBox::GetText(LPWSTR ret, size_t charnum){
	wcscpy_s(ret, charnum, text.c_str());
}

SIZE TextBox::GetSize(){
	return size;
}

COLORARGB TextBox::Gettcolor(){
	return tcolor;
}

LOGFONT& TextBox::GetFont() {
	return font;
}

void TextBox::Print(){
	if (hide) return;
	Canvas printer = Canvas(coord, id, size, alpha);
	printer.SetDrawFunction([&]() {
		settextstyle(&font);
		settextcolor(tcolor);
		drawtext_alpha((LPWSTR)text.c_str(), RECT{ 0,0,size.cx,size.cy });
		});
	printer.Print();
}