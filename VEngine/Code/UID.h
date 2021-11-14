#pragma once
#include <cstdint>

//TODO: if UIDs stay as uint32_t's, then I need to make a Qt spinbox widget that uints

//UID = Unique Identifier!
typedef uint32_t UID;

UID GenerateUID();
