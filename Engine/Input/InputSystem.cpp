#include "InputSystem.h"

namespace nc
{

	void InputSystem::Startup()
	{
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);

		keyboardState.resize(numKeys);

		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

		prevKeyboardState = keyboardState;
	}

	void InputSystem::Shutdown()
	{

	}

	void InputSystem::Update(float dt)
	{
		prevKeyboardState = keyboardState;
		
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardStateSDL, keyboardStateSDL + numKeys, keyboardState.begin());

		prevMouseButtonState = mouseButtonState;
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = nc::Vector2{ x, y };
		mouseButtonState[0] = buttons & SDL_BUTTON_LMASK; // buttons [0001] & [0RML]
		mouseButtonState[1] = buttons & SDL_BUTTON_MMASK; // buttons [0010] & [0RML]
		mouseButtonState[2] = buttons & SDL_BUTTON_RMASK; // buttons [0100] & [0RML]
	}

	nc::InputSystem::eKeyState InputSystem::GetButtonState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool buttonDown = IsButtonDown(id);
		bool prevButtonDown = IsPreviousButtonDown(id);

		if (buttonDown)
		{
			state = (prevButtonDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevButtonDown) ? eKeyState::Released : eKeyState::Idle;
		}

		return state;
	}

	nc::InputSystem::eKeyState nc::InputSystem::GetKeyState(int id)
	{
		eKeyState state = eKeyState::Idle;

		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);

		if (keyDown)
		{
			state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed;
		}
		else
		{
			state = (prevKeyDown) ? eKeyState::Released : eKeyState::Idle;
		}

		return state;
	}
}