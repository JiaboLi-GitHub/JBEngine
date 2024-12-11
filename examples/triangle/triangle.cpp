#include "../../engine/render/render.h"
#include "../../engine/objects/mesh.h"
#include "../../engine/camera/perspectiveCamera.h"
#include "../../engine/material/baseMaterial.h"

using namespace JB;

uint32_t WIDTH = 800;
uint32_t HEIGHT = 600;

int main()
{
	std::vector<float> positions = {
		-0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	std::vector<float> colors = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	std::vector<uint32_t> indices = {
		0, 1, 2
	};

	auto geometry = std::make_shared<Geometry>();
	geometry->setAttribute(AttributeType::Position, std::make_shared<AttributeF>(positions, 3));
	geometry->setAttribute(AttributeType::Color, std::make_shared<AttributeF>(colors, 3));
	geometry->setIndex(std::make_shared<AttributeUInt32>(indices, 1));

	auto material = std::make_shared<BaseMaterial>();

	auto mesh = Mesh::create(geometry, material);
	mesh->setPosition(0, 0, -3);

	auto scene = Scene::create();
	scene->addChild(mesh);

	auto camera = PerspectiveCamera::create(glm::radians(45.0f), (float)WIDTH / (float)(HEIGHT), 0.1f, 100.0f);

	Renderer::Descriptor rDc;
	rDc.width = WIDTH;
	rDc.height = HEIGHT;
	Renderer::Ptr renderer = Renderer::create(rDc);
	renderer->init();
	renderer->setClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	while (true) {
		if (!renderer->render(scene, camera)) {
			break;
		}
		renderer->swap();
	}

	return 0;
}
