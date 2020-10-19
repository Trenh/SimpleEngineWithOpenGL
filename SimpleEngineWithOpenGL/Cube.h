#pragma once
#include "Actor.h"


class Cube :public Actor
{

public:
	Cube();

	void updateActor(float deltaTime)override;
	void actorInput(SDL_MouseButtonEvent& mouseEvent) override;

	void setRatioRota(Vector2 ratioRotaP);
private:
	class MoveComponent* moveComponent;
	Vector2 ratioRota;
	Vector2 distanceLaunch;
	bool rotation;
	Vector2 midRota;
	float dt;
};


