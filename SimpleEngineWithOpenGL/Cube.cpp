#include "Cube.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Assets.h"

Cube::Cube() : Actor(), moveComponent(nullptr),ratioRota(Vector2()),rotation(true),distanceLaunch(Vector2())
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

		Quaternion q(Vector3::unitZ, ratioRota.x);
		setRotation(Quaternion::concatenate(getRotation(), q));
		setRotation(Quaternion::concatenate(getRotation(), Quaternion(Vector3::unitY, ratioRota.y)));

		//if ratio rota isn't small 
		// multiply by 0.99 each frame
		if (Maths::abs( ratioRota.length())>0.01) {
			ratioRota *= 0.99;
		//	printf("%d",ratioRota);
		}
		else {
			ratioRota = Vector2();
		}
	}
	else
	{
		//move  when hold mouse button
		int xMouse, yMouse;
		SDL_GetMouseState(&xMouse, &yMouse);
		dt += deltaTime;
		 Vector2 currentRota =  Vector2(xMouse, yMouse);
		 float lengthX = (midRota - currentRota).x / 1000 ;
		 float lengthY = (midRota - currentRota).y/1000;
		Quaternion q(Vector3::unitZ,lengthX);
		setRotation(Quaternion::concatenate(getRotation(), q));
		setRotation(Quaternion::concatenate(getRotation(), Quaternion (Vector3::unitY, lengthY)));

		midRota = currentRota;
	}
}


void Cube::setRatioRota(Vector2 ratioRotaP)
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
		setRatioRota(Vector2((distanceLaunch.x/5000)*((1-dt) >0 ? (1 - dt): 0)
			, (distanceLaunch.y/ 5000 ) * ((1 - dt) > 0 ? (1 - dt) : 0)));
		distanceLaunch = Vector2();
	}
}

