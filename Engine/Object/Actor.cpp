#include "Actor.h"
#include "Graphics/Renderer.h"
#include "Components/GraphicsComponent.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		std::for_each(components.begin(), components.end(), [](auto& component) { component->Update(); });

		transform.Update();
		std::for_each(chillins.begin(), chillins.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		std::for_each(components.begin(), components.end(), [renderer](auto& component) {if (dynamic_cast<GraphicsComponent*>(component.get())) { dynamic_cast<GraphicsComponent*>(component.get())->Draw(renderer); }});

		std::for_each(chillins.begin(), chillins.end(), [renderer](auto& child) { child->Draw(renderer); });
	}

	float Actor::GetRadius()
	{
		return 0;
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->owner = this;
		components.push_back(std::move(component));
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		chillins.push_back(std::move(actor));
	}
}