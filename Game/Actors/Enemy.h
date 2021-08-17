#pragma once
#include "Object/Actor.h"

class Enemy : public nc::Actor
{
public:

	Enemy(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed } {}
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 };
	float fireRate{ 0.7f };
	float speed{ 300 };
};
