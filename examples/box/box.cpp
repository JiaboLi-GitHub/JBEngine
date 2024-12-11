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

uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

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
	material->setTexture(std::make_shared<Texture2D>(std::string("E:/SoftWare_ProgRam/JBEngine/resources/wall.jpg")));

	auto mesh = Mesh::create(boxGeometry, material);
	mesh->setPosition(0, 0, -50);

	auto scene = Scene::create();
	scene->addChild(mesh);

	std::unordered_map<TextureCubeTarget, std::string> filePathMap = {
		{TextureCubeTarget::POSITIVE_X,"E:/SoftWare_ProgRam/JBEngine/resources/skybox/right.jpg"},
		{TextureCubeTarget::NEGATIVE_X,"E:/SoftWare_ProgRam/JBEngine/resources/skybox/left.jpg"},
		{TextureCubeTarget::POSITIVE_Y,"E:/SoftWare_ProgRam/JBEngine/resources/skybox/top.jpg"},
		{TextureCubeTarget::NEGATIVE_Y,"E:/SoftWare_ProgRam/JBEngine/resources/skybox/bottom.jpg"},
		{TextureCubeTarget::POSITIVE_Z,"E:/SoftWare_ProgRam/JBEngine/resources/skybox/front.jpg"},
		{TextureCubeTarget::NEGATIVE_Z,"E:/SoftWare_ProgRam/JBEngine/resources/skybox/back.jpg"}
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

	auto startTime = std::chrono::high_resolution_clock::now();
	int frameCount = 0;
	float fps = 0.0f;

	while (true)
	{
		if (!renderer->render(scene, camera))
		{
			break;
		}
		renderer->swap();

		fpsCameraControl->update();

		frameCount++;

		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsedTime = currentTime - startTime;

		if (elapsedTime.count() >= 1.0f)
		{
			fps = frameCount / elapsedTime.count();
			std::cout << "FPS: " << fps << std::endl;
			frameCount = 0;
			startTime = currentTime;
		}
	}

	return 0;
}
