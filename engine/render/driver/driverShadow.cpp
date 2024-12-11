#include "driverShadow.h"
#include "../../material/cubeMaterial.h"
#include "../../camera/perspectiveCamera.h"
#include "../../camera/orthographicCamera.h"
#include "../../material/depthMaterial.h"
#include "../render.h"

namespace JB
{
	DriverShadow::DriverShadow(std::weak_ptr<Light> light)
	{
		m_light = light;

		auto lightPtr = m_light.lock();
		m_driverRenderTarget = std::make_shared<DriverRenderTarget>(lightPtr->getLightShadow()->getRenderTarget());
	}

	DriverShadow::~DriverShadow()
	{
	}

	void DriverShadow::render(std::shared_ptr<Renderer> renderer, DriverRenderList::Ptr driverRenderList, Scene::Ptr scene)
	{
		auto lightPtr = m_light.lock();
		if (!lightPtr)
			return;

		auto depthMaterial = std::make_shared<DepthMaterial>();
		auto camera = lightPtr->getLightShadow()->getCamera();

		if (lightPtr->getLightsType() == LightsType::DirectionalLights)
		{
			auto orthographicCamera = std::dynamic_pointer_cast<OrthographicCamera>(camera);
			//TODO 使用包围盒/包围球精确参数
			orthographicCamera->setPerspective(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);
		}

		camera->setPosition(lightPtr->getPosition() * -1.0f);
		camera->lookAt(glm::vec3(0, 0, 0), glm::vec3(0.0, 1.0, 0.0));
		camera->updateModelMatrix(true, true);

		scene->setMaterial(depthMaterial);

		m_driverRenderTarget->use();

		auto width = lightPtr->getLightShadow()->getRenderTarget()->getWidth();
		auto height = lightPtr->getLightShadow()->getRenderTarget()->getHeight();
		glViewport(0, 0, width, height);
		
		glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
		
		renderer->renderScene(driverRenderList, scene, camera);
		m_driverRenderTarget->nonUse();

		scene->setMaterial(nullptr);
	}

	DriverRenderTarget::Ptr DriverShadow::getDriverRenderTarget()
	{
		return m_driverRenderTarget;
	}
}
