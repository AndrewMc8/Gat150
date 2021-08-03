#include "Actor.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		transform.Update();
		std::for_each(chillins.begin(), chillins.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw()
	{

	}

	float Actor::GetRadius()
	{
		return 0;
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		chillins.push_back(std::move(actor));
	}
}