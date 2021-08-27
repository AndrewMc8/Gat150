#pragma once
#include "Components/Component.h"
#include "Engine.h"
#include "Framework/EventSystem.h"

class PlayerComponent : public nc::Component
{
public:
	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const nc::Event& event);
	virtual void OnCollisionExit(const nc::Event& event);

	virtual bool Read(const rapidjson::Value& value) override;
	virtual bool Write(const rapidjson::Value& value) const override;

public:
	float speed = 0;

private:
	std::list<nc::Actor*> contacts;
};