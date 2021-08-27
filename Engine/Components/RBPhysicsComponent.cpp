#include "RBPhysicsComponent.h"
#include"Engine.h"

void nc::RBPhysicsComponent::Update()
{
	if (!body)
	{
		body = owner->scene->engine->Get<PhysicsSystem>()->CreateBody(owner->transform.position, owner->transform.rotation, data, owner);
		body->SetGravityScale(data.gravityScale);
		body->SetLinearDamping(1);
	}

	owner->transform.position = PhysicsSystem::WorldToScreen(body->GetPosition());
	owner->transform.rotation = body->GetAngle();
	velocity = body->GetLinearVelocity();
}

void nc::RBPhysicsComponent::ApplyForce(const Vector2& force)
{
	if (body)
	{
		body->ApplyForceToCenter(force, true);
	}
}

bool nc::RBPhysicsComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, data.isDynamic);
	JSON_READ(value, data.isSensor);
	JSON_READ(value, data.lockAngle);
	JSON_READ(value, data.size);
	JSON_READ(value, data.density);
	JSON_READ(value, data.friction);
	JSON_READ(value, data.restitution);
	JSON_READ(value, data.gravityScale);

	return true;
}

bool nc::RBPhysicsComponent::Write(const rapidjson::Value& value) const
{
	return false;
}
