#include "Engine.h"

namespace nc
{
	void Engine::Startup()
	{
		systems.push_back(std::make_unique<Renderer>()); //renderer
		systems.push_back(std::make_unique<EventSystem>()); //events
		systems.push_back(std::make_unique<AudioSystem>()); //audio
		systems.push_back(std::make_unique<ResourceSystem>()); //resources
		systems.push_back(std::make_unique<InputSystem>()); //input
		systems.push_back(std::make_unique<ParticleSystem>()); //particles
		systems.push_back(std::make_unique<PhysicsSystem>()); //physics

		std::for_each(systems.begin(), systems.end(), [](auto& system) { system->Startup(); });

		REGISTER_CLASS(Actor)
		REGISTER_CLASS(SpriteComponent)
		REGISTER_CLASS(SpriteAnimationComponent)
		REGISTER_CLASS(PhysicsComponent)
		REGISTER_CLASS(RBPhysicsComponent)
		REGISTER_CLASS(AudioComponent)
		REGISTER_CLASS(TextComponent)
	}

	void Engine::Shutdown()
	{
		std::for_each(systems.begin(), systems.end(), [](auto& system) { system->Shutdown(); });
	}

	void Engine::Update()
	{
		time.Tick();
		std::for_each(systems.begin(), systems.end(), [this](auto& system) { system->Update(this->time.deltaTime); });
	}

	void Engine::Draw(Renderer* renderer)
	{
		std::for_each(systems.begin(), systems.end(), [renderer](auto& system) { if (dynamic_cast<GraphicsSystem*>(system.get())) dynamic_cast<GraphicsSystem*>(system.get())->Draw(renderer); });
	}
}