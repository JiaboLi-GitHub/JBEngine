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

using namespace JB;

uint32_t WIDTH = 1280;
uint32_t HEIGHT = 720;

constexpr int RowMeshSize = 3;
constexpr int ColumnMeshSize = 3;

FPSCameraControl::Ptr fpsCameraControl = nullptr;
PerspectiveCamera::Ptr camera = nullptr;
std::vector<Mesh::Ptr> meshs;

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
	for (auto& mesh : meshs)
	{
		mesh->rotateAroundAxis(glm::vec3(1.0, 1.0, 1.0), 0.5f);
	}
}

int main()
{
	auto scene = Scene::create();

	auto boxGeometry = std::make_shared<BoxGeometry>(1.0f, 1.0f, 1.0f);
	for (int i = 1; i <= RowMeshSize; ++i)
	{
		for (int j = 1; j <= ColumnMeshSize; ++j)
		{
			auto material = std::make_shared<PhongLightingMaterial>();
			material->setDiffuse(std::make_shared<Texture2D>(std::string("resources/container2.png")));
			material->setSpecular(std::make_shared<Texture2D>(std::string("resources/container2_specular.png")));
			material->setShininess(i * j * 10.0f);
			auto mesh = Mesh::create(boxGeometry, material);
			mesh->setPosition(i * 2 - RowMeshSize * 1.25f, j * 2 - ColumnMeshSize * 1.25f, -10);
			meshs.push_back(mesh);
			scene->addChild(mesh);
		}
	}

	DirectionalLight::Ptr dirLight = std::make_shared<DirectionalLight>();
	dirLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
	dirLight->setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f));
	dirLight->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	dirLight->setPosition(0, 0, -100);
	dirLight->setCastShadow(false);
	scene->addChild(dirLight);

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
	renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (true)
	{
		if (!renderer->render(scene, camera))
		{
			break;
		}
		renderer->swap();

		fpsCameraControl->update();

		rotateCube();
	}

	camera = nullptr;
	fpsCameraControl = nullptr;
	meshs.clear();
	return 0;
}
