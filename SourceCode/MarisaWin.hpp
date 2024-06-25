/////////////////////////////////////////////////
//
//	   _____               .__               __      __.__
//	  /     \ _____ _______|__| ___________ /  \    /  \__| ____
//	 /  \ /  \\__  \\_  __ \  |/  ___/\__  \\   \/\/   /  |/    \
//	/    Y    \/ __ \|  | \/  |\___ \  / __ \\        /|  |   |  \
//	\____|__  (____  /__|  |__/____  >(____  /\__/\  / |__|___|  /
//			\/     \/              \/      \/      \/          \/
// MarisaWin UI Library [by 命令方块好方]
// Version: Bulid 20240517
// If U found any bug, please call 1798796443@qq.com
//
/////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include <windows.h>
#pragma comment(lib, "Gdiplus.lib")
#include <gdiplus.h>
using namespace Gdiplus;

#include <list>
#include <string>
#include <functional>
#include <ctime>

using namespace std;

namespace MarisaWin {
	typedef function<void(Graphics& graphics)> Drawfunc;

	inline void SetImageAlpha(Bitmap& bitmap, BYTE alpha)
	{
		BitmapData bitmapdata{};
		Rect bitmapsize(0, 0, bitmap.GetWidth(), bitmap.GetHeight());
		bitmap.LockBits(&bitmapsize,
			ImageLockModeRead | ImageLockModeWrite,
			PixelFormat32bppARGB,
			&bitmapdata);
		BYTE* pbuffer = (BYTE*)bitmapdata.Scan0;
		for (int i = 0; i < bitmapdata.Width * bitmapdata.Height; i++) {
			pbuffer[4 * i + 3] = pbuffer[4 * i + 3] * ((float)alpha / 255.0f);
		}
		bitmap.UnlockBits(&bitmapdata);
	}

	inline SizeF GetTextBounds(const Font* font, const StringFormat& strFormat, const wstring& szText)
	{
		GraphicsPath path;
		FontFamily fontfamily;
		font->GetFamily(&fontfamily);
		path.AddString(szText.c_str(), -1, &fontfamily, font->GetStyle(), font->GetSize(), PointF(0, 0), &strFormat);
		RectF rcBound;
		path.GetBounds(&rcBound);
		if (szText.back() == L' ') {
			RectF rectf;
			Bitmap b(0, 0);
			Graphics g(&b);
			g.MeasureString(L" ", -1, font, PointF(0, 0), &strFormat, &rectf);
			rcBound.Width += rectf.Width;
		}
		return SizeF(rcBound.Width, rcBound.Height + 2);
	}

	//This class is facilitate managing instantiated variable, such as some functions defined by user
	template <class T>
	class IDBinder
	{
	public:
		UINT id = NULL;
		IDBinder(T* pvariable_ = NULL, UINT handle_ = NULL)
		{
			id = handle_;
			pvariable = pvariable_;
		}

		IDBinder(const IDBinder<T>& newIDBinder)
		{
			id = newIDBinder.id;
			data_instantiated = true;
			pvariable = new T(*newIDBinder.pvariable);
		}

		IDBinder(IDBinder<T>&& newIDBinder)
		{
			id = newIDBinder.handle_;
			data_instantiated = true;
			pvariable = new T(newIDBinder.pvariable);
		}

		static inline IDBinder<T>& NewIDBinder(T pvariable_, UINT id_ = NULL)
		{
			IDBinder<T> ret(new T(pvariable_), id_);
			ret.data_instantiated = true;
			return ret;
		}

		static inline IDBinder<T>* NewIDBinder_Heap(T pvariable_, UINT id_ = NULL)
		{
			return new IDBinder<T>(NewIDBinder(pvariable_,id_));
		}
		
		inline void operator = (T* pvariable_)
		{
			if (data_instantiated) delete pvariable;
			pvariable = pvariable_;
		}

		inline void operator = (UINT id_)
		{
			id = id_;
		}

		inline void operator = (IDBinder<T> newIDBinder)
		{
			if (data_instantiated) delete pvariable;
			id = newIDBinder.id_;
			data_instantiated = true;
			pvariable = new T(newIDBinder.pvariable);
		}

