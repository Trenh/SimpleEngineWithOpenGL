#pragma once
#include "Component.h"
#include "Vector2.h"


class MoveComponent : public Component
{
public:
	MoveComponent(Actor* ownerP, int updateOrder = 10); //By default, update before other components
	MoveComponent() = delete;
	MoveComponent(const MoveComponent&) = delete;
	MoveComponent& operator=(const MoveComponent&) = delete;

	float getForwardSpeed() const { return forwardSpeed; }
	float getAngularSpeed() const { return angularSpeed; }

	void setForwardSpeed(float forwardSpeedP);
	void setAngularSpeed(float angularSpeedP);

	void setMass(float massP);
	void addForce(Vector2 force);

	void update(float dt) override;

private:
	float forwardSpeed;
	float angularSpeed;
	float mass;
	
	Vector2 sumForces;
	Vector2 velocity;


};