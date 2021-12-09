#include "HeldPickupWidget.h"
#include "VString.h"

void HeldPickupWidget::Tick(float deltaTime)
{
	auto layout = AlignLayout(200.f, 200.f, Align::BottomRight);
	FillRect(layout);
	Text(VString::wformat(L"%S", pickupName.c_str()), layout);
}
