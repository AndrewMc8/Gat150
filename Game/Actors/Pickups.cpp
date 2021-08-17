#include "Pickups.h"
#include "Player.h"
#include "Engine.h"

void Pickups::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	if (lifetime <= 0) destroy = true;
}

void Pickups::OnCollision(Actor* actor)
{
	if (dynamic_cast<Player*>(actor))
	{
		destroy = true;

		nc::Event event;
		event.name = "1UP";
		event.data = 1;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
