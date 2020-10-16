#include "Cube.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Assets.h"

Cube::Cube() : Actor(), moveComponent(nullptr),ratioRota(0.0),rotation(true),distanceLaunch(Vector2())
{
	MeshComponent* mc = new MeshComponent(this);
	mc->setMesh(Assets::getMesh("Mesh_Cube"));
	moveComponent = new MoveComponent(this);
}


void Cube::updateActor(float deltaTime)
{
	Actor::updateActor(deltaTime);
	if (rotation) 
	{

		Quaternion q(Vector3::unitZ, ratioRota);

		setRotation(Quaternion::concatenate(getRotation(), q));
		//if ratio rota isn't small 
		// multiply by 0.99 each frame
		if (Maths::abs( ratioRota)>0.01) {
			ratioRota *= 0.99;
		//	printf("%d",ratioRota);
		}
		else {
			ratioRota = 0;
		}
	}
	else
	{
		//move  when hold mouse button
		int xMouse, yMouse;
		SDL_GetMouseState(&xMouse, &yMouse);
		dt += deltaTime;
		 Vector2 currentRota =  Vector2(xMouse, yMouse);
		 float length = (midRota - currentRota).length() / 1000 * ((currentRota - midRota).x < 0 ? 1 : -1);
		Quaternion q(Vector3::unitZ,length);
		setRotation(Quaternion::concatenate(getRotation(), q));
		midRota = currentRota;
	}
}


void Cube::setRatioRota(float ratioRotaP)
{
	ratioRota = ratioRotaP;
}

void Cube::actorInput(SDL_MouseButtonEvent& mouseEvent)

{
	if (mouseEvent.button == SDL_BUTTON_LEFT &&mouseEvent.type == SDL_MOUSEBUTTONDOWN )
	{
		rotation = false;
		int xMouse, yMouse;
		SDL_GetMouseState(&xMouse, &yMouse);
		distanceLaunch = Vector2(xMouse, yMouse);
		midRota = Vector2(xMouse, yMouse);
		dt = 0;
	}

	if (mouseEvent.button == SDL_BUTTON_LEFT && mouseEvent.type == SDL_MOUSEBUTTONUP)
	{
		rotation = true;

		int xMouse, yMouse;
		SDL_GetMouseState(&xMouse, &yMouse);

		distanceLaunch -= Vector2(xMouse, yMouse);

		// set le ratio de rotation en fonction de la distance et de la vitesse de lancer
		setRatioRota((distanceLaunch.length()/5000*(distanceLaunch.x < 0 ?-1:1))*((1-dt) >0 ? (1 - dt): 0));
		distanceLaunch = Vector2();
	}
}

