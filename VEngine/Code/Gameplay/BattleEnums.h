#pragma once

//The direction in world a player or Unit can attack an from (e.g. standing behind)
enum class AttackDirection : int
{
	Front = 0b00000001,
	Back  = 0b00000010,
	Left  = 0b00000100,
	Right = 0b00001000,
	All   = 0b00001111
};

inline bool operator&(AttackDirection lhs, AttackDirection rhs)
{
	return static_cast<std::underlying_type_t<AttackDirection>>(lhs) &
		static_cast<std::underlying_type_t<AttackDirection>>(rhs);
}

inline AttackDirection operator|(AttackDirection lhs, AttackDirection rhs)
{
	return static_cast<AttackDirection>(static_cast<std::underlying_type_t<AttackDirection>>(lhs) 
		| static_cast<std::underlying_type_t<AttackDirection>>(rhs));
}
