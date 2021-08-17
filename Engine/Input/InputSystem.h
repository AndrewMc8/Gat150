#pragma once
#include "SDL.h"
#include "Framework/System.h"
#include "Math/Vector2.h"
#include <vector>
#include <array>

namespace nc
{
	class InputSystem : public System
	{
	public:
		enum class eKeyState
		{
			Idle,
			Pressed,
			Held,
			Released
		};

		enum class eMouseButton
		{
			Left,
			Middle,
			Right
		};

		eKeyState GetKeyState(int id);
		bool IsKeyDown(int id) const { return keyboardState[id]; }
		bool IsPreviousKeyDown(int id) const { return prevKeyboardState[id]; }

		virtual void Startup() override;
		virtual void Shutdown() override;
		virtual void Update(float dt) override;

		const Vector2& GetMousePosition() { return mousePosition; }
		bool IsButtonDown(int id) { return mouseButtonState[id]; }
		bool IsPreviousButtonDown(int id) { return prevMouseButtonState[id]; }
		eKeyState GetButtonState(int id);

	private:
		std::vector<Uint8> keyboardState;
		std::vector<Uint8> prevKeyboardState;
		int numKeys;

		Vector2 mousePosition;
		std::array<Uint8, 3> mouseButtonState;
		std::array<Uint8, 3> prevMouseButtonState;
	};

}