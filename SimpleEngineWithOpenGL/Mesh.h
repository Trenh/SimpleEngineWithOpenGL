#pragma once
#include <string>
#include <vector>
#include "Texture.h"


class Mesh
{
public :
	Mesh();
	~Mesh();

	void unload();

	class VertexArray* getVertexArray() { return vertexArray; }
	const string& getShaderName() const { return shaderName; }
	float geRadius() const { return radius; }

	void addTexture(Texture* texture);
	Texture* getTexture(int index);
	float getSpeculatPower()const { return specularPower; }

	void setVertexArray(VertexArray* vertexArrayP);
	void setShaderName(const string& shaderNameP);
	void setRadius(float radiusP);
	void setSpecularPower(float specularPowerP);

private:
	std::vector<Texture*> textures;
	class VertexArray* vertexArray;
	string shaderName;
	float radius; //Bounding shpere radius
	float specularPower;

};

