export module Core.OpenMode

import std.core;

export enum class OpenMode
{
	Out = std::ios_base::out,
	In = std::ios_base::in,
	Binary = std::ios_base::binary
};
