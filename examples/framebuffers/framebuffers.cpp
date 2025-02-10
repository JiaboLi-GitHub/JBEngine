#include <iostream>
#include <chrono>
#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/geometries/boxGeometry.h"
#include "../../engine/cameraControl/fpsCameraControl.h"
#include "../../engine/material/baseMaterial.h"
#include "../../engine/texture/texture2D.h"
#include "../../engine/render/renderTarget.h"

using namespace JB;

uint32_t WIDTH = 1280;
uint32_t HEIGHT = 720;

FPSCameraControl::Ptr fpsCameraControl = nullptr;

static void onFrameSizeCallback(int width, int height)
{
	fpsCameraControl->mouseStateReset();
}

static void onMouseMove(double xpos, double ypos)
{
	fpsCameraControl->onMouseMove(xpos, ypos);
}

static void onMouseAction(JB::MouseAction action)
{
	fpsCameraControl->onMouseAction(action);
}

static void onKeyboardAction(KeyBoardState action)
{
	fpsCameraControl->onKeyboard(action);
}

int main()
{
	std::vector<float> vertices = 
	{
		0.5f,  0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f,
	   -0.5f,  0.5f, 0.0f
	};

	std::vector<float> uv = 
	{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};

	auto geometry = std::make_shared<Geometry>();
	geometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(vertices, 3));
	geometry->setAttribute(AttributeType::UV, std::make_shared<AttributeF>(uv, 2));
	geometry->setIndex(std::make_shared<AttributeUInt32>(indices, 1));

	auto material2D = std::make_shared<BaseMaterial>();
	material2D->setTexture(std::make_shared<Texture2D>(std::string("resources/wall.jpg")));

	auto mesh2D = Mesh::create(geometry, material2D);
	mesh2D->setPosition(2, 0, -3);

	auto boxGeometry = std::make_shared<BoxGeometry>(10.0f, 10.0f, 10.0f);
	auto texture2D = std::make_shared<Texture2D>(std::string("resources/wall.jpg"));

	auto material = std::make_shared<BaseMaterial>();
	material->setTexture(texture2D);

	auto mesh = Mesh::create(boxGeometry, material);
	mesh->setPosition(0, 0, -50);

	auto scene = Scene::create();
	scene->addChild(mesh);
	scene->addChild(mesh2D);

	std::unordered_map<TextureCubeTarget, std::string> filePathMap = {
		{TextureCubeTarget::POSITIVE_X,"resources/skybox/right.jpg"},
		{TextureCubeTarget::NEGATIVE_X,"resources/skybox/left.jpg"},
		{TextureCubeTarget::POSITIVE_Y,"resources/skybox/top.jpg"},
		{TextureCubeTarget::NEGATIVE_Y,"resources/skybox/bottom.jpg"},
		{TextureCubeTarget::POSITIVE_Z,"resources/skybox/front.jpg"},
		{TextureCubeTarget::NEGATIVE_Z,"resources/skybox/back.jpg"}
	};
	auto textureCube = std::make_shared<TextureCube>(filePathMap);
	scene->setSkyBoxTexture(textureCube);

	auto camera = PerspectiveCamera::create(glm::radians(45.0f), (float)WIDTH / (float)(HEIGHT), 0.1f, 100.0f);
	fpsCameraControl = std::make_shared<FPSCameraControl>(camera);

	Renderer::Descriptor rDc;
	rDc.width = WIDTH;
	rDc.height = HEIGHT;
	Renderer::Ptr renderer = Renderer::create(rDc);
	renderer->init();
	renderer->setMouseActionCallback(onMouseAction);
	renderer->setKeyboardActionCallBack(onKeyboardAction);
	renderer->setFrameSizeCallBack(onFrameSizeCallback);
	renderer->setMouseMoveCallBack(onMouseMove);
	renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	RenderTarget::Ptr renderTarget = std::make_shared<RenderTarget>();

	while (true)
	{
		renderTarget->setSize(renderer->getWidth(), renderer->getHeight());

		material->setTexture(texture2D);
		renderer->setRenderTarget(renderTarget);
		if (!renderer->render(scene, camera))
		{
			break;
		}

		material->setTexture(renderTarget->getColorAttachment());
		material2D->setTexture(renderTarget->getColorAttachment());
		renderer->setRenderTarget(nullptr);
		if (!renderer->render(scene, camera))
		{
			break;
		}

		renderer->swap();

		fpsCameraControl->update();
	}

	return 0;
}
