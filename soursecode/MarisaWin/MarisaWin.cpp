#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void MarisaWin::rotateimage_alpha(IMAGE* dst, IMAGE* src, double rad)
{
	IMAGE* pWorking = GetWorkingImage();
	SetWorkingImage(src);
	int iWidth = getwidth();
	int iHeight = getheight();												// 获取原图长宽

	while (rad > 2 * PI)													// 化简弧度
		rad -= 2 * PI;

	double pad = rad;														// 处理弧度
	if (pad > PI / 2 && pad <= PI)
	{
		pad -= PI / 2;
		pad = PI / 2 - pad;
	}
	else if (pad > PI && pad <= PI / 2 * 3)
	{
		pad -= PI;
	}
	else if (pad > PI / 2 * 3 && pad <= PI * 2)
	{
		pad -= PI / 2 * 3;
		pad = PI / 2 - pad;
	}

	int	tWidth = int(iWidth * cos(pad) + iHeight * sin(pad));
	int	tHeight = int(iHeight * cos(pad) + iWidth * sin(pad));				// 计算新图大小

	int iMinX = -(iWidth / 2), iMinY = -(iHeight / 2);
	int iMaxX = iMinX + iWidth, iMaxY = iMinY + iHeight;					// 计算原图最小（大）坐标

	int tMinX = -(tWidth / 2), tMinY = -(tHeight / 2);
	int tMaxX = tMinX + tWidth, tMaxY = tMinY + tHeight;					// 计算新图最小（大）坐标

	setorigin(-iMinX, -iMinY);												// 设置图片中心为原点

	SetWorkingImage(NULL);
	dst->Resize(tWidth, tHeight);											// 初始化新图

	DWORD* pdst = GetImageBuffer(dst);
	DWORD* psrc = GetImageBuffer(src);										// 获取新图、原图的缓冲区

	SetWorkingImage(dst);
	for (int y1 = 0; y1 < tHeight; y1++)
	{
		for (int x1 = 0; x1 < tWidth; x1++)
			pdst[x1] = 0x00000000;
		pdst += tWidth;
	}
	SetWorkingImage(pWorking);
	for (int y1 = 0; y1 < tHeight; y1++)									// 初始化新图
		pdst -= tWidth;

	for (int y1 = tMinY; y1 < tMaxY; y1++)
	{
		for (int x1 = tMinX; x1 < tMaxX; x1++)
		{
			int x = int(x1 * cos(rad) - y1 * sin(rad));
			int y = int(x1 * sin(rad) + y1 * cos(rad));						// 计算变换后坐标

			int sxy = (iHeight - (y - iMinY) - 1) * iWidth + (x - iMinX);
			int dxy = (tHeight - (y1 - tMinY) - 1) * tWidth + (x1 - tMinX);	// 计算坐标在缓冲区的位置

			if (x >= iMinX && x < iMaxX && y >= iMinY && y < iMaxY)			// 越界特判
				pdst[dxy] = psrc[sxy];
		}
	}

	SetWorkingImage(src);
	setorigin(0, 0);
	SetWorkingImage(pWorking);												// 还原原图坐标
}

void MarisaWin::transparentimage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, BYTE alpha_ex)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();

	// 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha_ex, AC_SRC_ALPHA };
	// 使用 Windows GDI 函数实现半透明位图
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void MarisaWin::ChangeWorkingImage(bool changedevice, IMAGE* device)
{
	static list<IMAGE*> device_;
	if (changedevice) {
		device_.push_back(device);
		return;
	}
	if (device_.empty()) return;
	SetWorkingImage(device_.back());
	device_.pop_back();
}

void MarisaWin::arc_alpha(int left, int top, int right, int bottom, double stangle, double endangle) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	g.DrawArc(&pen, left, top, right - left, bottom - top, stangle, endangle);
}

void MarisaWin::circle_alpha(int x, int y, int radius) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	g.DrawArc(&pen, x - radius, y - radius, radius * 2, radius * 2, 0, 360);
}

void MarisaWin::ellipse_alpha(int left, int top, int right, int bottom) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	g.DrawEllipse(&pen, left, top, right - left, bottom - top);
}

COLORARGB MarisaWin::getpixel_alpha(int x, int y) {
	if (x > getwidth() || y > getheight()) return 0;    //If out of range
	DWORD* pimage = GetImageBuffer(GetWorkingImage());
	return pimage[y * getwidth() + x];
}

void MarisaWin::line_alpha(int x1, int y1, int x2, int y2) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	g.DrawLine(&pen, x1, y1, x2, y2);
}

