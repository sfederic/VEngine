#include "AcceptMemoryWidget.h"

void AcceptMemoryWidget::Draw(float deltaTime)
{
	auto layout = PercentAlignLayout(0.1f, 0.5f, 0.9f, 0.6f);

	FillRect(PercentAlignLayout(0.1f, 0.5f, 0.9f, 0.9f));

	Text(L"Memorise this object?", layout);
	layout.AddVerticalSpace(50.f);

	if (Button(L"Yes", layout))
	{
		acceptButtonCallback(actorName);
		Reset();
	}

	layout.AddHorizontalSpace(100.f);

	if (Button(L"No", layout))
	{
		//declineButtonCallback(actorName);
		Reset();
	}
}

void AcceptMemoryWidget::Reset()
{
	acceptButtonCallback = nullptr;
	//declineButtonCallback = nullptr;

	actorName.clear();

	this->RemoveFromViewport();
}
