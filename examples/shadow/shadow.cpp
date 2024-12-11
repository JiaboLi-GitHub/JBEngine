#include <iostream>
#include <chrono>
#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/geometries/boxGeometry.h"
#include "../../engine/cameraControl/fpsCameraControl.h"
#include "../../engine/material/phongLightingMaterial.h"
#include "../../engine/texture/texture2D.h"
#include "../../engine/lights/directionalLight.h"
#include "../../engine/material/depthMaterial.h"
#include "../../engine/camera/orthographicCamera.h"

using namespace JB;

constexpr uint32_t WIDTH = 600;
constexpr uint32_t HEIGHT = 400;

constexpr int RowMeshSize = 3;
constexpr int ColumnMeshSize = 3;

FPSCameraControl::Ptr fpsCameraControl = nullptr;
PerspectiveCamera::Ptr camera = nullptr;

//地板
Scene::Ptr scene = nullptr;

DirectionalLight::Ptr dirLight = nullptr;

void initScene();


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

void rotateCube()
{

}

int main()
{
	initScene();

	std::vector<float> positions = {
		-1.0f,  1.0f, 0.0f,  // 左上角
		-1.0f, -1.0f, 0.0f,  // 左下角
		 1.0f, -1.0f, 0.0f,  // 右下角
		 1.0f,  1.0f, 0.0f   // 右上角
	};

	std::vector<float> colors = {
		1.0f, 0.0f, 0.0f,  // 红色 (左上角)
		0.0f, 1.0f, 0.0f,  // 绿色 (左下角)
		0.0f, 0.0f, 1.0f,  // 蓝色 (右下角)
		1.0f, 1.0f, 0.0f   // 黄色 (右上角)
	};

	std::vector<float> uvCoordinates = {
	0.0f, 1.0f,  // 左上角
	0.0f, 0.0f,  // 左下角
	1.0f, 0.0f,  // 右下角
	1.0f, 1.0f   // 右上角
	};

	std::vector<uint32_t> indices = {
		0, 1, 2,  // 第一个三角形
		0, 2, 3   // 第二个三角形
	};

	auto geometry = std::make_shared<Geometry>();
	geometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
	geometry->setAttribute(AttributeType::Color, std::make_shared<AttributeF>(colors, 3));
	geometry->setAttribute(AttributeType::UV, std::make_shared<AttributeF>(uvCoordinates, 2));
	geometry->setIndex(std::make_shared<AttributeUInt32>(indices, 1));

	auto material = std::make_shared<BaseMaterial>();

	auto mesh = Mesh::create(geometry, material);
	mesh->setPosition(0, 0, -3);
	//scene->addChild(mesh);

	dirLight = std::make_shared<DirectionalLight>();
	dirLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	dirLight->setDiffuse(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
	dirLight->setPosition(-8, -8, -8);
	//dirLight->setCastShadow(false);
	scene->addChild(dirLight);

	auto dirLight1 = std::make_shared<DirectionalLight>();
	dirLight1->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	dirLight1->setDiffuse(glm::vec3(0.3f, 0.3f, 0.3f));
	dirLight1->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
	dirLight1->setPosition(8, -8, -8);
	//dirLight->setCastShadow(false);
	//scene->addChild(dirLight1);

	camera = PerspectiveCamera::create(glm::radians(45.0f), (float)WIDTH / (float)(HEIGHT), 0.1f, 100.0f);

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

		rotateCube();

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

	camera = nullptr;
	fpsCameraControl = nullptr;
	scene = nullptr;
	dirLight = nullptr;
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
	auto planeTexture2D = std::make_shared<Texture2D>(std::string("E:/SoftWare_ProgRam/JBEngine/resources/wood.png"));
	planeTexture2D->setTextureFilterMin(TextureFilter::LinearMipmapLinear);
	auto planeMaterial = std::make_shared<PhongLightingMaterial>();
	planeMaterial->setDiffuse(planeTexture2D);
	planeMaterial->setSpecular(planeTexture2D);
	auto plane = Mesh::create(planeGeometry, planeMaterial);

	auto boxGeometry = std::make_shared<BoxGeometry>(1.0f, 1.0f, 1.0f);
	auto boxTexture2D = std::make_shared<Texture2D>(std::string("E:/SoftWare_ProgRam/JBEngine/resources/wood.png"));
	auto boxMaterial = std::make_shared<PhongLightingMaterial>();
	boxMaterial->setDiffuse(boxTexture2D);
	boxMaterial->setSpecular(boxTexture2D);

	auto mesh1 = Mesh::create(boxGeometry, boxMaterial);
	mesh1->setPosition(0, 0, -10);

	auto mesh2 = Mesh::create(boxGeometry, boxMaterial);
	mesh2->setPosition(-4, 0, -15);

	auto mesh3 = Mesh::create(boxGeometry, boxMaterial);
	mesh3->setPosition(4, 0.5, -10);

	auto mesh4 = Mesh::create(boxGeometry, boxMaterial);
	mesh4->setPosition(2, 0.5, -5);

	scene = Scene::create();
	scene->addChild(plane);
	scene->addChild(mesh1);
	scene->addChild(mesh2);
	scene->addChild(mesh3);
	scene->addChild(mesh4);
}
