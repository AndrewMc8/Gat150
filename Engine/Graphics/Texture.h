#pragma once
#include "Resource/Resource.h"
#include "Math/Vector2.h"
#include <SDL.h>

namespace nc
{
	class Renderer;

	class Texture : public Resource
	{
	public:
		bool Load(const std::string& name, void* data) override;

		Texture() {}
		Texture(Renderer* renderer);

		Vector2 GetSize() const;

		friend class Renderer;

		bool Create(SDL_Surface* surface);

	private:
		SDL_Texture* texture = nullptr;
		SDL_Renderer* renderer = nullptr;
	};
}