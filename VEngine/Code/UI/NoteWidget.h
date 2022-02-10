#pragma once
#include "Widget.h"

//Note that is meant to be a debug-esque thing viewed from the editor,
//rendered in-world for development purposes. Got the idea from Breath of the Wild's engine from that GDC talk.
struct NoteWidget : Widget
{
	virtual void Draw(float deltaTime) override;
};
