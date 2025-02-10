#include <iostream>
#include <chrono>
#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/geometries/boxGeometry.h"
#include "../../engine/cameraControl/fpsCameraControl.h"
#include "../../engine/material/baseMaterial.h"
#include "../../engine/texture/texture2D.h"

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
	auto boxGeometry = std::make_shared<BoxGeometry>(10.0f, 10.0f, 10.0f);

	auto material = std::make_shared<BaseMaterial>();
	material->setTexture(std::make_shared<Texture2D>(std::string("resources/wall.jpg")));

	auto mesh = Mesh::create(boxGeometry, material);
	mesh->setPosition(0, 0, -50);

	auto scene = Scene::create();
	scene->addChild(mesh);

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

	while (true) 
	{
		if (!renderer->render(scene, camera)) 
		{
			break;
		}
		renderer->swap();

		fpsCameraControl->update();
	}

	return 0;
}
