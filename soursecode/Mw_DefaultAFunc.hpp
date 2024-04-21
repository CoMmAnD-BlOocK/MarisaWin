#pragma once
#include "MarisaWin.hpp"
using namespace MarisaWin;

//Default Action Function
namespace Mw_DefaultAFunc {
	void Move2PointWithin(COORD point, void* control, UINT frame, Window& window, IDentifier id, UINT countdown);
}