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
//累 tired:(
namespace MarisaWin {
	typedef function<void(Graphics& graphics)> Drawfunc;

	inline SIZE GetTextSizebyFont(Font& font,wstring text)
	{
		Bitmap bitmap(0, 0);
		Graphics g(&bitmap);
		
	}

	class Control {
	public:
		bool hide = false;
		wstring controlname = L"";
		PointF coord{ 0,0 };
		BYTE alpha = 255;
		list<function<void(void* pcontrol, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>>
			HandleMsgFunc;
		list<Drawfunc> UserDrawFuncBC;
		list<Drawfunc> UserDrawFuncAC;

		inline void Draw(Graphics& graphics)
		{
			for (auto i = UserDrawFuncBC.begin(); i != UserDrawFuncBC.end(); i++)
				(*i)(graphics);
			Control_Draw(graphics);
			for (auto i = UserDrawFuncAC.begin(); i != UserDrawFuncAC.end(); i++)
				(*i)(graphics);
		}

		virtual SIZE GetSize() = 0;
		void SetCoord(const PointF& coord_);

		inline void HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			for (auto i = HandleMsgFunc.begin(); i != HandleMsgFunc.end(); i++)
				(*i)((void*)this, hWnd, message, wParam, lParam);
		}
	private:
		virtual void Control_Draw(Graphics& graphics) = 0;
	};

	class Screen {
	public:
		void PushControlFront(void* control);
		void PushControlBack(void* control);

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
			for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++) {
				((Control*)(*i))->Draw(g);
			}
		}

		inline void DispatchMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			for (list<void*>::iterator i = ControlList.begin(); i != ControlList.end(); i++) {
				((Control*)(*i))->HandleMessage(hWnd, message, wParam, lParam);
			}
		}
	private:
		list<void*> ControlList;
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
			origin_lpfnWndProc = (WNDPROC)GetWindowLong(hWnd, GWLP_WNDPROC);
			managing = true;
			return false;
		}

		inline bool StopManageWindow()
		{
			if (!managing) return true;
			SetWindowLong(hWnd, GWLP_WNDPROC, (long)origin_lpfnWndProc);
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
			function<void(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>* user_wndproc_)
		{
			user_wndproc = user_wndproc_;
		}

		inline
		const function<void(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>*
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
	private:
		HWND		hWnd = NULL;
		USHORT		fps  = 60;
		Screen*		screen_now = NULL;
		WNDPROC		origin_lpfnWndProc = NULL;
		bool		managing = false;

		function<void(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>* user_wndproc = NULL;
		Drawfunc* user_drawfunc_bc = NULL;	//Before paint control
		Drawfunc* user_drawfunc_ac = NULL;	//After paint control

		LRESULT CALLBACK WndProc_MarisaWin(HWND hWnd, UINT message,
			WPARAM wParam, LPARAM lParam)
		{
			static clock_t lastframe = clock();
			if (user_wndproc != NULL)
			(*user_wndproc)(hWnd, message, wParam, lParam);
			
			switch (message)
			{
				case WM_PAINT:
				{
					if (clock() - lastframe < 1000 / fps) return 0;
					lastframe = clock();
					RECT windowsize;
					GetClientRect(hWnd, &windowsize);
					Bitmap buf(windowsize.right - windowsize.left,
						windowsize.bottom - windowsize.top);
					Graphics paintbuffer(&buf);
					//Begin Paint on the Buffer

					(*user_drawfunc_bc)(paintbuffer);
					screen_now->DrawScreen(paintbuffer);
					(*user_drawfunc_ac)(paintbuffer);

					PAINTSTRUCT ps{};
					Graphics window(BeginPaint(hWnd, &ps));
					window.DrawImage(&buf, 0, 0);
					EndPaint(hWnd, &ps);
				}
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
			FontFamily fontfamily(L"宋体");
			font = new Font(&fontfamily, 16, FontStyleRegular, UnitPixel);
			textcolor.textcolor = Color(255, 0, 0, 0);
			textcolor.backgroundcolor = Color(0, 0, 0, 0);
		}

		Label(PointF coord_, const Font& font_, TextColor textcolor_, wstring text_)
		{
			coord = coord_;
			font = font_.Clone();
			textcolor = textcolor_;
			text = text_;
		}

		void operator =(const Label& label)
		{
			delete font;
			font = label.font->Clone();
			textcolor = label.textcolor;
		}

		~Label()
		{
			delete font;
		}

		TextColor textcolor;
		wstring text = L"";
	private:
		void Control_Draw(Graphics& graphics) override;
		Font* font = NULL;
	};
}