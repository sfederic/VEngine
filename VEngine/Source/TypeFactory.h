#pragma once

#include <unordered_map>

class ActorSystem;

//This is where every ActorSystem is registered at startup in a hashtable.
//For level loading, TypeFactory is used to call Init()s and whatever else per actor system.
//The map key is the Actor System's name from typeid().
std::unordered_map<const char*, ActorSystem*> actorSystemMap;
