#include "EventSystem.h"

namespace nc
{
	void EventSystem::Startup()
	{

	}

	void EventSystem::Shutdown()
	{

	}

	void EventSystem::Update(float dt)
	{

	}

	void EventSystem::Subscribe(const std::string& name, function_t function, Object* reciever)
	{
		Observer observer;
		observer.function = function;
		observer.reciever = reciever;

		observers[name].push_back(observer);
	}

	void EventSystem::Unsubscribe(const std::string& name, Object* reciever)
	{
			auto& eventObservers = observers[name];
			for (auto iter = eventObservers.begin(); iter != eventObservers.end();)
			{
				if (iter->reciever == reciever)
				{
					iter = eventObservers.erase(iter);
				}
				else
				{
					iter++;
				}
			}
	}

	void EventSystem::Notify(const Event& event)
	{
		auto& eventObservers = observers[event.name];
		for (auto& observer : eventObservers)
		{
			if (event.reciever == nullptr || event.reciever == observer.reciever)
			{
				observer.function(event);
			}
		}
	}
}