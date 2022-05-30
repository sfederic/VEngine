#pragma once

//@Todo: there's an interesting todo here where Loaded is only set when the engine is initially loaded.
//However, the enum here was originally for creating System resources in batches. 
//e.g. Create the structure for all the Textures and then on world load, create the actual D3D11 data at once
//for every texture.
//What's needed is for systems to be set back to Unloaded when their Cleanup() happens.
enum class SystemStates
{
	Unloaded,
	Loaded
};
