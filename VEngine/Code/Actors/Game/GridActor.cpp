import Actors.Game.GridActor;

GridActor::GridActor()
{
	_mesh = CreateComponent("Mesh", MeshComponent("cube.vmesh", "test.png"));
	SetRootComponent(_mesh);

	dialogueComponent = CreateComponent<DialogueComponent>("Dialogue");
}

void GridActor::Create()
{
	__super::Create();

	_mesh->SetRenderStatic(false);

	SetInteractText();
}

bool GridActor::CheckNextRotationBoundsIntersect()
{
	if (ignoreRotationValidCheck)
	{
		return false;
	}

	BoundingOrientedBox nextRotBounds = _mesh->GetBoundsInWorldSpace();
	XMStoreFloat4(&nextRotBounds.Orientation, nextRot);
	//Lower extents so the actor isn't hitting things like the floor.
	nextRotBounds.Extents.x -= 0.15f;
	nextRotBounds.Extents.y -= 0.15f;
	nextRotBounds.Extents.z -= 0.15f;
	HitResult hit(this);
	return Physics::OrientedBoxCast(hit, nextRotBounds, true, true);
}

void GridActor::OnLinkActivate()
{
	isLinked = true;

	if (bigGridActor)
	{
		Grid::system.GetOnlyActor()->RecalcNodesToIgnoreLinkedGridActor(this);
	}
	else
	{
		HitResult hit(this);
		GetCurrentNode()->RecalcNodeHeight(hit);
	}
}

void GridActor::OnLinkDeactivate()
{
	isLinked = false;

	HitResult hit;
	if (bigGridActor)
	{
		//Make sure to include this actor again so that nodes get placed on top of it.
		Grid::system.GetOnlyActor()->RecalcAllNodes(hit, true);
	}
	else
	{
		GetCurrentNode()->RecalcNodeHeight(hit);
	}
}

void GridActor::OnLinkRotate()
{
	isRotating = true;
	SpawnDustSpriteSheet();
	GameUtils::PlayAudioOneShot(linkRotateAudio);
}

void GridActor::CheckSetIsMoving()
{
	if (isMoving)
	{
		if (CheckMovementAndRotationStopped())
		{
			isMoving = false;
			OnMoveEnd();
		}
	}
}

void GridActor::CheckSetIsRotating()
{
	if (isRotating)
	{
		if (CheckMovementAndRotationStopped())
		{
			isRotating = false;

			if (bigGridActor)
			{
				auto grid = Grid::system.GetOnlyActor();
				HitResult hit(this);
				grid->RecalcAllNodes(hit, true);
			}

			OnRotationEnd();
		}
	}
}

void GridActor::FallCheck()
{
	if (!canFall)
	{
		return;
	}

	if (!CheckMovementAndRotationStopped())
	{
		return;
	}

	if (inFall)
	{
		RecalcCurrentNodeDontIgnoreThis();
		inFall = false;

		auto node = GetCurrentNode();
		node->active = true;
	}
	else
	{
		HitResult hit(this);
		if (!Physics::Raycast(hit, GetPositionV(), -VMath::GlobalUpVector(), 1.f))
		{
			inFall = true;
			RecalcCurrentNodePosition();
			nextPos = GetCurrentNode()->GetWorldPosV();

			auto node = GetCurrentNode();
			node->active = false;
		}
	}
}

void GridActor::OnLinkMove()
{
	isMoving = true;
	SpawnDustSpriteSheet();
	GameUtils::PlayAudioOneShot(linkMoveAudio);
}

void GridActor::Inspect()
{
	auto player = Player::system.GetOnlyActor();
	player->SetCameraTargetActor(this);
	player->SetZoomedInCameraFOV();
}

void GridActor::Start()
{
	__super::Start();

	nextPos = GetPositionV();
	nextRot = GetRotationV();

	SetGridPosition();

	if (!IsActive())
	{
		auto node = GetCurrentNode();
		if (node)
		{
			node->Show();
		}
	}
}

void GridActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	CheckSetIsMoving();
	CheckSetIsRotating();

	CheckIfSubmerged();

	if (!isSubmerged)
	{
		FallCheck();
	}

	if (inFall)
	{
		SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, fallSpeed));
	}
	else
	{
		SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, moveSpeed));
	}

	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, rotateSpeed));

	dialogueComponent->SetPosition(GetHomogeneousPositionV());
}

Properties GridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();

	props.Add("Destruct", &isDestructible);
	props.Add("Health", &health);
	props.Add("Interact", &isInteractable);
	props.Add("Inspect", &isInspectable);
	props.Add("Interact Text", &interactTextKey).change = [&](Property&) { SetInteractText(); };
	props.Add("InteractKnownText", &interactKnownText);
	props.Add("DisableGridInteract", &disableGridInteract);
	props.Add("Obstacle", &isGridObstacle);
	props.Add("Link Rotation", &canBeRotatedInLink);
	props.Add("Link Movement", &canBeMovedInLink);
	props.Add("Ignore Rotate Check", &ignoreRotationValidCheck);
	props.Add("Rotate Yaw", &canBeRotatedYawYAxis);
	props.Add("Rotate Pitch", &canBeRotatedPitchXAxis);
	props.Add("Rotate Roll", &canBeRotatedRollZAxis);
	props.Add("Move Axis Pos.", &validPositiveMovementAxis);
	props.Add("Move Axis Neg.", &validNegativeMovementAxis);
	props.Add("Move Speed", &moveSpeed);
	props.Add("MoveOnlyZAxis", &moveConstrainedToZAxis);
	props.Add("MoveOnlyXAxis", &moveConstrainedToXAxis);
	props.Add("Rotate Speed", &rotateSpeed);
	props.Add("Can Fall", &canFall);
	props.Add("BigGridActor", &bigGridActor);

	props.Add("Audio LinkMove", &linkMoveAudio).autoCompletePath = "/Audio/";
	props.Add("Audio LinkRotate", &linkRotateAudio).autoCompletePath = "/Audio/";

	return props;
}

void GridActor::InflictDamage(int damage)
{
	Attacked();

	if (!isDestructible) return;

	health -= damage;
	if (healthWidget)
	{
		healthWidget->healthPoints = health;
	}

	if (health <= 0)
	{
		//Hide node if attacked Unit is the last enemy
		if (dynamic_cast<Unit*>(this))
		{
			GetCurrentNode()->Hide();
		}
		else
		{
			GetCurrentNode()->Show();
		}

		HitResult hit(this);
		GetCurrentNode()->RecalcNodeHeight(hit);
		Remove();
	}
}

void GridActor::SetGridPosition()
{
	xIndex = std::round(GetPosition().x);
	yIndex = std::round(GetPosition().z);
}

GridNode* GridActor::GetCurrentNode()
{
	if (disableGridInteract)
	{
		return nullptr;
	}

	auto grid = Grid::system.GetFirstActor();
	auto node = grid->GetNode(xIndex, yIndex);
	return node;
}

void GridActor::DisableCurrentNode()
{
	auto node = GetCurrentNode();
	node->Hide();
}

void GridActor::EnableCurrentNode()
{
	auto node = GetCurrentNode();
	node->Show();
}

void GridActor::RecalcCurrentNodePosition()
{
	auto node = GetCurrentNode();
	HitResult hit(this);
	node->RecalcNodeHeight(hit);
}

void GridActor::RecalcCurrentNodeDontIgnoreThis()
{
	auto node = GetCurrentNode();
	HitResult hit;
	node->RecalcNodeHeight(hit);
}

