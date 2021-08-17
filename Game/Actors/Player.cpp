#include "Player.h"
#include "Projectile.h"
#include "Math/MathUtils.h"
#include "Engine.h"
#include"Enemy.h"

#include <memory>

Player::Player(const nc::Transform& transform, std::shared_ptr<nc::Texture> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed }
{
	
}

void Player::Initialize()
{
	std::unique_ptr shoot = std::make_unique<Actor>();
	shoot->transform.localPosition = nc::Vector2{ 0, 4 };
	AddChild(std::move(shoot));

	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 3, -2 };
	locator->transform.localRotation = nc::DegToRad(180);
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ -3, -2 };
	AddChild(std::move(locator));

	/*std::unique_ptr engine = std::make_unique<Actor>(nc::Transform{}, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("engine.txt"));
	engine->transform.localPosition = nc::Vector2{ 3, -4 };
	AddChild(std::move(engine));

	engine = std::make_unique<Actor>(nc::Transform{}, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("engine.txt"));
	engine->transform.localPosition = nc::Vector2{ -3, -4 };
	AddChild(std::move(engine));*/
}

void Player::Update(float dt)
{
	Actor::Update(dt);

	//chillins[1]->transform.localRotation += 5 * dt;

	float thrust = 0;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held) thrust = speed;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == nc::InputSystem::eKeyState::Held) thrust = -speed;

	nc::Vector2::Rotate(nc::Vector2::up, transform.rotation);

	//acceleration
	nc::Vector2 acceleration;
	acceleration = (nc::Vector2::Rotate(nc::Vector2::down, transform.rotation) * (thrust));
	//nc::Vector2 gravity = (nc::Vector2{ 400, 300 } - transform.position).Normalized() * 200;
	//nc::Vector2 gravity = nc::Vector2::down * 200;
	//acceleration += gravity;

	velocity += acceleration * dt;
	transform.position += velocity * dt;

	velocity *= 0.99f;

	//if (Core::Input::IsPressed('S')) input.y = 1;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 800.0f);

	//fire
	fireTimer -= dt;
	ITimer -= dt;
	if (fireTimer <= 0 && scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		fireTimer = fireRate;

		std::vector<nc::Vector2> points = { {-5, -5}, { 5, -5}, { 0, 24}, { -5, -5 } };
		std::shared_ptr<nc::Texture> shape2 = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("cannonBall.png", scene->engine->Get<nc::Renderer>());
		
		{
			nc::Transform t = chillins[0]->transform;
			t.scale = 0.5f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, shape2, 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}


		/*nc::Transform t = chillins[1]->transform;
		t.scale = 0.5f;

		std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, shape2, 600.0f);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));*/

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("Player_Shoot");

	}

	scene->engine->Get<nc::ParticleSystem>()->Create(chillins[1]->transform.position, 3, 2, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("cannonBall.png", scene->engine->Get<nc::Renderer>()), 50, chillins[1]->transform.localRotation, nc::DegToRad(30));
	scene->engine->Get<nc::ParticleSystem>()->Create(chillins[2]->transform.position, 3, 2, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("cannonBall.png", scene->engine->Get<nc::Renderer>()), 50, chillins[1]->transform.localRotation, nc::DegToRad(30));
}

void Player::OnCollision(Actor* actor)
{
	if ((dynamic_cast<Enemy*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy")) && ITimer <= 0)
	{

		//destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 20, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>()), 550);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("Player_Death");

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("*BANG* You are dead! No big surprise.");
		scene->engine->Get<nc::EventSystem>()->Notify(event);

		ITimer = IDuration;
	}
}