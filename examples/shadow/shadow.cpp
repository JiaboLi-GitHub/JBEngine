#include <iostream>
#include <chrono>
#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/geometries/boxGeometry.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/cameraControl/fpsCameraControl.h"

using namespace JB;

constexpr uint32_t WIDTH = 600;
constexpr uint32_t HEIGHT = 400;

constexpr int RowMeshSize = 3;
constexpr int ColumnMeshSize = 3;

Scene::Ptr scene = nullptr;
FPSCameraControl::Ptr fpsCameraControl = nullptr;
PerspectiveCamera::Ptr camera = nullptr;

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

	release();
	return 0;
}

void initScene()
{
	std::vector<float> positions = {
		25.0f, -0.5f,  25.0f,
	   -25.0f, -0.5f,  25.0f,
	   -25.0f, -0.5f, -25.0f,
		25.0f, -0.5f,  25.0f,
	   -25.0f, -0.5f, -25.0f,
		25.0f, -0.5f, -25.0f
	};

	std::vector<float> normals = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	std::vector<float> texCoords = {
		25.0f,  0.0f,
		0.0f,   0.0f,
		0.0f,  25.0f,
		25.0f,  0.0f,
		0.0f,  25.0f,
		25.0f, 25.0f
	};

	//地板
	auto planeGeometry = std::make_shared<Geometry>();
	planeGeometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
	planeGeometry->setAttribute(AttributeType::Normal, std::make_shared<AttributeF>(normals, 3));
	planeGeometry->setAttribute(AttributeType::UV, std::make_shared<AttributeF>(texCoords, 2));

	auto planeTexture2D = std::make_shared<Texture2D>(std::string("resources/wood.png"));
	planeTexture2D->setTextureFilterMin(TextureFilter::LinearMipmapLinear);

	auto planeMaterial = std::make_shared<PhongLightingMaterial>();
	planeMaterial->setDiffuse(planeTexture2D);
	planeMaterial->setSpecular(planeTexture2D);

	auto plane = Mesh::create(planeGeometry, planeMaterial);

	//箱子
	auto boxGeometry = std::make_shared<BoxGeometry>(1.0f, 1.0f, 1.0f);

	auto boxTexture2D = std::make_shared<Texture2D>(std::string("resources/wood.png"));
	boxTexture2D->setTextureFilterMin(TextureFilter::LinearMipmapLinear);

	auto boxMaterial = std::make_shared<PhongLightingMaterial>();
	boxMaterial->setDiffuse(boxTexture2D);
	boxMaterial->setSpecular(boxTexture2D);

	auto mesh1 = Mesh::create(boxGeometry, boxMaterial);
	mesh1->setPosition(-1, 0, -8);

	auto mesh2 = Mesh::create(boxGeometry, boxMaterial);
	mesh2->setPosition(-3, 0, -6);
	mesh2->rotateAroundAxis(glm::vec3(0.0, 1.0, 0.0), 60);

	auto mesh3 = Mesh::create(boxGeometry, boxMaterial);
	mesh3->setPosition(1, 0.5, -6);
	mesh3->rotateAroundAxis(glm::vec3(1.0, 1.0, 1.0), 60);

	//灯光
	auto dirLight = std::make_shared<DirectionalLight>();
	dirLight->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight->setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	dirLight->setSpecular(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight->setPosition(-8, -8, -8);

	scene = Scene::create();
	scene->addChild(plane);
	scene->addChild(mesh1);
	scene->addChild(mesh2);
	scene->addChild(mesh3);
	scene->addChild(dirLight);
}

void initCamera()
{
	camera = PerspectiveCamera::create(glm::radians(45.0f), (float)WIDTH / (float)(HEIGHT), 0.1f, 100.0f);
	camera->setPosition(-1, 0, -1);

	fpsCameraControl = std::make_shared<FPSCameraControl>(camera);
}

void release()
{
	camera = nullptr;
	fpsCameraControl = nullptr;
	scene = nullptr;
}
