#include "..\MarisaWin.hpp"
using namespace MarisaWin;

void CheckBox::Print() {
	if (hide) return;
	if ((checked == NULL) || (notchecked == NULL)) return;
	Picture* ppicture = check ? checked : notchecked;
	PictureFrame buffer(coord, id, ppicture, SIZE{ ppicture->GetImage().getwidth(),ppicture->GetImage().getheight() }, alpha);
	buffer.Print();
}