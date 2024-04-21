#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void EditBox::SetText(wstring& text_) {
	text = text_;
}

void MarisaWin::EditBox::SetText(const wchar_t* text_) {
	text = text_;
}

void EditBox::SetSize(SIZE size_) {
	size = size_;
}

void EditBox::Settcolor(COLORARGB tcolor_) {
	tcolor = tcolor_;
}

void EditBox::Setstbcolor(COLORARGB stbcolor_) {
	stbcolor = stbcolor_;
}

void EditBox::Setbcolor(COLORARGB bcolor_) {
	bcolor = bcolor_;
}

void EditBox::Setccolor(COLORARGB ccolor_) {
	ccolor = ccolor_;
}

void EditBox::SetCursorPosition(ULONG position_) {
	cursor_position = position_;
}

void EditBox::SetChoosedCursorPosition(ULONG position_) {
	select_cursor_position = position_;
}

void EditBox::SetFont(LOGFONT& font_) {
	font = font_;
}

void EditBox::SetSettings(BYTE settings_) {
	settings = settings_;
}

const wstring& EditBox::GetText() {
	return text;
}

SIZE& EditBox::GetSize() {
	return size;
}

COLORARGB EditBox::Gettcolor() {
	return tcolor;
}

COLORARGB EditBox::Getctbcolor() {
	return stbcolor;
}

COLORARGB EditBox::Getbcolor() {
	return bcolor;
}

COLORARGB EditBox::Getccolor() {
	return ccolor;
}

ULONG EditBox::GetCursorPosition() {
	return cursor_position;
}

ULONG EditBox::GetChoosedCursorPosition() {
	return select_cursor_position;
}

const LOGFONT& EditBox::GetFont() {
	return font;
}

BYTE EditBox::GetSettings() {
	return settings;
}

