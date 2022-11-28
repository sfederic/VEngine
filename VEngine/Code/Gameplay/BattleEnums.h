#pragma once

//Use 'count' as a max to determine random attack/defend directions

//Attack Position is the position in world a player or Unit can attack an from (e.g. standing behind)
enum class AttackPosition : int
{
	Front = 0b00000001,
	Back  = 0b00000010,
	Left  = 0b00000100,
	Right = 0b00001000,
	All   = 0b00001111
};

inline bool operator&(AttackPosition lhs, AttackPosition rhs) 
{
	return static_cast<std::underlying_type_t<AttackPosition>>(lhs) &
		static_cast<std::underlying_type_t<AttackPosition>>(rhs);
}

inline AttackPosition operator|(AttackPosition lhs, AttackPosition rhs)
{
	return static_cast<AttackPosition>(static_cast<char>(lhs) | static_cast<char>(rhs));
}