void MarisaWin::pie_alpha(int left, int top, int right, int bottom, double stangle, double endangle) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	g.DrawPie(&pen, left, top, right - left, bottom - top, stangle, endangle);
}

void MarisaWin::polybezier_alpha(const POINT* points, int num) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	Gdiplus::Point* p = new Gdiplus::Point[num];
	for (int i = 0; i < num; i++) {
		p[i].X = points[i].x;
		p[i].Y = points[i].y;
	}
	g.DrawBeziers(&pen, p, num);
	delete[num] p;
}

void MarisaWin::polygon_alpha(const POINT* points, int num) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	Gdiplus::Point* p = new Gdiplus::Point[num];
	for (int i = 0; i < num; i++) {
		p[i].X = points[i].x;
		p[i].Y = points[i].y;
	}
	g.DrawPolygon(&pen, p, num);
	delete[num] p;
}

void MarisaWin::polyline_alpha(const POINT* points, int num) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	Gdiplus::Point* p = new Gdiplus::Point[num];
	for (int i = 0; i < num; i++) {
		p[i].X = points[i].x;
		p[i].Y = points[i].y;
	}
	g.DrawPolygon(&pen, p, num);
	delete[num] p;
}

void MarisaWin::putpixel_alpha(int x, int y, COLORARGB color) {
	if ((x > getwidth()) || (y > getheight())) return;    //If out of range
	int dst_pixiv = y * getwidth() + x;
	DWORD* pdst = GetImageBuffer(GetWorkingImage());
	DWORD srca = GetAlpha_ARGB(color);

	DWORD ndsta = 0, ndstr = 0, ndstg = 0, ndstb = 0;
	ndsta = DWORD((1 - (double)srca / 255) * double(srca / 255) + (double)srca / 255 * (double)GetAlpha_ARGB(pdst[dst_pixiv]) / 255);
	ndstr = DWORD((1 - (double)srca / 255) * GetRValue(color) + (double)srca / 255 * GetRValue(pdst[dst_pixiv]));
	ndstg = DWORD((1 - (double)srca / 255) * GetGValue(color) + (double)srca / 255 * GetGValue(pdst[dst_pixiv]));
	ndstb = DWORD((1 - (double)srca / 255) * GetBValue(color) + (double)srca / 255 * GetBValue(pdst[dst_pixiv]));

	pdst[dst_pixiv] = (ndsta << 24) | (ndstr << 16) | (ndstg << 8) | ndstb;
}

void MarisaWin::rectangle_alpha(int left, int top, int right, int bottom) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);

	Pen pen(Color(ARGB(BGR(getlinecolor()))), linestyle.thickness);
	g.DrawRectangle(&pen, left, top, right - left, bottom - top);
}

void MarisaWin::roundrect_alpha(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	LINESTYLE linestyle;
	getlinestyle(&linestyle);
	Color c = Color(ARGB(BGR(getlinecolor())));
	Pen pen(c, linestyle.thickness);
	//LT
	g.DrawArc(&pen, left, top, ellipsewidth, ellipseheight, 180, 90);
	//RT
	g.DrawArc(&pen, right - ellipsewidth, top, ellipsewidth, ellipseheight, 270, 90);
	//LB
	g.DrawArc(&pen, left, bottom - ellipseheight, ellipsewidth, ellipseheight, 90, 90);
	//RB
	g.DrawArc(&pen, right - ellipsewidth, bottom - ellipseheight, ellipsewidth, ellipseheight, 0, 90);

	MarisaWin::line_alpha(left + ellipsewidth / 2, top, right - ellipsewidth / 2, top);
	MarisaWin::line_alpha(left + ellipsewidth / 2, bottom, right - ellipsewidth / 2, bottom);

	MarisaWin::line_alpha(left, top + ellipseheight / 2, left, bottom - ellipseheight / 2);
	MarisaWin::line_alpha(right, top + ellipseheight / 2, right, bottom - ellipseheight / 2);
}

void MarisaWin::solidcircle_alpha(int x, int y, int radius) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	Color c = Color(ARGB(BGR(getfillcolor())));
	SolidBrush b(c);
	g.FillEllipse(&b, x - radius, y - radius, radius * 2, radius * 2);
}

void MarisaWin::solidellipse_alpha(int left, int top, int right, int bottom) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	Color c = Color(ARGB(BGR(getfillcolor())));
	SolidBrush b(c);
	g.FillEllipse(&b, left, top, right, bottom);
}

void MarisaWin::solidpie_alpha(int left, int top, int right, int bottom, double stangle, double endangle){
	Graphics g(GetImageHDC(GetWorkingImage()));
	Color c = Color(ARGB(BGR(getfillcolor())));
	SolidBrush b(c);
	g.FillPie(&b, left, top, right, bottom, stangle, endangle);
}

