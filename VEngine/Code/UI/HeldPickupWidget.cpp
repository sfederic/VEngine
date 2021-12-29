#include "HeldPickupWidget.h"
#include "VString.h"

void HeldPickupWidget::Draw()
{
	auto layout = AlignLayout(50.f, 50.f, Align::BottomRight);
	FillRect(layout);
	Text(VString::wformat(L"%S", pickupName.c_str()), layout);
}