bool GridActor::CheckNextNodeMoveIsValid(const XMVECTOR nextMoveDirection)
{
	nextMoveCardinalDirection = nextMoveDirection;

	VMath::RoundVector(nextMoveCardinalDirection);

	const int nextXIndex = (int)std::round(nextPos.m128_f32[0]);
	const int nextYIndex = (int)std::round(nextPos.m128_f32[2]);

	auto grid = Grid::system.GetOnlyActor();

	const XMVECTOR currentPos = GetPositionV();

	if (nextXIndex >= grid->GetSizeX() || nextYIndex >= grid->GetSizeY()
		|| nextXIndex < 0 || nextYIndex < 0)
	{
		nextPos = currentPos;
		return false;
	}

	auto nextNodeToMoveTo = grid->GetNode(nextXIndex, nextYIndex);
	if (!nextNodeToMoveTo->active)
	{
		nextPos = currentPos;
		return false;
	}

	auto nextNode = grid->GetNode(nextXIndex, nextYIndex);

	//Check next node height
	if (nextNode->worldPosition.y > (GetPosition().y + Grid::maxHeightMove))
	{
		nextPos = currentPos;
		return false;
	}

	//Check if player is standing on next node
	auto player = Player::system.GetOnlyActor();
	int playerGridX, playerGridY;
	player->GetGridIndices(playerGridX, playerGridY);
	if (nextNode->Equals(playerGridX, playerGridY))
	{
		nextPos = currentPos;
		return false;
	}

	//Fence check
	HitResult fenceHit(this);
	if (Physics::Raycast(fenceHit, GetPositionV(), nextMoveCardinalDirection, 1.0f))
	{
		if (fenceHit.GetHitActorAs<FenceActor>())
		{
			Log("[%s] hit fence.", GetName().c_str());
			nextPos = GetPositionV();
			return false;
		}
	}

	//Big grid actor bounds check against all meshes of other grid actors
	if (bigGridActor)
	{
		//Going to trick the bounds in world space creation so that it's working with the future position.
		//Make sure to reset back to original position at end of the code block and returns.
		SetPosition(nextPos);

		for (auto mesh : GetComponents<MeshComponent>())
		{
			for (auto& gridActor : GridActor::system.GetActors())
			{
				//Ignore self
				if (gridActor.get() == this)
				{
					continue;
				}

				for (auto gridActorMesh : gridActor->GetComponents<MeshComponent>())
				{
					auto bounds = mesh->GetBoundsInWorldSpace();
					//Shrink extents down so we're not colliding with every bounds edge in the world.
					bounds.Extents.x -= 0.1f;
					bounds.Extents.y -= 0.1f;
					bounds.Extents.z -= 0.1f;

					if (bounds.Intersects(gridActorMesh->GetBoundsInWorldSpace()))
					{
						Log("[%s] big grid actor can't move. Intersecting with [%s].",
							GetName().c_str(), gridActor->GetName().c_str());
						SetPosition(currentPos);
						nextPos = GetPositionV();
						return false;
					}
				}
			}
		}

		SetPosition(currentPos);
	}

	//Note that both bools can be set to true, in which case z axis will take precedence.
	if (moveConstrainedToZAxis)
	{
		if (!VMath::VecEqual(nextMoveCardinalDirection, VMath::GlobalForwardVector()) &&
			!VMath::VecEqual(nextMoveCardinalDirection, -VMath::GlobalForwardVector()))
		{
			Log("[%s] constrained to Z axis. Can't be moved in this direction.", GetName().c_str());
			nextPos = GetPositionV();
			return false;
		}
	}
	else if (moveConstrainedToXAxis)
	{
		if (!VMath::VecEqual(nextMoveCardinalDirection, VMath::GlobalRightVector()) &&
			!VMath::VecEqual(nextMoveCardinalDirection, -VMath::GlobalRightVector()))
		{
			Log("[%s] constrained to X axis. Can't be moved in this direction.", GetName().c_str());
			nextPos = GetPositionV();
			return false;
		}
	}

	nextPos = XMLoadFloat3(&nextNode->worldPosition);

	//Make it so that floating actors won't be on the same Y pos as the node.
	if (!canFall)
	{
		nextPos.m128_f32[1] = GetPosition().y;
	}

	//When a GridActor moves, need to recalculate the Grid node height of the last position it was at.
	const int lastGridIndexX = (int)std::round(GetPosition().x);
	const int lastGridIndexY = (int)std::round(GetPosition().z);
	GridNode* lastNode = grid->GetNode(lastGridIndexX, lastGridIndexY);
	HitResult lastPosHit;
	lastPosHit.actorsToIgnore.emplace_back(this);
	lastPosHit.actorsToIgnore.emplace_back(Player::system.GetOnlyActor());
	lastNode->RecalcNodeHeight(lastPosHit);

	return true;
}

