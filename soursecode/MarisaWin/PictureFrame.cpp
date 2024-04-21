#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void PictureFrame::Print() {
	if (picture == NULL) return;
	if (hide) return;
	IMAGE image = IMAGE(size.cx, size.cy);
	transparentimage(&image, 0, 0, &picture->GetImage(), picture->GetAlpha());
	transparentimage(GetWorkingImage(), coord.X, coord.Y, &image, alpha);
}

void PictureFrame::SetSize(SIZE size_) {
	size = size;
}

SIZE PictureFrame::GetSize() {
	return size;
}