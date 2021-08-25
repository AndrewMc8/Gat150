#include "Game.h"
#include "GameComponent/PlayerComponent.h"

//int global = 10;

void Game::Initialize()
{
	//create engine
	engine = std::make_unique<nc::Engine>(); //new Engine()
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("Gat150", 800, 600);

	//register classes
	REGISTER_CLASS(PlayerComponent);

	//create scene
	scene = std::make_unique<nc::Scene>(); //new Scene()
	scene->engine = engine.get();

	nc::SetFilePath("../Resources");

	rapidjson::Document document;
	//nc::json::Load("scene.txt", document);
	bool success = nc::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);

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
	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame();
}