		inline T* operator * ()
		{
			return pvariable;
		}

		inline UINT operator & ()
		{
			return id;
		}

		~IDBinder()
		{
			if (data_instantiated) delete pvariable;
		}
	private:
		bool data_instantiated = false;
		T* pvariable = NULL;
	};

	class Control {
	public:
#define ConstrControl PointF coord_, const wstring& controlname_
#define ConstrControlAssign coord = coord;\
controlname = controlname;
		typedef function<void(void* pcontrol, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)> HandleMsgFunc;
		bool hide = false;			//If it is true,this control won't paint on the window
		wstring controlname = L"";	//The identifier of control
		PointF coord{ 0,0 };		//The coordinate of control
		BYTE alpha = 255;
		list<IDBinder<HandleMsgFunc>> UserHandleMsgFunc;
		list<IDBinder<Drawfunc>> UserDrawFuncBC;	//Execute before execute default painting control
		list<IDBinder<Drawfunc>> UserDrawFuncAC;	//Execute bfter execute default painting control

		inline void Draw(Graphics& graphics)
		{
			if (alpha == 0) return;
			Bitmap buffer(GetControlSize().Width, GetControlSize().Height);
			Graphics gbuffer(&buffer);
			if (hide) return;
			for (auto i = UserDrawFuncBC.begin(); i != UserDrawFuncBC.end(); i++)
				(**(*i))(gbuffer);
			Control_Draw(gbuffer);
			for (auto i = UserDrawFuncAC.begin(); i != UserDrawFuncAC.end(); i++)
				(**(*i))(gbuffer);
			if (alpha != 255) SetImageAlpha(buffer, alpha);
			graphics.DrawImage(&buffer, coord);
		}

		virtual SizeF GetControlSize() = 0;
		inline void SetCoord(const PointF& coord_)
		{
			coord = coord_;
		}

		inline void HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			for (auto i = UserHandleMsgFunc.begin(); i != UserHandleMsgFunc.end(); i++)
				(**(*i))((void*)this, hWnd, message, wParam, lParam);

			MW_DefWindowProc(hWnd, message, wParam, lParam);
		}

		inline bool PointInControl(PointF point)
		{
			RectF controlrect(coord.X, coord.Y, coord.X + GetControlSize().Width, coord.Y + GetControlSize().Height);
			return controlrect.Contains(point);
		}
	private:
		virtual void Control_Draw(Graphics& graphics) = 0;
		virtual void MW_DefWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam) = 0;
