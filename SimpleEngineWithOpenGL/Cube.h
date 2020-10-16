#pragma once
#include "Actor.h"


class Cube :public Actor
{

public:
	Cube();

	void updateActor(float deltaTime)override;
	void actorInput(SDL_MouseButtonEvent& mouseEvent) override;

	void setRatioRota(float ratioRotaP);
private:
	class MoveComponent* moveComponent;
	float ratioRota;
	Vector2 distanceLaunch;
	bool rotation;
	Vector2 midRota;
	float dt;
};


