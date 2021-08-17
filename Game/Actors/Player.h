#pragma once
#include "Object/Actor.h"

class Player : public nc::Actor 
{
public:

	Player(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed);

	void Initialize() override;

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 };
	float fireRate{ 0.2f };
	const float IDuration{ 2.0f };
	float ITimer{ IDuration };
	float speed{ 3000 };
	nc::Vector2 velocity;
};
