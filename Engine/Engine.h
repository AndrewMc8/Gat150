#pragma once

#define REGISTER_CLASS(class) nc::ObjectFactory::Instance().Register<class>(#class);

//systems
#include "Audio/AudioSystem.h"

//core
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Utilities.h"
#include "Core/Json.h"
#include "Core/Serializable.h"

//input
#include "Input/InputSystem.h"

//math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

//graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/Font.h"

//physics
#include "Physics/PhysicsSystem.h"

//objects
#include "Object/Actor.h"
#include "Object/Scene.h"
#include "Object/TileMap.h"

//components
#include "Components/SpriteComponent.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimationComponent.h"
#include "Components/RBPhysicsComponent.h"
#include "Components/AudioComponent.h"
#include "Components/TextComponent.h"

//Framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

//resource
#include "Resource/ResourseSystem.h"

//files
#include "Files/File.h"

#include <vector>
#include <memory>
#include <algorithm>


namespace nc
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());
		}

		return nullptr;
	}
}