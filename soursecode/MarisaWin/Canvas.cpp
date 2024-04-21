#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void Canvas::Print() {
	if (hide) return;
	if (refresh_after_print) Refresh();
	Picture p(image);
	PictureFrame(coord, id, &p, SIZE{ image.getwidth(),image.getheight() }, alpha).Print();
}

void Canvas::SetDrawFunction(function<void()> draw_) {
	draw = draw_;
}

void Canvas::SetSize(SIZE size_) {
	image.Resize(size_.cx, size_.cy);
}

SIZE Canvas::GetSize() {
	return SIZE{ image.getwidth(),image.getheight() };
}

void Canvas::ClearCanvas() {
	ChangeWorkingImage(true);
	Graphics(GetImageHDC(GetWorkingImage())).SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
	SetWorkingImage(&image);
	graphdefaults();
	cleardevice();
	ChangeWorkingImage();
}

void Canvas::CanvaDefaults() {
	ChangeWorkingImage(true);
	SetWorkingImage(&image);
	graphdefaults();
	ChangeWorkingImage();
}

void Canvas::Refresh(){
	ChangeWorkingImage(true);
	SetWorkingImage(&image);
	draw();
	ChangeWorkingImage();
}

const IMAGE& Canvas::GetImage() {
	return image;
}