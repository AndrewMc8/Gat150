#pragma once
#include "Components/Component.h"
#include "Engine.h"
#include "Framework/EventSystem.h"

class PickupComponent : public nc::Component
{
public:
	std::unique_ptr<Object> Clone() const { return std::make_unique<PickupComponent>(*this); }

	~PickupComponent();

	void Create() override;
	virtual void Update() override;

	virtual void OnCollisionEnter(const nc::Event& event);
	virtual void OnCollisionExit(const nc::Event& event);

	virtual bool Read(const rapidjson::Value& value) override;
	virtual bool Write(const rapidjson::Value& value) const override;
};