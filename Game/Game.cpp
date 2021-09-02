#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"

//int global = 10;

void Game::Initialize()
{
	//create engine
	engine = std::make_unique<nc::Engine>(); //new Engine()
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("Gat150", 800, 600);

	//register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);

	//create scene
	scene = std::make_unique<nc::Scene>(); //new Scene()
	scene->engine = engine.get();

	nc::SetFilePath("../Resources");

	engine->Get<nc::EventSystem>()->Subscribe("add_score", std::bind(&Game::OnAddScore, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("spawn", std::bind(&Game::OnSpawn, this, std::placeholders::_1));

	
	scene->engine->Get<nc::AudioSystem>()->AddAudio("music", "Audio/nostalgia-for-lost-times.wav");
	scene->engine->Get<nc::AudioSystem>()->PlayAudio("music");

	auto scoreActor = scene->FindActor("Score");
	if (scoreActor)
	{
		scoreActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(score));
	}

	//std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(nc::Transform{ nc::Vector2{400, 300}, 0, 1 });
	//{
	//	auto component = nc::ObjectFactory::Instance().Create<nc::SpriteAnimationComponent>("SpriteAnimationComponent");

	//	//nc::SpriteComponent* component = actor->AddComponent<nc::SpriteComponent>();
	//	//component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("character.png", engine->Get<nc::Renderer>());

	//	//nc::SpriteAnimationComponent* component = actor->AddComponent<nc::SpriteAnimationComponent>();
	//	component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sparkle.png", engine->Get<nc::Renderer>());
	//	component->fps = 24;
	//	component->numFramesX = 8;
	//	component->numFramesY = 8;
	//	actor->AddComponent(std::move(component));

	//}
	//{
	//	/*nc::PhysicsComponent* component = actor->AddComponent<nc::PhysicsComponent>();
	//	component->ApplyForce(nc::Vector2::right * 200);*/
	//}
	//scene->AddActor(std::move(actor));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	//update
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	// update score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor)
	{
		scoreActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(score));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::Reset()
{
	scene->RemoveAllActors();

	score = 0;

	rapidjson::Document document;
	//nc::json::Load("scene.txt", document);
	bool success = nc::json::Load("title.txt", document);
	assert(success);

	scene->Read(document);
	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		assert(title);
		title->destroy = true;

		state = eState::StartGame;
	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	//nc::json::Load("scene.txt", document);
	bool success = nc::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

	nc::TileMap tilemap;
	tilemap.scene = scene.get();
	success = nc::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;

	if (stateTimer >= 1)
	{
		auto player = nc::ObjectFactory::Instance().Create<nc::Actor>("Player");
		player->transform.position = nc::Vector2{ 400, 500 };
		scene->AddActor(std::move(player));

		auto coin = nc::ObjectFactory::Instance().Create<nc::Actor>("coin");
		coin->transform.position = nc::Vector2{ nc::RandomRange(0, 800), nc::RandomRange(300, 500) };
		scene->AddActor(std::move(coin));

		state = eState::Level;
	}
}

void Game::Level()
{
	auto player = scene->FindActor("Player");

	if (player->transform.position.x > 900 || player->transform.position.x < -100 || player->transform.position.y > 700 || player->transform.position.y < -100)
	{
		state = eState::Reset;
	}
}

void Game::PlayerDead()
{

}

void Game::GameOver()
{

}

void Game::OnAddScore(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnSpawn(const nc::Event& event)
{
	auto coin = nc::ObjectFactory::Instance().Create<nc::Actor>("coin");
	coin->transform.position = nc::Vector2{ nc::RandomRange(0, 800), nc::RandomRange(300, 500) };
	scene->AddActor(std::move(coin));

	auto bat = nc::ObjectFactory::Instance().Create<nc::Actor>("bat");
	bat->transform.position = nc::Vector2{ nc::RandomRange(0, 800), nc::RandomRange(300, 500) };
	scene->AddActor(std::move(bat));
}
