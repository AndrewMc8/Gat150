#pragma once
#include "Engine.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"

class Game
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver,
	};

public: 
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool IsQuit() { return quit; }

private:
	void UpdateTitle(float dt);
	void UpdateLevelStart(float dt);
	void OnPlayerDead(const nc::Event& event);
	void OnAddPoints(const nc::Event& event);
	void On1UP(const nc::Event& event);

public:
	std::unique_ptr<nc::Engine> engine;
	std::unique_ptr<nc::Scene> scene;

private:
	bool quit = false;
	eState state = eState::Title;
	float stateTimer = 0.0f;
	float gameOverTimer = 0.0f;
	int levelCounter = 1;

	size_t score = 0;
	size_t HighScore = 0;
	size_t lives = 0;

	nc::AudioChannel musicChannel;
	std::shared_ptr<nc::Texture> particleTexture;
	std::shared_ptr<nc::Texture> textTexture;
};
