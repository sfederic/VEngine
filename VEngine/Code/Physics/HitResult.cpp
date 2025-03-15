import Physics.HitResult;

Actor* HitResult::FindHitActor(Actor* findActor)
{
	for (auto actor : hitActors)
	{
		if (actor == findActor)
		{
			return actor;
		}
	}

	return nullptr;
}

Actor* HitResult::GetClosestHitActor(const DirectX::XMVECTOR point)
{
	struct ActorPack
	{
		Actor* actor = nullptr;
		float distance = 0.f;
	};

	std::vector<ActorPack> actorPacks;

	for (auto lHitActor : hitActors)
	{
		const float distance = XMVector3Length(point - lHitActor->GetPositionV()).m128_f32[0];
		ActorPack pack = { lHitActor , distance };
		actorPacks.emplace_back(pack);
	}

	auto DistCompare = [](const ActorPack& leftPack, const ActorPack& rightPack)
	{
		return leftPack.distance < rightPack.distance;
	};

	std::sort(actorPacks.begin(), actorPacks.end(), DistCompare);

	return actorPacks.front().actor;
}

void HitResult::AddActorsToIgnore(std::vector<Actor*>& actors)
{
	actorsToIgnore.reserve(actorsToIgnore.size() + actors.size());

	for (auto actor : actors)
	{
		actorsToIgnore.emplace_back(actor);
	}
}

void HitResult::IgnorePlayer()
{
	actorsToIgnore.emplace_back(Player::system.GetOnlyActor());
}

void HitResult::AddAllRenderStaticMeshesToIgnore()
{
	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		if (!mesh->IsRenderStatic())
		{
			componentsToIgnore.emplace_back(mesh.get());
		}
	}
}

std::vector<Vertex> HitResult::GetHitVertexFromHitActorMesh()
{
	std::vector<Vertex> outputVerts;

	const auto meshes = hitActor->GetComponents<MeshComponent>();
	for (const auto mesh : meshes)
	{
		const auto& meshVerts = mesh->GetAllVertices();
		for (const auto meshVertIndex : vertIndexesOfHitTriangleFace)
		{
			if (meshVertIndex > meshVerts.size())
			{
				throw new std::exception("something wong.");
			}

			const auto& meshVert = meshVerts.at(meshVertIndex);
			outputVerts.emplace_back(meshVert);
		}
	}

	return outputVerts;
}
