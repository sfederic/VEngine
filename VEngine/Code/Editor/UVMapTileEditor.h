#pragma once

//@Todo: One workflow idea these sort of PS1 JRPG titles have (or at least what I thought would be good)
//is to be able to paint textures directly onto faces. Everything in the engine is triangulated but the idea 
//is based on works like:
//https://jeiel.itch.io/sprytile
//https://crocotile3d.com/
//What those two example are missing though is directly painting to existing faces instead of creating quads from
//the selected area of the texture map.

//What this tool would need is a picker that could be resized to various dimensions (e.g. 1x16, 4x4, 8x4, etc.)
//with the texture map image loaded, and then able to map the selcted triangles to the texture map picker.

//The workflow currently in Blender isn't too bad, but this sort of tool is worth looking into down the line.