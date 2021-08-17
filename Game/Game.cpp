#include "Game.h"


void Game::Initialize()
{
	//create engine
	engine = std::make_unique<nc::Engine>(); //new Engine()
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("Gat150", 800, 600);

	//create scene
	scene = std::make_unique<nc::Scene>(); //new Scene()
	scene->engine = engine.get();

	nc::SetFilePath("../Resources");

	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(nc::Transform{ nc::Vector2{400, 300}, 0, 5 });
	{
		std::unique_ptr<nc::SpriteComponent> component = std::make_unique<nc::SpriteComponent>();
		component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine->Get<nc::Renderer>());
		actor->AddComponent(std::move(component));
	}
	{
		std::unique_ptr<nc::PhysicsComponent> component = std::make_unique<nc::PhysicsComponent>();
		component->ApplyForce(nc::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}
	scene->AddActor(std::move(actor));
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