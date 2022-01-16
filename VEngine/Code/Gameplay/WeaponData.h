#pragma once

struct WeaponData
{
	int energyCost = 1; //How much energy weapon costs to use
	int attackPoints = 0; //How much damage weapon does
	int defendPoints = 0; //How much damage weapon negates
	int weight = 1; //Movement cost while moving with weapon out (Make sure it's never lower than 1)
};
