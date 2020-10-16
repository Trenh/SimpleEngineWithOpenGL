#include "RendererOGL.h"
#include <GL/glew.h>
#include "Rectangle.h"
#include "Vector2.h"
#include "Assets.h"
#include "Log.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "Actor.h"

#include <SDL_image.h>
#include <utility>

RendererOGL::RendererOGL() :
	window(nullptr),
	spriteVertexArray(nullptr),
	context(nullptr),
	spriteViewProj(Matrix4::createSimpleViewProj(WINDOW_WIDTH, WINDOW_HEIGHT)),
	view(Matrix4::createLookAt(Vector3::zero,Vector3::unitX,Vector3::unitZ)),
	projection(Matrix4::createPerspectiveFOV(Maths::toRadians(70.0f),WINDOW_WIDTH,WINDOW_HEIGHT,25.0f, 10000.0f)),
	ambientLight{Vector3(1.0f,1.0f,1.0f)},
	dirLight({ Vector3::zero, Vector3::zero, Vector3::zero})
{
}

RendererOGL::~RendererOGL()
{
}

bool RendererOGL::initialize(Window& windowP)
{
	window = &windowP;

	// Set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	// Depth buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// OpenGL Context
	context = SDL_GL_CreateContext(windowP.getSDLWindow());

	// GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Log::error(LogCategory::Video, "Failed to initialize GLEW.");
		return false;
	}

	// On some platforms, GLEW will emit a benign error code, so clear it
	glGetError();

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		Log::error(LogCategory::Video, "Unable to initialize SDL_image");
		return false;
	}

	spriteVertexArray = new VertexArray(spriteVertices, 4, indices, 6);
	return true;
}

void RendererOGL::beginDraw()
{
	glClearColor(0.45f, 0.45f, 1.0f, 1.0f);
	// Clear the color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
	
}

void RendererOGL::draw()
{
	drawMeshes();
	drawSprites();
}

void RendererOGL::drawMeshes()
{
	//enable depht buffering/disable apha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	Shader& shader = Assets::getShader("Phong");
	shader.use();

	//Assets::getShader("BasicMesh").use();
	//update view-projection matrix
	shader.setMatrix4("uViewProj", view * projection);
	// Lights
	setLightUniforms(shader);
	//Draw
	for (auto& shader : Assets::shaders)
	{
		Shader& shade = Assets::getShader(shader.first);
		
		for (auto mc : meshes)
		{
			if (mc->getMesh()->getShaderName() == shader.first) {
				mc->draw(Assets::getShader(shader.first));

			}

		}
	
	}
	
}

void RendererOGL::drawSprite(const Actor& actor, const Texture& tex, Rectangle srcRect, Vector2 origin, Flip flip) const
{
	Matrix4 scaleMat = Matrix4::createScale((float)tex.getWidth(), (float)tex.getHeight(), 1.0f);
	Matrix4 world = scaleMat * actor.getWorldTransform();
	//Matrix4 pixelTranslation = Matrix4::createTranslation(Vector3(-WINDOW_WIDTH / 2 - origin.x, -WINDOW_HEIGHT / 2 - origin.y, 0.0f)); // Screen pixel coordinates
	Assets::getShader("Sprite").setMatrix4("uWorldTransform", world /** pixelTranslation*/);
	tex.setActive();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RendererOGL::endDraw()
{
	SDL_GL_SwapWindow(window->getSDLWindow());
}

void RendererOGL::close()
{
	SDL_GL_DeleteContext(context);
	delete spriteVertexArray;
}

void RendererOGL::addSprite(SpriteComponent* sprite)
{
	// Insert the sprite at the right place in function of drawOrder
	int spriteDrawOrder = sprite->getDrawOrder();
	auto iter = begin(sprites);
	for (; iter != end(sprites); ++iter)
	{
		if (spriteDrawOrder < (*iter)->getDrawOrder()) break;
	}
	sprites.insert(iter, sprite);
}

void RendererOGL::removeSprite(SpriteComponent* sprite)
{
	auto iter = std::find(begin(sprites), end(sprites), sprite);
	sprites.erase(iter);
}

void RendererOGL::drawSprites()
{
	glDisable(GL_DEPTH_TEST);
	//enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// activate shader and vertex array
	Shader& spriteShader = Assets::getShader("Sprite");
	spriteShader.use();
	spriteShader.setMatrix4("uViewProj",spriteViewProj);
	spriteVertexArray->setActive();

	for (auto sprite : sprites)
	{
		sprite->draw(*this);
	}
}

void RendererOGL::addMesh(MeshComponent* mesh)
{
	meshes.emplace_back(mesh);

}

void RendererOGL::removeMesh(MeshComponent* mesh)
{
	auto iter = std::find(begin(meshes), end(meshes), mesh);
	meshes.erase(iter);

}

void RendererOGL::setViewMatrix(const Matrix4& viewP)
{
	view = viewP;

}


void RendererOGL::setLightUniforms(Shader& shader)
{
	//camera position is from inverted view
	Matrix4 invertedView = view;
	invertedView.invert();
	shader.setVector3f("uCameraPos", invertedView.getTranslation());
	//Ambient
	shader.setVector3f("uAmbientLight", ambientLight);
	//DirectionnaleLight
	shader.setVector3f("uDirLight.direction", dirLight.direction);
	shader.setVector3f("uDirLight.diffuseColor", dirLight.diffuseColor);
	shader.setVector3f("uDirLight.specColor", dirLight.specColor);
}

void RendererOGL::setAmbientLight(const Vector3& ambientP)
{
	ambientLight = ambientP;
}