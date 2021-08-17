#include "Enemy.h"
#include "Player.h"
#include "Engine.h"
#include "Projectile.h"
#include "Pickups.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::down, transform.rotation);

		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (dt * 3);

		//transform.rotation = nc::Lerp(transform.rotation, transform.rotation + turnAngle, dt * 2);

		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= nc::DegToRad(10))
		{
			fireTimer = fireRate;

			std::vector<nc::Vector2> points = { {-5, -5}, { 5, -5}, { 0, 24}, { -5, -5 } };
			std::shared_ptr<nc::Texture> shape2 = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("cannonBall.png", scene->engine->Get<nc::Renderer>());

			nc::Transform t = transform;
			t.scale = 0.5f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, shape2, 600.0f);
			projectile->tag = "Enemy";
			scene->AddActor(std::move(projectile));

			scene->engine->Get<nc::AudioSystem>()->PlayAudio("Enemy_Shoot");

		}
	}

	transform.position += (nc::Vector2::Rotate(nc::Vector2::down, transform.rotation) * (speed * dt));
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 800.0f);
}

void Enemy::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")
	{
		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 20, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>()), 550);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("Enemy_Death");

		int random = rand() % 50 + 1;

		if (random == 42)
		{
			nc::Transform t = transform;
			t.scale = 0.5f;

			std::shared_ptr<nc::Texture> pickup = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("1up.png", scene->engine->Get<nc::Renderer>());

			std::unique_ptr<Pickups> pickups = std::make_unique<Pickups>(t, pickup);
			pickups->tag = "pickup";
			scene->AddActor(std::move(pickups));
		}
		
		nc::Event event;
		event.name = "AddPoints";
		event.data = 100;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
