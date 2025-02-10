#include <iostream>
#include <chrono>
#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/cameraControl/fpsCameraControl.h"
#include "../../engine/model/assimpLoader.h"

using namespace JB;

uint32_t WIDTH = 1280;
uint32_t HEIGHT = 720;

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

	while (true)
	{
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

	auto planeGeometry = std::make_shared<Geometry>();
	planeGeometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
	planeGeometry->setAttribute(AttributeType::Normal, std::make_shared<AttributeF>(normals, 3));
	planeGeometry->setAttribute(AttributeType::UV, std::make_shared<AttributeF>(texCoords, 2));

	auto planeTexture2D = std::make_shared<Texture2D>(std::string("resources/white.png"));
	planeTexture2D->setTextureFilterMin(TextureFilter::LinearMipmapLinear);

	auto planeMaterial = std::make_shared<PhongLightingMaterial>();
	planeMaterial->setDiffuse(planeTexture2D);
	planeMaterial->setSpecular(planeTexture2D);

	auto plane = Mesh::create(planeGeometry, planeMaterial);

	auto model = AssimpLoader::load("resources/nanosuit/nanosuit.obj");
	model->object3D->setPosition(0, 0, -50);

	auto dirLight = std::make_shared<DirectionalLight>();
	dirLight->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight->setDiffuse(glm::vec3(0.7f, 0.7f, 0.7f));
	dirLight->setSpecular(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight->setPosition(-8, -8, -8);

	scene = Scene::create();
	scene->addChild(plane);
	scene->addChild(model->object3D);
	scene->addChild(dirLight);
}

void initCamera()
{
	camera = PerspectiveCamera::create(glm::radians(45.0f), (float)WIDTH / (float)(HEIGHT), 0.1f, 100.0f);
	fpsCameraControl = std::make_shared<FPSCameraControl>(camera);
}

void release()
{
	camera = nullptr;
	fpsCameraControl = nullptr;
	scene = nullptr;
}
