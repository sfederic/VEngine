export module Physics.CollisionLayers;

export enum class CollisionLayers : int
{
	None = 0x00,
	Editor = 0x01,
	All = 0x02
};

export inline CollisionLayers operator|(CollisionLayers a, CollisionLayers b)
{
	return static_cast<CollisionLayers>(static_cast<int>(a) | static_cast<int>(b));
}

export CollisionLayers operator&(CollisionLayers a, CollisionLayers b)
{
	return static_cast<CollisionLayers>(static_cast<int>(a) & static_cast<int>(b));
}
