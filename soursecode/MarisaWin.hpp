/*
	
	   _____               .__               __      __.__        
	  /     \ _____ _______|__| ___________ /  \    /  \__| ____  
	 /  \ /  \\__  \\_  __ \  |/  ___/\__  \\   \/\/   /  |/    \ 
	/    Y    \/ __ \|  | \/  |\___ \  / __ \\        /|  |   |  \
	\____|__  (____  /__|  |__/____  >(____  /\__/\  / |__|___|  /
			\/     \/              \/      \/      \/          \/ 
	
	MarisaWin UI Library for C++
	Version: 0.01 ///JUST FOR INFORMATION COMPETITION/// (Update Date: 2024/4/7 Sun)
	TIPS: Please install easyx library first. URL: http://www.easyx.com
	YOU MUST FOLLOW "GPL License" WHEN YOU USE "Marisa Win Library"
*/

#pragma once
#include <easyx.h>
#include <windows.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")

#include <list>
#include <initializer_list>
#include <vector>
#include <string>

#include <time.h>
#include <conio.h>
#include <cmath>
#include <memory>
#include <functional>
#include <mmsystem.h>
#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib,"Winmm.lib")

#define Radian(angle) (PI / 180.0 * (angle))
#define Fps(MilliSecond, fps) ((MilliSecond) / (1000 / (fps)))
#define MilliSecond(frame,fps) ((int)(1000.0 / fps * frame))
#define BlenderAlpha(RGB,alpha) ((RGB) | ((alpha) << 24))
#define SetAlpha_ARGB(ARGB,alpha) ((ARGB >> 8) | (alpha << 24))
#define GetAlpha_ARGB(ARGB) ((ARGB) >> 24)
#define GetRGB_ARGB(ARGB) ((ARGB) & 0x00ffffff)

#define ilist initializer_list
#define nullfunc ([]() {})
#define PI (3.14159265359)

#pragma warning(disable:26495)
#pragma warning(disable:28251)
#pragma warning(disable:6031)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#define _CRT_SECURE_NO_WARNINGS

#define DEBUGLOG OutputDebugString

namespace MarisaWin{
	using namespace std;
	typedef UINT IDentifier;
	using PVFUNC = void(*)();
	using COLORRGB = COLORREF;

	typedef DWORD COLORARGB;

	struct UpdateFunc {         //Execute in the window loop
		IDentifier id;
		UINT frame_countdown;
		USHORT repeat_times;
		UINT repeat_interval;
		function<void()> function;	//Update Function
		bool static_ = false;
	};

