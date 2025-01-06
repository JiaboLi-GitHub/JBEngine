#include <iostream>
#include <chrono>
#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/cameraControl/fpsCameraControl.h"
#include "../../engine/model/assimpLoader.h"
#include "../../engine/tools/timer.h"
#include "../../engine/geometries/planeGeometry.h"

using namespace JB;

uint32_t WIDTH = 1280;
uint32_t HEIGHT = 720;

// 场景
Scene::Ptr scene = nullptr;

// 相机
FPSCameraControl::Ptr fpsCameraControl = nullptr;
PerspectiveCamera::Ptr camera = nullptr;

// 动画
AnimationAction::Ptr action = nullptr;
Timer::Ptr	timer = Timer::create();

void initScene();

void initCamera();

void release();

static void onFrameSizeCallback(int width, int height)
{
	fpsCameraControl->mouseStateReset();
	camera->setPerspective(glm::radians(45.0f), (float)width / (float)(height), 0.1f, 100.0f);
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
	initScene();

	initCamera();

	Renderer::Descriptor rDc;
	rDc.width = WIDTH;
	rDc.height = HEIGHT;
	Renderer::Ptr renderer = Renderer::create(rDc);
	renderer->init();
	renderer->setMouseActionCallback(onMouseAction);
	renderer->setKeyboardActionCallBack(onKeyboardAction);
	renderer->setFrameSizeCallBack(onFrameSizeCallback);
	renderer->setMouseMoveCallBack(onMouseMove);
	renderer->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	float lastTime = timer->elapsed_mill() / 1000.0f;

	while (true)
	{
		float currentTime = timer->elapsed_mill() / 1000.0f;

		float deltaTime = currentTime - lastTime;

		lastTime = currentTime;

		action->update(deltaTime);

		if (!renderer->render(scene, camera))
		{
			break;
		}
		renderer->swap();

		fpsCameraControl->update();
	}

	release();

	return 0;
}

void initScene()
{
	//地板
	auto planeGeometry = std::make_shared<PlaneGeometry>();

	auto planeTexture2D = std::make_shared<Texture2D>(std::string("resources/white.png"));
	planeTexture2D->setTextureFilterMin(TextureFilter::LinearMipmapLinear);

	auto planeMaterial = std::make_shared<PhongLightingMaterial>();
	planeMaterial->setDiffuse(planeTexture2D);
	planeMaterial->setSpecular(planeTexture2D);

	auto plane = Mesh::create(planeGeometry, planeMaterial);

	//模型
	auto model = AssimpLoader::load("resources/vampire/dancing_vampire.dae");
	model->object3D->setPosition(0, -0.5, 0);

	//灯光
	auto dirLight = std::make_shared<DirectionalLight>();
	dirLight->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight->setDiffuse(glm::vec3(0.6f, 0.6f, 0.6f));
	dirLight->setSpecular(glm::vec3(0.1f, 0.1f, 0.1f));
	dirLight->setPosition(-8, -8, -8);

	scene = Scene::create();
	scene->addChild(plane);
	scene->addChild(model->object3D);
	scene->addChild(dirLight);

	action = model->actions[0];
	action->play();
}

void initCamera()
{
	camera = PerspectiveCamera::create(glm::radians(45.0f), (float)WIDTH / (float)(HEIGHT), 0.1f, 100.0f);
	camera->setPosition(0, 0, 5);

	fpsCameraControl = std::make_shared<FPSCameraControl>(camera);
}

void release()
{
	camera = nullptr;
	fpsCameraControl = nullptr;
	scene = nullptr;
	action = nullptr;
	timer = nullptr;
}