void MarisaWin::solidpolygon_alpha(const POINT* points, int num) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	Color c = Color(ARGB(BGR(getfillcolor())));
	SolidBrush b(c);
	Gdiplus::Point* p = new Gdiplus::Point[num];
	for (int i = 0; i < num; i++) {
		p[i].X = points[i].x;
		p[i].Y = points[i].y;
	}
	g.FillPolygon(&b, p, num);
	delete[num] p;
}

void MarisaWin::solidrectangle_alpha(int left, int top, int right, int bottom) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	Color c = Color(ARGB(BGR(getfillcolor())));
	SolidBrush b(c);
	g.FillRectangle(&b, left, top, right - left, bottom - top);
}

void MarisaWin::solidroundrect_alpha(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	Color c = Color(ARGB(BGR(getfillcolor())));
	SolidBrush b(c);
	//LT
	g.FillPie(&b, left, top, ellipsewidth, ellipseheight, 180, 90);
	//RT
	g.FillPie(&b, right - ellipsewidth, top, ellipsewidth, ellipseheight, 270, 90);
	//LB
	g.FillPie(&b, left, bottom - ellipseheight, ellipsewidth, ellipseheight, 90, 90);
	//RB
	g.FillPie(&b, right - ellipsewidth, bottom - ellipseheight, ellipsewidth, ellipseheight, 0, 90);

	MarisaWin::solidrectangle_alpha(left + ellipsewidth / 2, top, right - ellipsewidth / 2, bottom);
	MarisaWin::solidrectangle_alpha(left, top + ellipseheight / 2, right, bottom - ellipseheight / 2);
}

void MarisaWin::fillcircle_alpha(int x, int y, int radius) {
	MarisaWin::solidcircle_alpha(x, y, radius);
	circle(x, y, radius);
}

void MarisaWin::fillellipse_alpha(int left, int top, int right, int bottom) {
	MarisaWin::solidellipse_alpha(left, top, right, bottom);
	MarisaWin::ellipse_alpha(left, top, right, bottom);
}

void MarisaWin::fillpie_alpha(int left, int top, int right, int bottom, double stangle, double endangle) {
	MarisaWin::solidpie_alpha(left, top, right, bottom, stangle, endangle);
	MarisaWin::pie_alpha(left, top, right, bottom, stangle, endangle);
}

void MarisaWin::fillpolygon_alpha(const POINT* points, int num) {
	MarisaWin::solidpolygon_alpha(points, num);
	MarisaWin::polygon_alpha(points, num);
}

void MarisaWin::fillrectangle_alpha(int left, int top, int right, int bottom) {
	MarisaWin::solidrectangle_alpha(left, top, right, bottom);
	MarisaWin::rectangle_alpha(left, top, right, bottom);
}

void MarisaWin::fillroundrect_alpha(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	MarisaWin::solidroundrect_alpha(left, top, right, bottom, ellipseheight, ellipseheight);
	MarisaWin::roundrect_alpha(left, top, right, bottom, ellipseheight, ellipseheight);
}

void MarisaWin::outtextxy_alpha(int x, int y, const wchar_t* str) {
	Graphics g(GetImageHDC(GetWorkingImage()));
	Font font(GetImageHDC(GetWorkingImage()));
	SolidBrush b(Color(ARGB(BGR(gettextcolor()))));
	g.SetTextRenderingHint(TextRenderingHintAntiAlias);
	g.DrawString(str, -1, &font, PointF(x, y), &b);
}

void MarisaWin::drawtext_alpha(const wchar_t* str, RECT rect) {
	if (*str == L'\0') return;
	IMAGE buffer(rect.right - rect.left, rect.bottom - rect.top);
	COLORREF tcolor = gettextcolor();
	LOGFONT textstyle;
	gettextstyle(&textstyle);
	ChangeWorkingImage(true);
	SetWorkingImage(&buffer);
	settextcolor(tcolor);
	settextstyle(&textstyle);
	wstring text;
	UINT row_num = 0;
	for (UINT i = 0; str[i] != L'\0'; i++) {
		text += str[i];
		if ((str[i] == L'\n') || (str[i + 1] == L'\0')) {
			outtextxy_alpha(0, row_num * textstyle.lfHeight, text.c_str());
			row_num++;
			text.clear();
			continue;
		}
	}
	ChangeWorkingImage();
	transparentimage(GetWorkingImage(), rect.left, rect.top, &buffer, 255);
}