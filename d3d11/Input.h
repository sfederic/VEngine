#pragma once

#include <Windows.h>

extern WPARAM currentUpKey;
extern WPARAM currentDownKey;
extern bool leftMouseUp;
extern bool leftMouseDown;
extern bool rightMouseUp;
extern bool rightMouseDown;

bool GetAnyKeyUp();
bool GetAnyKeyDown();
void StoreKeyDownInput(WPARAM key);
void StoreKeyUpInput(WPARAM key);
void StoreMouseLeftDownInput(WPARAM key);
void StoreMouseLeftUpInput(WPARAM key);
void StoreMouseRightDownInput(WPARAM key);
void StoreMouseRightUpInput(WPARAM key);
bool GetKeyUpState(int key);
bool GetKeyDownState(int key);
bool GetMouseLeftDownState();
bool GetMouseLeftUpState();
bool GetMouseRightDownState();
bool GetMouseRightUpState();
bool GetAsyncKey(WPARAM key); //Just a wrapper call to GetAsyncKeyState for portability purposes
void InputEnd();
