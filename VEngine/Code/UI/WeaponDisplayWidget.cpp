#include "WeaponDisplayWidget.h"
#include "Gameplay/GameUtils.h"
#include "Actors/Game/Player.h"
#include "VString.h"

void WeaponDisplayWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.1f, 0.6f, 0.3f, 0.9f);

	FillRect(layout);

	//Button(L"Spear", layout);
	//layout.AddHorizontalSpace(120.f);

	//Button(L"Sword", layout);
	//layout.AddHorizontalSpace(120.f);

	//Button(L"Shield", layout);
	//layout.AddHorizontalSpace(120.f);

	auto player = GameUtils::GetPlayer();
	auto aw = player->activeWeapon;
	if (aw)
	{
		Text(VString::wformat(L"ATK: %S", aw->name.c_str()), layout);
		layout.AddVerticalSpace(30.f);

		Text(VString::wformat(L"ATK: %d", aw->attackPoints), layout);
		layout.AddVerticalSpace(30.f);

		Text(VString::wformat(L"ENRG: %d", aw->energyCost), layout);
		layout.AddVerticalSpace(30.f);

		Text(VString::wformat(L"WGHT: %d", aw->weight), layout);
	}
}
