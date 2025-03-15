export module Render.RastStates;

import <string>;

export struct RastStates
{
	inline static const std::string solid = "solid";
	inline static const std::string noBackCull = "nobackcull";
	inline static const std::string frontCull = "frontCull";
	inline static const std::string wireframe = "wirefrmae";
	inline static const std::string shadow = "shadow";
};
