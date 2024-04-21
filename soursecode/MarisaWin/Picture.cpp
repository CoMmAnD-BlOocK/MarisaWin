#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void Picture::SetImage(const IMAGE& image) {
	origin_image = image;
}

IMAGE& Picture::GetImage() {
	return origin_image;
}

void MarisaWin::Picture::SetAlpha(const BYTE alpha_) {
	alpha = alpha_;
}

BYTE MarisaWin::Picture::GetAlpha() {
	return alpha;
}
