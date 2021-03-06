#include "Engine.h"

namespace nc
{
	void Engine::Startup()
	{
		systems.push_back(std::make_unique<EventSystem>()); //events
		systems.push_back(std::make_unique<ResourceSystem>()); //resources

		std::for_each(systems.begin(), systems.end(), [](auto& system) { system->Startup(); });
	}

	void Engine::Shutdown()
	{
		std::for_each(systems.begin(), systems.end(), [](auto& system) { system->Shutdown(); });
	}

	void Engine::Update(float dt)
	{
		std::for_each(systems.begin(), systems.end(), [dt](auto& system) { system->Update(dt); });
	}

	void Engine::Draw()
	{
		//
	}
}