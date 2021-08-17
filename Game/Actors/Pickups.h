#pragma once
#include "Object/Actor.h"

class Pickups : public nc::Actor
{
public:

	Pickups(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture) : nc::Actor{ transform, texture } {}

	void Update(float dt) override;
	void OnCollision(Actor* actor);


private:
	float lifetime{ 20 };
};