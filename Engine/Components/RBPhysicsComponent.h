#pragma once
#include "PhysicsComponent.h"
#include "Physics/PhysicsSystem.h"

namespace nc
{
	class RBPhysicsComponent : public PhysicsComponent
	{
	public:
		void Update() override;
		virtual void ApplyForce(const Vector2& force) override;

		virtual bool Read(const rapidjson::Value& value) override;
		virtual bool Write(const rapidjson::Value& value) const override;

	public:
		PhysicsSystem::RigidBodyData data;
		b2Body* body = nullptr;
	};
}