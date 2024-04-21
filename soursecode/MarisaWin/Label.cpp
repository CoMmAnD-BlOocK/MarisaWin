#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void Label::Print() {
	if (hide) return;
	wstring buffer_text;
	for (size_t i = 0; i < text.length() + 1; i++) {
		if (text[i] == '\n') {
			buffer_text += L'\0';
			break;
		}
		buffer_text += text[i];
	}

	IMAGE fontimage;
	ChangeWorkingImage(true);
	SetWorkingImage(&fontimage);
	settextstyle(&font);
	UINT textwidth = GetTextWidth(font, text.c_str());

	IMAGE buffer(textwidth, font.lfHeight);
	SetWorkingImage(&buffer);
	setfillcolor(BGR(bcolor));
	solidrectangle_alpha(0, 0, textwidth, font.lfHeight);
	settextcolor(BGR(tcolor));
	settextstyle(&font);
	outtextxy_alpha(0, 0, text.c_str());
	ChangeWorkingImage();

	transparentimage(GetWorkingImage(), coord.X, coord.Y, &buffer, alpha);
}

void Label::SetFont(wstring& font_) {
	memset(font.lfFaceName, 0, sizeof(font.lfFaceName));
	if (font_.size() <= 32) wmemcpy(font.lfFaceName, font_.c_str(), font_.size());
	else {
		wmemcpy(font.lfFaceName, font_.c_str(), 31);
		font.lfFaceName[31] = L'\0';
	}
}

void Label::SetText(wstring& text_) {
	text = text_;
}

void Label::Settcolor(COLORARGB color) {
	tcolor = color;
}

void Label::Setbcolor(COLORARGB color) {
	bcolor = color;
}

const wstring& Label::GetText() {
	return text;
}

LOGFONT Label::GetFont() {
	return font;
}

COLORARGB Label::GetTextColor() {
	return tcolor;
}

COLORARGB Label::GetBackgroundColor() {
	return bcolor;
}