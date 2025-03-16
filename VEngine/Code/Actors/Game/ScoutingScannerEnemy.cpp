
#include "ScoutingScannerEnemy.h"

void ScoutingScannerEnemy::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	MoveToNextNode(deltaTime);
}

void ScoutingScannerEnemy::MoveToNextNode(float deltaTime)
{
	constexpr float moveInterval = 5.f;
	moveTimer += deltaTime;
	if (moveTimer > moveInterval)
	{
		moveTimer = 0.f;
		MoveToRandomNodeInMovementRange();
	}
}

Properties ScoutingScannerEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