ForwardFace GridActor::GetCurrentForwardFace()
{
	const XMFLOAT3 forward = GetForwardVector();

	const int forwardIndex = static_cast<int>(std::lroundf(forward.z));
	const int rightIndex = static_cast<int>(std::lroundf(forward.x));

	if (forwardIndex > 0) return ForwardFace::positiveZ;
	else if (forwardIndex < 0) return ForwardFace::negativeZ;
	else if (rightIndex > 0) return ForwardFace::positiveX;
	else if (rightIndex < 0) return ForwardFace::negativeX;

	throw new std::exception("something went wrong with forward vector");
}

bool GridActor::CheckMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}

void GridActor::RoundPosition()
{
	XMVECTOR pos = GetPositionV();
	VMath::RoundVector(pos);
	SetNextPos(pos);
}

void GridActor::AddNextRot(XMVECTOR axis, float angle)
{
	auto newRotation = XMQuaternionMultiply(nextRot,
		DirectX::XMQuaternionRotationAxis(axis,
			XMConvertToRadians(Core::GetDeltaTime() * angle)));
	SetNextRot(newRotation);
}

void GridActor::ResetNextPos()
{
	nextPos = GetPositionV();
}

void GridActor::ResetNextRot()
{
	nextRot = GetRotationV();
}

void GridActor::SetMeshFilename(std::string_view filename)
{
	_mesh->SetMeshFilename(filename);
}

void GridActor::AddNextRotation(XMVECTOR axis, float angle)
{
	nextRot = VMath::AddRotationAngle(nextRot, axis, angle);
}

void GridActor::AddNextPosition(XMVECTOR offset)
{
	nextPos += offset;
}

bool GridActor::IsNextMoveAxisValid(XMVECTOR direction)
{
	const float xAxis = direction.m128_f32[0];
	const float yAxis = direction.m128_f32[2];

	if (xAxis == 1 && validPositiveMovementAxis.x == 0) return false;
	if (xAxis == -1 && validNegativeMovementAxis.x == 0) return false;

	if (yAxis == 1 && validPositiveMovementAxis.y == 0) return false;
	if (yAxis == -1 && validNegativeMovementAxis.y == 0) return false;

	return true;
}

void GridActor::CheckIfSubmerged()
{
	if (!CheckMovementAndRotationStopped())
	{
		return;
	}

	for (auto& waterVolume : WaterVolume::system.GetActors())
	{
		auto pos = GetPositionV();
		if (waterVolume->Contains(pos))
		{
			isSubmerged = true;

			//Grab the backside hit of the water volume and set the grid actor's position to that hit pos.
			HitResult hit(this);
			hit.ignoreBackFaceHits = false;
			if (Physics::Raycast(hit, GetPositionV(), VMath::GlobalUpVector(), 15.f))
			{
				if (hit.GetHitActorAs<WaterVolume>())
				{
					SetNextPos(hit.GetHitPosV());
					return;
				}
			}
		}
	}

	isSubmerged = false;
}

void GridActor::DisableAllInteractivity()
{
	isDestructible = false;
	isInspectable = false;
	disableGridInteract = true;
	canBeRotatedInLink = false;
	canBeMovedInLink = false;
	canBeRotatedYawYAxis = false;
	canBeRotatedPitchXAxis = false;
	ignoreRotationValidCheck = true;
}

void GridActor::SetInteractText()
{
	interactText = Localise::GetString(interactTextKey, LocalisationFiles::gridActorInteractLines);
}

void GridActor::SpawnDustSpriteSheet() const
{
	XMVECTOR spawnPos = GetPositionV();
	spawnPos.m128_f32[1] -= 0.25f;
	auto& sprite = GameUtils::SpawnSpriteSheet("Sprites/dust.png", spawnPos, false, 3, 2);
	sprite.SetAnimationSpeed(15.f);
}
