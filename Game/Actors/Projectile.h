#pragma once
#include "Object/Actor.h"

class Projectile : public nc::Actor
{
public:

	Projectile(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } {}
	void Update(float dt) override;
	void OnCollision(Actor* actor);
	

private:
	float lifetime{ 2 };
	float speed{ 300 };
};