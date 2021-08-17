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

	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/nostalgia-for-lost-times.wav");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	//std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine->Get<nc::Renderer>());
	particleTexture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine->Get<nc::Renderer>());

	//for (size_t i = 0; i < 20; i++)
	//{
	//	nc::Transform transform{ { nc::RandomRangeInt(0, 800), nc::RandomRangeInt(0, 600)}, nc::RandomRange(0, 360), 1.0f };
	//	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	//	scene->AddActor(std::move(actor));
	//}

	// get font from resource system
	int size = 30;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/8514oem.fon", &size);

	// create font texture
	//textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
	//textTexture->Create(font->CreateSurface("hello world", nc::Color{ 1, 1, 1 }));
	// add font texture to resource system
	//engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

	//game
	engine->Get<nc::AudioSystem>()->AddAudio("Enemy_Death", "Enemy_Death.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Player_Death", "Player_Death.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Enemy_Shoot", "Enemy_Shoot.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Player_Shoot", "Player_shoot.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("Background_Music", "nostalgia-for-lost-times.wav");

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("1UP", std::bind(&Game::On1UP, this, std::placeholders::_1));

	HighScore = nc::File::LoadHighScore("score.txt");

	{
		std::shared_ptr<nc::Texture> titleTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		titleTexture->Create(font->CreateSurface("Scuffed Galaga", nc::Color{ 0, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("Scuffed Galaga", titleTexture);
	}

	{
		std::shared_ptr<nc::Texture> titleTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		titleTexture->Create(font->CreateSurface("Press SPACE To Start!", nc::Color{ 0, 1, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("Start", titleTexture);
	}

	{
		std::shared_ptr<nc::Texture> gameoverTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		gameoverTexture->Create(font->CreateSurface("GAME OVER", nc::Color{ 1, 0, 0 }));
		engine->Get<nc::ResourceSystem>()->Add("gameoverTexture", gameoverTexture);
	}

	{
		std::shared_ptr<nc::Texture> scoreTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		scoreTexture->Create(font->CreateSurface("0000", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("scoreTexture", scoreTexture);
	}

	{
		std::shared_ptr<nc::Texture> scoreTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		scoreTexture->Create(font->CreateSurface("0000", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("highScoreTexture", scoreTexture);
	}

	{
		std::shared_ptr<nc::Texture> scoreTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		scoreTexture->Create(font->CreateSurface("0000", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("livesTexture", scoreTexture);
	}

	{
		std::shared_ptr<nc::Texture> overheadTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		overheadTexture->Create(font->CreateSurface("Level: ", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("level", overheadTexture);
	}

	{
		std::shared_ptr<nc::Texture> overheadTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
		overheadTexture->Create(font->CreateSurface("0000", nc::Color{ 1, 1, 1 }));
		engine->Get<nc::ResourceSystem>()->Add("levelCount", overheadTexture);
	}
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	float dt = engine->time.deltaTime;

	stateTimer += dt;
	gameOverTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
	{
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
		{
			state = eState::StartGame;
		}
	}
		break;
	case Game::eState::StartGame:
	{
		score = 0;
		lives = 3;
		levelCounter = 1;
		state = eState::StartLevel;
	}
		break;
	case Game::eState::StartLevel:
	{
		UpdateLevelStart(dt);
	}
		break;
	case Game::eState::Game:
	{
		gameOverTimer = 10;

		if (scene->GetActors<Enemy>().size() == 0)
		{
			levelCounter++;

			int random = rand() % 10 + 1;

			for (int i = 0; i < random + levelCounter; i++)
			{
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 0.5f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip.png", engine->Get<nc::Renderer>()), 200.0f));
			}
		}

		if (lives == 0)
		{
			state = eState::GameOver;
		}
	}
		break;
	case Game::eState::GameOver:
		scene->RemoveAllActors();

		gameOverTimer -= 0.05f;

		if (gameOverTimer <= 0)
		{
			state = eState::Title;
		}


		break;
	default:
		break;
	}


	//update
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
	{
		nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
		engine->Get<nc::ParticleSystem>()->Create({ position.x, position.y }, 10, 1, particleTexture, 200);

		std::cout << position.x << " " << position.y << std::endl;

		engine->Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f), false);
		//musicChannel.SetPitch(nc::RandomRange(0.2f, 2.0f));
	}

	scene->Update(dt);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	switch (state)
	{
	case Game::eState::Title:
	{
		/*graphics.SetColor(nc::Color::green);
		graphics.DrawString(360, 300 + static_cast<int>(std::sin(stateTimer * 6) * 10), "Scuffed Galaga");*/

		{// score text
			{
				nc::Transform t;
				t.position = { 360, 300 + static_cast<int>(std::sin(stateTimer * 6) * 10) };
				engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Scuffed Galaga"), t);
			}

			{
				nc::Transform t;
				t.position = { 340, 360 + static_cast<int>(std::sin(stateTimer * 6) * 10) };
				engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Start"), t);
			}
		}

		/*graphics.SetColor(nc::Color::blue);
		graphics.DrawString(340, 360, "Press SPACE to Start!");*/

	}
	break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
	{
		/*graphics.SetColor(nc::Color::red);
		graphics.DrawString(360, 300 + static_cast<int>(std::sin(stateTimer * 6) * 10), "Game Over");*/

		{
			nc::Transform t;
			t.position = { 360, 300 + static_cast<int>(std::sin(stateTimer * 6) * 10) };
			engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("gameoverTexture"), t);
		}
		break;
	}


		//graphics.DrawString(30, 35, std::to_string(score).c_str());
		//graphics.DrawString(30, 20, std::to_string(HighScore).c_str());
		//graphics.DrawString(750, 20, std::to_string(lives).c_str());
		//graphics.DrawString(360, 20, "Level: ");
		//graphics.DrawString(410, 20, std::to_string(levelCounter).c_str());
	}

	{
		// score text
		nc::Transform t;
		t.position = { 30, 35 };
		std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("scoreTexture");
		std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/8514oem.fon");
		texture->Create(font->CreateSurface(std::to_string(score), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::Renderer>()->Draw(texture, t);
	}

	{
		nc::Transform t;
		t.position = { 30, 20 };
		std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("highScoreTexture");
		std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/8514oem.fon");
		texture->Create(font->CreateSurface(std::to_string(HighScore), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::Renderer>()->Draw(texture, t);
	}

	{
		nc::Transform t;
		t.position = { 750, 20 };
		std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("livesTexture");
		std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/8514oem.fon");
		texture->Create(font->CreateSurface(std::to_string(lives), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::Renderer>()->Draw(texture, t);
	}

	{
		nc::Transform t;
		t.position = { 360, 20 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("level"), t);
	}

	{
		nc::Transform t;
		t.position = { 410, 20 };
		std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("livesTexture");
		std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/8514oem.fon");
		texture->Create(font->CreateSurface(std::to_string(levelCounter), nc::Color{ 1, 1, 1 }));
		engine->Get<nc::Renderer>()->Draw(texture, t);
	}


	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	//nc::Transform t;
	//t.position = { 30, 30 };
	//engine->Get<nc::Renderer>()->Draw(textTexture, t);

	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::UpdateTitle(float dt)
{
	//if (Core::Input::IsPressed(VK_SPACE))
	//{
	//	//stateFunction = &Game::UpdateLevelStart;
	//	state = eState::StartGame;
	//}
}

	void Game::UpdateLevelStart(float dt)
	{
		{
			//scene->engine->Get<nc::AudioSystem>()->PlayAudio("Background_Music");

			scene->AddActor(std::make_unique<Player>(nc::Transform{ nc::Vector2{400, 300}, 0.0f, 0.5f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("ship.png", engine->Get<nc::Renderer>()), 1000.0f));

			int random = rand() % 10 + 1;

			for (size_t i = 0; i < random + levelCounter; i++)
			{
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 0.5f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("enemyShip.png", engine->Get<nc::Renderer>()), 200.0f));
			}
			state = eState::Game;
		}
	}

	void Game::OnPlayerDead(const nc::Event & event)
	{
		lives--;
	}

	void Game::OnAddPoints(const nc::Event & event)
	{
		score += std::get<int>(event.data);

		if (score > HighScore)
		{
			nc::File::SaveHighScore("score.txt", score);
			HighScore = score;
		}
	}

	void Game::On1UP(const nc::Event & event)
	{
		lives++;
	}