	#define StaticUpdateFunc(id, frame_countdown, repeat_interval, function) UpdateFunc {\
	(id),\
	(frame_countdown),\
	1,\
	(repeat_interval),\
	(function),\
	true}

	struct HandleMessageFunc {
		IDentifier id;
		function<void(ExMessage*)> function;
	};

	//See https://codebus.cn/lostperson/rotate
	void rotateimage_alpha(IMAGE* dst, IMAGE* src, double rad);

	//See https://codebus.cn/yangw/transparent-putimage
	void transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, BYTE alpha_ex = 255);

	void ChangeWorkingImage(bool changedevice = false, IMAGE* device = GetWorkingImage());

	void arc_alpha(int left, int top, int right, int bottom, double stangle, double endangle);

	void circle_alpha(int x, int y, int radius);

	void ellipse_alpha(int left, int top, int right, int bottom);

	void fillcircle_alpha(int x, int y, int radius);

	void fillellipse_alpha(int left, int top, int right, int bottom);

	void fillpie_alpha(int left, int top, int right, int bottom, double stangle, double endangle);

	void fillpolygon_alpha(const POINT* points, int num);

	void fillrectangle_alpha(int left, int top, int right, int bottom);

	void fillroundrect_alpha(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);

	COLORARGB getpixel_alpha(int x, int y);

	void line_alpha(int x1, int y1, int x2, int y2);

	void pie_alpha(int left, int top, int right, int bottom, double stangle, double endangle);

	void polybezier_alpha(const POINT* points, int num);

	void polygon_alpha(const POINT* points, int num);

	void polyline_alpha(const POINT* points, int num);

	void putpixel_alpha(int x, int y, COLORARGB color);

	void rectangle_alpha(int left, int top, int right, int bottom);

	void roundrect_alpha(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);

	void solidcircle_alpha(int x, int y, int radius);

	void solidellipse_alpha(int left, int top, int right, int bottom);

	void solidpie_alpha(int left, int top, int right, int bottom, double stangle, double endangle);

	void solidpolygon_alpha(const POINT* points, int num);

	void solidrectangle_alpha(int left, int top, int right, int bottom);

	void solidroundrect_alpha(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);

	void outtextxy_alpha(int x, int y, const wchar_t* str);

	void drawtext_alpha(const wchar_t* str, RECT rect);

	inline bool InRect(COORD coord, RECT rect) {
		if ((coord.X > rect.left) && (coord.X < rect.right) && (coord.Y > rect.top) && (coord.Y < rect.bottom))
			return true;
		return false;
	};

	inline UINT GetTextWidth(LOGFONT& font, wstring& text) {
		IMAGE image;
		ChangeWorkingImage(true);
		SetWorkingImage(&image);
		settextstyle(&font);
		ChangeWorkingImage();
		SizeF layout(FLT_MAX, FLT_MAX);
		Font f(GetImageHDC(&image));
		Graphics g(GetImageHDC(&image));
		SizeF ret;
		g.MeasureString(text.c_str(), -1, &f, layout, NULL, &ret);
		return ret.Width;
	}

	inline UINT GetTextWidth(LOGFONT& font, const wchar_t* text) {
		IMAGE image;
		ChangeWorkingImage(true);
		SetWorkingImage(&image);
		settextstyle(&font);
		ChangeWorkingImage();
		SizeF layout(FLT_MAX, FLT_MAX);
		Font f(GetImageHDC(&image));
		Graphics g(GetImageHDC(&image));
		SizeF ret;
		g.MeasureString(text, -1, &f, layout, NULL, &ret);
		return ret.Width;
	}

	inline LOGFONT simple_textstyle(LONG height, LONG weight, wstring& faceName) {
		LOGFONT ret{};
		ret.lfHeight = height;
		ret.lfWeight = weight;
		ret.lfFaceName[31] = L'\0';
		if (faceName.size() <= 32) wmemcpy(ret.lfFaceName, faceName.c_str(), faceName.size());
		else {
			wmemcpy(ret.lfFaceName, faceName.c_str(), 31);
		}
		ret.lfQuality = ANTIALIASED_QUALITY;
		return ret;
	}

	class Control {
	public:
		virtual IDentifier GetIDentifier();
		void SetIDentifer(IDentifier id);
		void SetCoord(COORD coord_);
		virtual COORD GetCoord();

		virtual void SetAlpha(BYTE alpha_);
		virtual void AddAlpha(BYTE alpha_);
		virtual void SubAlpha(BYTE alpha_);
		virtual BYTE GetAlpha();

		virtual void AddX(SHORT x_);
		virtual void AddY(SHORT y_);

		virtual void Print() = 0;

		virtual void SetHide(bool ishide);
	protected:
		bool hide = false;
		IDentifier id;
		COORD coord;
		BYTE alpha;
	};

	class Screen {
	public:
		void AddControl(void* control);
		void AddControl(void* control, USHORT layer);
		void RemoveControl(IDentifier IDentifier);
		void RemoveControl(USHORT layer);

		void AdjustControl(USHORT layer, USHORT newlayer);
		void AdjustControl(IDentifier layer, IDentifier newlayer);

		void Print();
		void AddX(SHORT x);
		void AddY(SHORT y);

		void SetAlpha(BYTE alpha);
		void AddAlpha(BYTE alpha);
		void SubAlpha(BYTE alpha);

		void DispatchMessageEW(ExMessage* msg);
	private:
		list<void*> controllist;
	};

	class Window {
	public:
		Window(SIZE size_, const wstring& windowname_, Screen* screen = NULL, bool sizebox_ = true, USHORT fps_ = 60) {
			windowname = windowname_;
			sizebox = sizebox_;
			scrnow = screen;
			size = size_;
			fps = fps_;
		}

		void RunInstance();
		void CloseWindow();
		void ChangeScreen(Screen* screen, bool clearfunc = true);

		void PushUpdateFunction(UpdateFunc function);
		bool DeleteUpdateFunction(IDentifier id);
		void ClearUpdateFunction();

		void PushHandleMessageFunction(const HandleMessageFunc& function);
		void DeleteHandleMessageFunction(IDentifier id);
		void ClearHandleMessageFunction();

		void UsingSmoothFrame(bool smooth_frame_ = true);
		void SetFPS(USHORT fps_);
		USHORT GetFps();
		float GetRealFps();

		void EnableSizeBox(bool sizebox_);
		void SetWindowName(wstring& windowname_);
		void SetWindowName(const wchar_t* windowname_);
		void SetWindowSize(SIZE windowsize_);
		UpdateFunc* GetFunction(IDentifier id);
		list<function<void()>> highfreq_functionlist;

	private:
		float real_fps;
		bool smooth_frame = true;
		wstring windowname;
		bool sizebox;
		SIZE size;
		bool created;

		list<UpdateFunc> updatefunctionlist;
		USHORT fps;
		bool exit = false;
		list<HandleMessageFunc> HandleMessage;
		Screen* scrnow;
	};

	class Label :public Control {
	public:
		Label(COORD coord_, wstring& text_, LOGFONT& textstyle_, IDentifier id_, COLORARGB tcolor_ = 0xff000000, COLORARGB bcolor_ = 0x00000000, BYTE alpha_ = 255) {
			coord = coord_;
			text = text_;
			tcolor = tcolor_;
			bcolor = bcolor_;
			font = textstyle_;
			alpha = alpha_;
			id = id_;
		}
		void Print();

		void SetFont(wstring& font_);
		void SetText(wstring& text);
		void Settcolor(COLORARGB color);
		void Setbcolor(COLORARGB color);

		const wstring& GetText();
		LOGFONT GetFont();
		COLORARGB GetTextColor();
		COLORARGB GetBackgroundColor();
	private:
		wstring     text;       //The text for display
		LOGFONT     font;
		COLORARGB   tcolor;     //Text color
		COLORARGB   bcolor;     //Background color
	};

	class Picture {
	public:
		Picture(SIZE size = SIZE{ 0, 0 }, BYTE alpha_ = 255) {
			origin_image = IMAGE(size.cx, size.cy);
			alpha = alpha_;
		}

		Picture(const IMAGE& image, BYTE alpha_ = 255) {
			origin_image = image;
			alpha = alpha_;
		}

		void SetImage(const IMAGE& image);
		IMAGE& GetImage();
		
		void SetAlpha(const BYTE alpha_);
		BYTE GetAlpha();

		void operator =(const IMAGE& image) {
			origin_image = image;
		}
	private:
		IMAGE origin_image = IMAGE(0,0);
		BYTE alpha = 255;
	};

	class PictureFrame :public Control {
	public:
		Picture* picture;
		PictureFrame(COORD coord_, IDentifier id_, Picture* picture_, SIZE size_ = { 0,0 }, BYTE alpha_ = 255) {
			coord = coord_;
			id = id_;
			picture = picture_;
			size = size_;
			alpha = alpha_;
		}

		void operator =(Picture* picture_) {
			picture = picture_;
		}

		void SetSize(SIZE size_);
		SIZE GetSize();

		void Print();
	private:
		SIZE size;
	};

	class Canvas :public Control {
	public:
		Canvas(SIZE size_, function<void()> draw_ = []() {}) {
			image = IMAGE{ size_.cx,size_.cy };
			draw = draw_;
		}

		Canvas(COORD coord_, IDentifier id_, SIZE size_ = SIZE{0, 0}, BYTE alpha_ = 255) {
			coord = coord_;
			alpha = alpha_;
			id = id_;
			image = IMAGE{ size_.cx,size_.cy };
		}
		void Print();

		void SetDrawFunction(function<void()> draw_);

		void SetSize(SIZE size_);
		SIZE GetSize();

		void ClearCanvas();
		void CanvaDefaults();
		void Refresh();

		const IMAGE& GetImage();

		bool refresh_after_print = true;
	private:
		function<void()> draw;
		IMAGE image;
	};

	class TextBox :public Control {
	public:
		TextBox(COORD coord_, IDentifier id_, LOGFONT& font_, SIZE size_ = SIZE{ 0, 0 }, BYTE alpha_ = 255) {
			coord = coord_;
			alpha = alpha_;
			id = id_;
			size = size_;
			font = font_;
		}

		TextBox() {
			text = L"";
		}

		void SetSize(SIZE size_);
		void Settcolor(COLORARGB color);
		void SetText(wstring& text_);
		void SetFont(LOGFONT& font_);

		const wstring& GetText();
		void GetText(LPWSTR ret, size_t charnum);
		SIZE GetSize();
		COLORARGB Gettcolor();
		LOGFONT& GetFont();
	
		void Print();
	private:
		SIZE		size;
		COLORARGB	tcolor = 0;
		wstring		text;
		LOGFONT		font;
	};

	template<class T>
	class InteractiveControl :public Control {
	public:
		virtual void HandleMessage(ExMessage* msg);
		virtual void Print() = 0;
		list<function<void(T* pcontrol,ExMessage* msg)>> HandleMsgFunctions;
	};

	class EditBox :public InteractiveControl<EditBox> {
	public:
		enum settings_ {
			hidecursor = 1,
			oneline = 2,
			cannotedit = 4,
			hidetext = 8,
			rewrite = 16
		};

		EditBox(COORD coord_, IDentifier id_, LOGFONT& font_, SIZE size_ = SIZE{ 0, 0 }, BYTE alpha_ = 255) {
			coord = coord_;
			alpha = alpha_;
			id = id_;
			size = size_;
			font = font_;
			HandleMsgFunctions.push_back(
				[&](EditBox* pcontrol, ExMessage* msg) {
					Mw_Default_Handle_KeyMessage(pcontrol, msg);
					Mw_Default_Handle_MouseMessage(pcontrol, msg);
				});
		}

		EditBox() {
			coord = COORD{ 0,0 };
			alpha = 255;
			id = 0;
			size = SIZE{ 0,0 };
			gettextstyle(&font);
			HandleMsgFunctions.push_back(
				[&](EditBox* pcontrol, ExMessage* msg) {
					Mw_Default_Handle_KeyMessage(pcontrol, msg);
					Mw_Default_Handle_MouseMessage(pcontrol, msg);
				});
		}

		void SetText(wstring& text_);
		void SetText(const wchar_t* text_);
		void SetSize(SIZE size_);
		void Settcolor(COLORARGB tcolor_);
		void Setstbcolor(COLORARGB ctbcolor_);
		void Setbcolor(COLORARGB bcolor_);
		void Setccolor(COLORARGB ccolor_);
		void SetCursorPosition(ULONG position_);
		void SetChoosedCursorPosition(ULONG position_);
		void SetFont(LOGFONT& font_);
		void SetSettings(BYTE settings_);

		const wstring& GetText();
		SIZE& GetSize();
		COLORARGB Gettcolor();
		COLORARGB Getctbcolor();
		COLORARGB Getbcolor();
		COLORARGB Getccolor();
		ULONG GetCursorPosition();
		ULONG GetChoosedCursorPosition();
		const LOGFONT& GetFont();
		BYTE GetSettings();

		void Print() override;

		void Mw_Default_Handle_KeyMessage(EditBox* pcontrol, ExMessage* msg);
		void Mw_Default_Handle_MouseMessage(EditBox* pcontrol, ExMessage* msg);

		bool		onfocus;
	private:
		SIZE		size;
		COLORARGB	tcolor = 0;
		COLORARGB	stbcolor = 0;				//The background color of the select text 
		COLORARGB	bcolor = 0;
		COLORARGB	ccolor = 0;					//cursor color
		UINT		cursor_position;
		UINT		select_cursor_position;	//The position of the cursor before choose
		wstring		text;
		LOGFONT		font;
		BYTE		settings;

		inline COORD CursorPosition2Coord(UINT cursor_position) {
			if (cursor_position == 0) return COORD{ 0,(short)(font.lfHeight) };
			UINT rownum = 1;
			UINT cursor_row = 0;

			for (UINT i = 0; i < cursor_position; i++)
				if (text[i] == L'\n') {
					cursor_row = i + 1;
					rownum++;
				}
			wstring buffer;
			for (size_t i = cursor_row; i < cursor_position; i++)
				buffer += text[i];

			IMAGE bufferimage;
			ChangeWorkingImage(true);
			SetWorkingImage(&bufferimage);
			settextstyle(&font);
			ChangeWorkingImage();

			Font f(GetImageHDC(&bufferimage));
			short width = GetTextWidth(font, buffer);

			return COORD{ width,(short)(rownum * font.lfHeight) };
		}

		UINT Coord2CursorPosition(COORD coord);
	};

	class Button :public InteractiveControl<Button> {
	public:
		Button(COORD coord_, IDentifier id_, Picture* picture_ = NULL, BYTE alpha_ = 255) {
			coord = coord_;
			id = id_;
			picture = picture_;
			alpha = alpha_;
			HandleMsgFunctions.push_back([&](Button* pcontrol, ExMessage* msg) {
				Mw_Default_HandleClickMessage(pcontrol, msg);
			});
		}

		Button() {
			coord = COORD{ 0,0 };
			id = 0;
			alpha = 255;
		}

		void SetImage(Picture* picture_);
		const Picture* GetImage();

		enum clickstate {
			none = 0,
			onclick = 1,
			clicked = 2
		};

		inline BYTE GetClickState() {
			return clickstate_;
		}

		inline void Mw_Default_HandleClickMessage(Button* pcontrol, ExMessage* msg) {
			if (msg->message == WM_LBUTTONUP) {
				if (pcontrol->onclick_ == true)
					pcontrol->onclick_ = false;
				else {
					pcontrol->clickstate_ = none;
					return;
				}
				if (InRect(
					COORD{ msg->x,msg->y },
					RECT{ pcontrol->coord.X,pcontrol->coord.Y,
					pcontrol->coord.X + pcontrol->picture->GetImage().getwidth(),pcontrol->coord.Y + pcontrol->picture->GetImage().getheight() }))
				{
					pcontrol->clickstate_ = clicked;
					return;
				}
			}
			else if (msg->message == WM_LBUTTONDOWN) {
				if (InRect(
					COORD{ msg->x,msg->y },
					RECT{ pcontrol->coord.X,pcontrol->coord.Y,
					pcontrol->coord.X + pcontrol->picture->GetImage().getwidth(),pcontrol->coord.Y + pcontrol->picture->GetImage().getheight() }))
				{
					pcontrol->onclick_ = true;
					pcontrol->clickstate_ = onclick;
					return;
				}
			}
			pcontrol->clickstate_ = none;
			return;
		}

		void Print() override;
	private:
		Picture* picture = NULL;
		bool onclick_ = 0;
		BYTE clickstate_ = none;
	};

	class CheckBox :public InteractiveControl<CheckBox> {
	public:
		CheckBox() {
			coord = COORD{ 0,0 };
			id = 0;
			alpha = 255;
		}

		CheckBox(COORD coord_, IDentifier id_, Picture* checked_ = NULL, Picture* notchecked_, BYTE alpha_ = 255) {
			coord = coord_;
			id = id_;
			checked = checked_;
			notchecked = notchecked;
			alpha = alpha_;
		}


	private:
		bool check = false;
		Picture* checked = NULL;
		Picture* notchecked = NULL;
	};
}