#define MW_DefWindowProc_ MW_DefWindowProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
	};

	class Screen {
	public:
		void PushControlFront(void* control);
		void PushControlFront(size_t controlcount, ...);
		void PushControlBack(void* control);
		void PushControlBack(size_t controlcount, ...);

		void PopControlFront();
		void PopControlBack();

		void AddControl(void* control, size_t where_);

		void RemoveControl(void* control);
		void RemoveControl(const wstring& controlname);
		void RemoveControl(size_t where_);

		void SortControl(void* control, size_t where_);
		void SortControl(const wstring& controlname, size_t where_);

		size_t LookUpControl(void* control);
		size_t LookUpControl(const wstring& controlname);
		void* LookUpControl(size_t where_);

		inline void DrawScreen(Graphics& g)
		{
			if (!ControlList.empty())
			for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++) {
				((Control*)(*i))->Draw(g);
			}
		}

		inline void MW_DispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			if (!ControlList.empty())
			for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++) {
				((Control*)(*i))->HandleMessage(hWnd, message, wParam, lParam);
			}
		}
	private:
		list<void*> ControlList;
	};

	class DelayedFunction {
		friend class Window;
	public:
		DelayedFunction(
			const function<void(void)> &delayedfunction_,
			size_t delayedtimes_ = 0,
			size_t delayedinterval_ = 0,
			bool staticfunction_ = false,
			bool executenow = true)
		{
			delayedtimes	= delayedtimes_;
			delayedinterval	= delayedinterval_;
			delayedfunction	= delayedfunction_;
			staticfunction	= staticfunction_;
			if (!executenow) countdown = delayedinterval_;
		}

		size_t delayedtimes = 0;
		size_t delayedinterval = 0;
		function<void(void)> delayedfunction;

		bool staticfunction = false;
	private:
		size_t countdown;
	};

	class Window {
	public:
		Window(HWND hWnd_, USHORT fps_ = 60)
		{
			if (!IsWindow(hWnd_)) return;
			hWnd = hWnd_;
			fps = fps_;
		}

		inline void SetScreen(Screen* screen)
		{
			screen_now = screen;
		}

		inline Screen* GetScreen()
		{
			return screen_now;
		}

		inline bool StartManageWindow()
		{
			if (managing || (!IsWindow(hWnd))) return true;

			origin_lpfnWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc_MarisaWin);

			managing = true;
			return false;
		}

		inline bool StopManageWindow()
		{
			if (!managing) return true;

			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)origin_lpfnWndProc);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)0);

			managing = false;
			return false;
		}

		inline void SetWindow(HWND hWnd_)
		{
			StopManageWindow();
			hWnd = hWnd_;
		}

		inline HWND GetWindow() const
		{
			return hWnd;
		}

		inline void SetUserWndproc(
			function<bool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>* user_wndproc_)
		{
			user_wndproc = user_wndproc_;
		}

		inline
		const function<bool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>*
			GetUserWndproc()
		{
			return user_wndproc;
		}

		inline void SetUserDrawfuncBC(Drawfunc* user_drawfunc_bc_)
		{
			user_drawfunc_bc = user_drawfunc_bc_;
		}

		inline const Drawfunc* GetUserDrawfuncBC()
		{
			return user_drawfunc_bc;
		}

		inline void SetUserDrawfuncAC(Drawfunc* user_drawfunc_ac_)
		{
			user_drawfunc_ac = user_drawfunc_ac_;

		}

		inline const Drawfunc* GetUserDrawfuncAC()
		{
			return user_drawfunc_ac;
		}

		inline void SetFps(USHORT fps_)
		{
			fps = fps_;
		}

		inline USHORT GetFps() const
		{
			return fps;
		}

		inline void SetIcon(HICON hIcon) {
			SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		}

		~Window()
		{
			StopManageWindow();
		}

		//The delayedfunction list
		list<DelayedFunction> delayedfunction;
	private:
		HWND		hWnd = NULL;
		USHORT		fps  = 60;
		Screen*		screen_now = NULL;
		WNDPROC		origin_lpfnWndProc = NULL;
		bool		managing = false;

		//To handle message in window-process function
		function<bool(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>* user_wndproc = NULL;

		Drawfunc* user_drawfunc_bc = NULL;	//Before paint control
		Drawfunc* user_drawfunc_ac = NULL;	//After paint control

		static LRESULT CALLBACK WndProc_MarisaWin(HWND hWnd, UINT message,
			WPARAM wParam, LPARAM lParam)
		{
			//Get the pointer of window class
			Window* this_ = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

			//Callback window-proc function the user-defined
			if (this_->user_wndproc != NULL)
				(*this_->user_wndproc)(hWnd, message, wParam, lParam);
			
			//Dispatch Message
			if (this_->screen_now != NULL)
			this_->screen_now->MW_DispatchMessage(hWnd, message, wParam, lParam);

			static const auto redrawwindow = [=]()
				{
					RECT windowsize;
					GetClientRect(hWnd, &windowsize);
					Bitmap buf(windowsize.right - windowsize.left,
						windowsize.bottom - windowsize.top);
					Graphics paintbuffer(&buf);
					SolidBrush clearbkground(Color(255, 0, 0, 0));
					paintbuffer.FillRectangle(&clearbkground, 
						Rect(windowsize.left,windowsize.top,windowsize.right,windowsize.bottom));
					paintbuffer.SetSmoothingMode(SmoothingModeAntiAlias);
					paintbuffer.SetTextRenderingHint(TextRenderingHintAntiAlias);
					//Begin Paint on the Buffer

					if (this_->user_drawfunc_bc != NULL)
						(*this_->user_drawfunc_bc)(paintbuffer);

					if (this_->screen_now != NULL)
						this_->screen_now->DrawScreen(paintbuffer);

					if (this_->user_drawfunc_ac != NULL)
						(*this_->user_drawfunc_ac)(paintbuffer);

					PAINTSTRUCT ps{};
					Graphics window(BeginPaint(hWnd, &ps));
					window.DrawImage(&buf, 0, 0);
					EndPaint(hWnd, &ps);
				};

			static clock_t frame = 0;
			switch (message)
			{
				case WM_PAINT:
				{
					//Refresh the window
					if (this_->fps == 0) {
						redrawwindow();
					}
					else if ((clock() - frame >= 1000 / this_->fps) && this_->fps != 0) {
						for (auto i = this_->delayedfunction.begin(); i != this_->delayedfunction.end(); i++) {
							if (i->staticfunction) i->delayedfunction();
							else {
								if (i->delayedtimes == 0) {
									auto unnamed = i; i--;
									this_->delayedfunction.erase(unnamed);
									continue;
								}
								if (i->countdown == 0) {
									i->delayedfunction();
									i->countdown = i->delayedinterval;
									i->delayedtimes--;
								}
								else i->countdown--;
							}
						}
						redrawwindow();
						frame = clock();
					}
					return 0;
				}
				case WM_SIZE:
					redrawwindow();
					InvalidateRect(hWnd, NULL, false);
					UpdateWindow(hWnd);
				case WM_ERASEBKGND:
					return 0;
				case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
	};

	struct TextColor
	{
		Color textcolor, backgroundcolor;
	};

	class Label : public Control{
	public:
		Label()
		{
			FontFamily fontfamily(L"黑体");
			font = Font(&fontfamily, 24, FontStyleRegular, UnitPixel).Clone();
			textcolor.textcolor = Color(255, 0, 0, 0);
			textcolor.backgroundcolor = Color(0, 0, 0, 0);
		}

		Label(ConstrControl, const Font& font_, TextColor textcolor_, wstring text_)
		{
			ConstrControlAssign
			font = font_.Clone();
			textcolor = textcolor_;
			text = text_;
		}

		Label(ConstrControl, TextColor textcolor_, wstring text_)
		{
			ConstrControlAssign
			FontFamily fontfamily(L"黑体");
			font = Font(&fontfamily, 24, FontStyleRegular, UnitPixel).Clone();
			coord = coord_;
			textcolor = textcolor_;
			text = text_;
		}

		void operator =(const Label& label)
		{
			delete font;
			font = label.font->Clone();
			textcolor = label.textcolor;
		}

		inline SizeF GetControlSize()
		{
			return GetTextBounds(font, 0, text);
		}

		TextColor textcolor;
		wstring text = L"";
	private:
		void Control_Draw(Graphics& graphics);
		void MW_DefWindowProc_;
		Font* font = NULL;
	};

	class Button : public Control {
	public:
		Button(ConstrControl, Bitmap *picture_) {
			ConstrControlAssign
			coord = coord_;
			picture = picture_->Clone(
				0,0,picture_->GetWidth(),picture_->GetHeight(),
				picture_->GetPixelFormat());
		}

		inline void MW_DefWindowProc_
		{
			switch (Msg)
			{
				if (clickstate == clicked) clickstate = none;
			case WM_LBUTTONDOWN:
				if ((clickstate == none)&&PointInControl(PointF(LOWORD(lParam), HIWORD(lParam))))
					clickstate = clicking;
			case WM_LBUTTONUP:
				if (clickstate == clicking)
					clickstate = clicked;
			case WM_MOUSEMOVE:
				if ((clickstate == clicking) && !PointInControl(PointF(LOWORD(lParam), HIWORD(lParam))))
					clickstate = none;
			break;
			}
		}

		SizeF GetControlSize();
		
		enum clickstate_{none, clicking, clicked};
		clickstate_ clickstate = none;
	private:
		void Control_Draw(Graphics& graphics);
		Bitmap* picture = NULL;
	};
}