void EditBox::Mw_Default_Handle_KeyMessage(EditBox* pcontrol, ExMessage* msg) {
	if (!(pcontrol->onfocus)) return;
	if (msg->message == WM_KEYDOWN) {
		switch (msg->vkcode) {
			case VK_LEFT:
			{
				if (pcontrol->cursor_position != 0)
					pcontrol->cursor_position--;
				pcontrol->select_cursor_position = pcontrol->cursor_position;
				break;
			}
			case VK_RIGHT:
			{
				if (pcontrol->cursor_position != pcontrol->text.length())
					pcontrol->cursor_position++;
				pcontrol->select_cursor_position = pcontrol->cursor_position;
				break;
			}
			case VK_UP:
			{
				if (pcontrol->settings & oneline) {
					pcontrol->select_cursor_position = pcontrol->cursor_position;
					break;
				}
				wstring* text_before_the_cursor = new wstring;
				wstring* text_lastrow = new wstring;
				UINT last_row_of_the_cursor = 0;
				for (UINT i = pcontrol->cursor_position; pcontrol->text[i] != L'\n'; i--) {
					if (i == 0) {
						pcontrol->select_cursor_position = pcontrol->cursor_position;
						return;
					}
					(*text_before_the_cursor) += pcontrol->text[i];
					last_row_of_the_cursor = i - 2;
				}
				for (UINT i = last_row_of_the_cursor; pcontrol->text[i] != L'\n'; i--) {
					if (i == 0) break;
					(*text_lastrow) += pcontrol->text[i];
				}

				IMAGE unnamed_variable;
				ChangeWorkingImage(true);
				SetWorkingImage(&unnamed_variable);
				settextstyle(&pcontrol->font);
				ChangeWorkingImage();

				int max_chars;
				SIZE* charsize = new SIZE;
				GetTextExtentExPoint(GetImageHDC(&unnamed_variable), text_lastrow->c_str(), text_lastrow->length(), GetTextWidth(pcontrol->font, *text_before_the_cursor), &max_chars, NULL, charsize);
				delete charsize;
				delete text_before_the_cursor;
				delete text_lastrow;

				for (; pcontrol->text[pcontrol->cursor_position] != L'\n'; pcontrol->cursor_position--);
				for (; pcontrol->text[pcontrol->cursor_position] != L'\n'; pcontrol->cursor_position--);
				pcontrol->cursor_position += max_chars + 1;
				pcontrol->select_cursor_position = pcontrol->cursor_position;
				break;
			}
			case VK_DOWN:
			{
				if (pcontrol->settings & oneline) {
					pcontrol->select_cursor_position = pcontrol->cursor_position;
					break;
				}
				wstring* text_before_the_cursor = new wstring;
				wstring* text_nextrow = new wstring;
				UINT next_row_of_the_cursor = 0;
				for (UINT i = pcontrol->cursor_position; pcontrol->text[i] != L'\n'; i--) *(text_before_the_cursor) += i;
				for (UINT i = pcontrol->cursor_position; pcontrol->text[i] != L'\n'; i++) next_row_of_the_cursor = i + 2;
				for (UINT i = next_row_of_the_cursor; pcontrol->text[i] != L'\n'; i++) {
					if (i == pcontrol->text.length()) {
						pcontrol->select_cursor_position = pcontrol->cursor_position;
						return;
					}
					(*text_nextrow) += pcontrol->text[i];
				}

				IMAGE unnamed_variable;
				ChangeWorkingImage(true);
				SetWorkingImage(&unnamed_variable);
				settextstyle(&pcontrol->font);
				ChangeWorkingImage();

				int max_chars;
				SIZE* charsize = new SIZE;
				GetTextExtentExPoint(GetImageHDC(&unnamed_variable), text_nextrow->c_str(), text_nextrow->length(), GetTextWidth(pcontrol->font, *text_before_the_cursor), &max_chars, NULL, charsize);
				delete charsize;
				delete text_nextrow;
				delete text_before_the_cursor;
				for (; pcontrol->text[pcontrol->cursor_position] != L'\n'; pcontrol->cursor_position++);
				pcontrol->cursor_position += max_chars + 1;
				pcontrol->select_cursor_position = pcontrol->cursor_position;
				break;
			}
			case VK_INSERT:
			{
				pcontrol->settings ^= rewrite;
			}
		}
	}
	else if (msg->message == WM_CHAR) {
		if (pcontrol->settings & cannotedit) return;
		if (msg->ch == L'\r') msg->ch = L'\n';
		if ((msg->ch == L'\n') && (pcontrol->settings & oneline)) return;

		if (pcontrol->cursor_position == pcontrol->select_cursor_position) {	//unsecting
			if (msg->ch == L'\x08') {		//backspace
				if (pcontrol->cursor_position != 0) {
					pcontrol->text.erase(pcontrol->cursor_position - 1, 1);
					pcontrol->cursor_position--;
					pcontrol->select_cursor_position--;
				}
				return;
			}
			else if (msg->ch == L'\x16') {	//Ctrl+V
				wstring clipboarddata;
				if (OpenClipboard(NULL)) {
					HANDLE hData = GetClipboardData(CF_TEXT);
					wchar_t* buffer = (wchar_t*)GlobalLock(hData);
					if (buffer != NULL) clipboarddata = buffer;
					GlobalUnlock(hData);
					CloseClipboard();
				}
				pcontrol->text.insert((size_t)pcontrol->cursor_position, (const wchar_t*)clipboarddata.c_str());
			}
			else if (msg->ch == L'\xff') {	//delete
				if (pcontrol->cursor_position != pcontrol->text.length())
					pcontrol->text.erase(pcontrol->cursor_position);
				return;
			}
			if (pcontrol->settings & rewrite)
				pcontrol->text[pcontrol->cursor_position] = msg->ch;
			else if (msg->ch >= L' ')	//Not control char
				pcontrol->text.insert((size_t)pcontrol->cursor_position, (const wchar_t*)&msg->ch);
			else return;
			pcontrol->cursor_position = pcontrol->select_cursor_position = pcontrol->cursor_position + 1;
		}

		else {
			if (pcontrol->cursor_position > pcontrol->select_cursor_position)
				pcontrol->cursor_position ^= pcontrol->select_cursor_position ^= pcontrol->cursor_position ^= pcontrol->select_cursor_position;

			if ((msg->ch == L'\x08') || (msg->ch == L'\xff')) {
				if (pcontrol->cursor_position != 0) {
					pcontrol->text.erase(pcontrol->cursor_position, pcontrol->select_cursor_position - pcontrol->cursor_position);
					pcontrol->select_cursor_position = pcontrol->cursor_position -= (pcontrol->select_cursor_position - pcontrol->cursor_position);
				}
				return;
			}
			else if (msg->ch == L'\x3') {	//Ctrl+C
				if (OpenClipboard(NULL)) {
					EmptyClipboard();
					HGLOBAL hClip = GlobalAlloc(GMEM_MOVEABLE, (((pcontrol->select_cursor_position - pcontrol->cursor_position) * 2) + 2));
					wchar_t* pBuf = (wchar_t*)GlobalLock(hClip);
					wmemcpy(pBuf, &pcontrol->text.c_str()[pcontrol->cursor_position], (pcontrol->select_cursor_position - pcontrol->cursor_position));
					GlobalUnlock(hClip);
					SetClipboardData(CF_UNICODETEXT, hClip);
					CloseClipboard();
				}
				return;
			}
			else if (msg->ch == L'\x16') {	//Ctrl+V
				wstring clipboarddata;
				if (OpenClipboard(NULL)) {
					HANDLE hData = GetClipboardData(CF_TEXT);
					wchar_t* buffer = (wchar_t*)GlobalLock(hData);
					if (buffer != NULL) clipboarddata = buffer;
					GlobalUnlock(hData);
					CloseClipboard();
				}
				pcontrol->text.replace(pcontrol->cursor_position, pcontrol->select_cursor_position - pcontrol->cursor_position, clipboarddata.c_str());
			}
			else if (msg->ch >= L' ')	//Not control char
				pcontrol->text.replace(pcontrol->cursor_position, pcontrol->select_cursor_position - pcontrol->cursor_position, (const wchar_t*)&msg->ch);
			else return;
			pcontrol->cursor_position = pcontrol->select_cursor_position = pcontrol->cursor_position + 1;
		}
	}
}

