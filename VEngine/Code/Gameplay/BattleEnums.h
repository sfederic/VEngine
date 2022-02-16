#pragma once

//Use 'count' as a max to determine random attack/defend directions

enum class AttackDirection : int
{
	Up,
	Down,
	Left,
	Right,
	Count
};

enum class DefendDirection : int
{
	Up,
	Down,
	Left,
	Right,
	Count
};

inline bool operator==(AttackDirection atk, DefendDirection def)
{
	return (static_cast<int>(atk) == static_cast<int>(def));
}

inline bool operator==(DefendDirection def, AttackDirection atk)
{
	return (static_cast<int>(def) == static_cast<int>(atk));
}
