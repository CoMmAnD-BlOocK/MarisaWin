#include "..\Mw_DefaultAFunc.hpp"
using namespace Mw_DefaultAFunc;

void Mw_DefaultAFunc::Move2PointWithin(COORD point, void* control, UINT frame, Window& window, IDentifier id, UINT countdown)
{
	if (control == NULL) return;
	COORD origin_coord = static_cast<Control*>(control)->GetCoord();
	if (origin_coord.X == point.X && origin_coord.Y == point.Y) return;

	UpdateFunc function_x, function_y, correct;

	//Initialize
	function_x.frame_countdown = function_y.frame_countdown = countdown;
	correct.id = function_x.id = function_y.id = id;

	if (point.X - origin_coord.X == 0) goto x_equ;
	if ((UINT)abs(point.X - origin_coord.X) >= frame)
	{
		SHORT addx = (SHORT)(point.X - origin_coord.X) / (INT)frame;
		function_x.function = [control, addx] {
			static_cast<Control*>(control)->AddX(addx);
			};
		function_x.repeat_times = frame;
	}
	else {
		SHORT addx = (point.X - origin_coord.X) > 0 ? 1 : -1;
		function_x.function = [control, addx] {
			static_cast<Control*>(control)->AddX(addx);
			};
		function_x.repeat_times = abs(point.X - origin_coord.X);
		function_x.repeat_interval = (int)((double)abs(point.X - origin_coord.X) / frame);
	}

	if (point.Y - origin_coord.Y == 0) goto y_equ;
x_equ:
	if ((UINT)abs(point.Y - origin_coord.Y) >= frame)
	{
		SHORT addy = (SHORT)(point.Y - origin_coord.Y) / (INT)frame;
		function_y.function = [control, addy] {
			static_cast<Control*>(control)->AddY(addy);
			};
		function_y.repeat_times = frame;
	}
	else {
		SHORT addy = point.Y - origin_coord.Y > 0 ? 1 : -1;
		function_y.function = [control, addy] {
			static_cast<Control*>(control)->AddY(addy);
			};
		function_y.repeat_times = abs(point.X - origin_coord.X);
		function_y.repeat_interval = (int)((double)abs(point.Y - origin_coord.Y) / frame);
	}

y_equ:
	correct.frame_countdown = countdown + frame;
	correct.function = [control, point, origin_coord] {
		if (origin_coord.X != point.X || origin_coord.Y != point.Y)
		{
			static_cast<Control*>(control)->SetCoord(point);
		}
		};

	window.PushUpdateFunction(function_x);
	window.PushUpdateFunction(function_y);
	window.PushUpdateFunction(correct);
}