void EditBox::Mw_Default_Handle_MouseMessage(EditBox* pcontrol, ExMessage* msg) {
	enum mouse_state_ {
		nonestate	= 0,
		lbdown		= 1,
		lbmove		= 2,
	};
	static BYTE mouse_state = 0;

	switch (msg->message) {
		case WM_LBUTTONDOWN:
		{
			if (InRect(COORD{ msg->x,msg->y }, RECT{ pcontrol->coord.X,pcontrol->coord.Y,pcontrol->coord.X + pcontrol->size.cx,pcontrol->coord.Y + pcontrol->size.cy })) {
				pcontrol->onfocus = true;
				pcontrol->select_cursor_position = pcontrol->cursor_position = pcontrol->Coord2CursorPosition(COORD{ msg->x,msg->y });
				mouse_state = lbdown;
			}
			else {
				mouse_state = nonestate;
				pcontrol->onfocus = false;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if ((mouse_state == lbdown) || (mouse_state == lbmove)) {
				mouse_state = lbmove;
				pcontrol->select_cursor_position = pcontrol->Coord2CursorPosition(COORD{ msg->x,msg->y });
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			if (mouse_state == lbmove) {
				mouse_state = nonestate;
				return;
			}
			mouse_state = nonestate;
			pcontrol->onfocus = InRect(COORD{ msg->x,msg->y }, RECT{ pcontrol->coord.X,pcontrol->coord.Y,pcontrol->coord.X + pcontrol->size.cx,pcontrol->coord.Y + pcontrol->size.cy });
			if (pcontrol->onfocus) pcontrol->cursor_position = pcontrol->select_cursor_position = pcontrol->Coord2CursorPosition(COORD{ msg->x,msg->y });
			break;
		}
	}
}

UINT EditBox::Coord2CursorPosition(COORD ccoord) {
	//Calculate row
	if (text.empty()) return 0;
	if (!InRect(ccoord, RECT{ coord.X,coord.Y,coord.X + size.cx,coord.Y + size.cy })) return cursor_position;
	ccoord.X = ccoord.X - coord.X;
	ccoord.Y = ccoord.Y - coord.Y;

	UINT row = ccoord.Y / font.lfHeight;
	UINT cursor_row = 0;
	for (; row != 0; cursor_row++) {
		if (text[cursor_row] == L'\n') row--;
		if (text[cursor_row] == L'\0') return text.length();
	}

	//Calculate column
	wstring row_string;
	for (UINT i = cursor_row;; i++) {
		if ((text[i] == L'\n') || (text[i] == L'\0')) {
			break;
		}
		row_string += text[i];
	}

	IMAGE buffer;
	ChangeWorkingImage(true);
	SetWorkingImage(&buffer);
	settextstyle(&font);
	ChangeWorkingImage();
	int cursor_column = 0;
	SIZE text_size;
	GetTextExtentExPoint(GetImageHDC(&buffer), row_string.c_str(), row_string.length(), ccoord.X, &cursor_column, NULL, &text_size);
	return cursor_row + cursor_column;
}

void EditBox::Print() {
	if (hide) return;
	Picture picture(size);
	PictureFrame buffer(coord, id, &picture, size, alpha);

	ChangeWorkingImage(true);
	SetWorkingImage(&picture.GetImage());
	settextstyle(&font);
	settextcolor(BGR(tcolor));
	setfillcolor(BGR(bcolor));
	solidrectangle_alpha(0, 0, size.cx, size.cy);

	//draw the background of the choosed text

	if (cursor_position != select_cursor_position) {
		bool swaped = false;
		if (cursor_position >= select_cursor_position) {
			cursor_position ^= select_cursor_position ^= cursor_position ^= select_cursor_position;
			swaped = true;
		}

		UINT cursor = cursor_position;
		
		wstring text_thisrow;
		COORD lastrow = CursorPosition2Coord(cursor_position);
		lastrow.Y -= font.lfHeight;
		setfillcolor(BGR(stbcolor));
		for (UINT i = cursor_position; i <= select_cursor_position; i++) {
			if ((text[i] == L'\n') || (i == select_cursor_position)) {
				solidrectangle_alpha(lastrow.X, lastrow.Y,
					lastrow.X + GetTextWidth(font, text_thisrow),
					lastrow.Y + font.lfHeight);
				text_thisrow.clear();
				lastrow.X = 0;
				lastrow.Y += font.lfHeight;
				continue;
			}
			text_thisrow += text[i];
		}

		if (swaped) cursor_position ^= select_cursor_position ^= cursor_position ^= select_cursor_position;
	}

	//Print the text
	if (((settings & hidetext) == 0) && (!text.empty())) {
		settextcolor(BGR(tcolor));
		drawtext_alpha(text.c_str(), RECT{0,0,size.cx,size.cy});
	}

	//Draw the cursor
	if (((settings & hidecursor) == 0) && onfocus) {
		setlinecolor(ccolor);
		COORD cursor_coord = CursorPosition2Coord(cursor_position);
		line_alpha(
			cursor_coord.X,
			cursor_coord.Y,
			cursor_coord.X,
			cursor_coord.Y - font.lfHeight);
	}

	ChangeWorkingImage();
	buffer.Print();
}