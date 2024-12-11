#include "render.h"
#include <stb_image.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "driver/driverGLSL.h"
#include "driver/driverShadow.h"
#include "../cache/cache.h"
#include "../objects/mesh.h"
#include "../material/phongLightingMaterial.h"
#include "../lights/directionalLight.h"
#include "../tools/errorAnalyzer.h"

namespace JB
{
	Renderer::Renderer(const Descriptor& descriptor)
	{
		m_width = descriptor.width;
		m_height = descriptor.height;
	}

	Renderer::Ptr Renderer::create(const Descriptor& descriptor)
	{
		return std::shared_ptr<Renderer>(new Renderer(descriptor));
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::init()
	{
		auto thisPtr = shared_from_this();
		m_driverWindow = std::make_shared<DriverWindow>(thisPtr, m_width, m_height);
		m_driverRenderList = std::make_shared<DriverRenderList>();
		m_driverState = std::make_shared<DriverState>();
		m_driverLightList = std::make_shared<DriverLightList>();
		m_driverState->setViewport(0, 0, m_width, m_height);
	}

	bool Renderer::render(Scene::Ptr scene, Camera::Ptr camera)
	{
		if (!m_driverWindow->processEvent())
			return false;

		m_driverRenderList->clear();
		m_driverLightList->clear();

		m_driverState->clearBufferBit();

		DriverTexture::init();

		scene->updateModelMatrix(true, true);
		camera->updateModelMatrix(true, true);

		buildRenderList(scene, camera);
		
		for (auto& driverLight : m_driverLightList->getAllDriverLights())
		{
			if (!driverLight.light.lock()->getCastShadow())
				continue;

			auto driverShadow = driverLight.driverShadow;
			driverShadow->render(shared_from_this(), m_driverRenderList, scene);
		}

		ErrorAnalyzer::checkErrors();

		m_driverState->use();

		ErrorAnalyzer::checkErrors();

		renderScene(m_driverRenderList, scene, camera);

		buildRenderSkyBox(scene, camera);

		return true;
	}

	void Renderer::swap()
	{
		m_driverWindow->swap();
	}

	void Renderer::setSize(int width, int height)
	{
		m_width = width;
		m_height = height;

		m_driverState->setViewport(0, 0, width, height);
	}

	int Renderer::getWidth()
	{
		return m_width;
	}

	int Renderer::getHeight()
	{
		return m_height;
	}

	void Renderer::setFrameSizeCallBack(const DriverWindow::FrameSizeCallback& callback)
	{
		m_driverWindow->setFrameSizeCallback(callback);
	}

	void Renderer::setMouseMoveCallBack(const DriverWindow::MouseMoveCallback& callback) 
	{
		m_driverWindow->setMouseMoveCallBack(callback);
	}

	void Renderer::setMouseActionCallback(const DriverWindow::MouseActionCallback& callback) 
	{
		m_driverWindow->setMouseActionCallback(callback);
	}

	void Renderer::setKeyboardActionCallBack(const DriverWindow::KeyboardActionCallback& callback) 
	{
		m_driverWindow->setKeyboardActionCallBack(callback);
	}

	void Renderer::setClearColor(float r, float g, float b, float a)
	{
		m_driverState->setClearColor(r, g, b, a);
	}

	void Renderer::setRenderTarget(const RenderTarget::Ptr& renderTarget)
	{
		m_driverRenderTarget = std::make_shared<DriverRenderTarget>(renderTarget);
		m_driverRenderTarget->use();
	}

	void Renderer::buildRenderList(Object3D::Ptr object, Camera::Ptr camera)
	{
		//可渲染物体
		if (object->getType() == Object3DType::RenderableObject || object->getType() == Object3DType::Mesh)
		{
			auto renderableObject = std::static_pointer_cast<RenderableObject>(object);
			auto modelViewMatrix = camera->getModelMatrixInverse() * object->getModelMatrix() * glm::vec4(object->getPosition(), 1.0);
			m_driverRenderList->push(renderableObject, modelViewMatrix.z);
		}

		//灯光
		if (object->getType() == Object3DType::Lights)
		{
			auto lights = std::static_pointer_cast<Light>(object);
			m_driverLightList->push(lights);
		}

		for (auto& child : object->getChildren())
		{
			buildRenderList(child, camera);
		}
	}

	void Renderer::buildRenderSkyBox(Scene::Ptr scene, Camera::Ptr camera)
	{
		if (!scene->getSkyBox())
			return;

		RenderItem renderItem = { scene->getSkyBox(), 0 };
		m_driverState->setDepthFunc(GL_LEQUAL);
		renderRenderItem(renderItem, scene, camera);
		m_driverState->setDepthFunc(GL_LESS);
	}

	void Renderer::renderScene(DriverRenderList::Ptr driverRenderList, Scene::Ptr scene, Camera::Ptr camera)
	{
		auto opaques = driverRenderList->getOpaques();
		auto transparents = driverRenderList->getTransparents();

		renderRenderItemList(opaques, scene, camera);
		renderRenderItemList(transparents, scene, camera);
	}

	void Renderer::renderRenderItemList(std::vector<RenderItem>& renderItems, Scene::Ptr scene, Camera::Ptr camera)
	{
		for (auto& renderItem : renderItems)
		{
			renderRenderItem(renderItem, scene, camera);
		}
	}

	void printMatrixRowMajor(const glm::mat4& matrix) {
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				std::cout << matrix[col][row] << " ";
			}
			std::cout << std::endl;
		}
	}

	void Renderer::renderRenderItem(RenderItem& renderItems, Scene::Ptr scene, Camera::Ptr camera)
	{
		auto globalMaterial = scene->getMaterial();
		auto renderableObject = renderItems.renderableObject;
		auto geometry = renderItems.renderableObject->getGeometry();
		auto material = renderItems.renderableObject->getMaterial();

		if (globalMaterial)
			material = globalMaterial;

		renderableObject->onBeforeRender(this, scene.get(), camera.get());

		{
			//DEBUG
	/*		if (material->getMaterialType() == MaterialType::BaseMaterial)
			{
				auto baseMaterial = std::static_pointer_cast<BaseMaterial>(material);
				auto driverDirLights = m_driverLightList->getDriverDirLights();
				auto driverDirLight = driverDirLights.at(0);
				baseMaterial->setTexture(driverDirLight.light.lock()->getLightShadow()->getRenderTarget()->getColorAttachment());
			}*/
		}

		auto driverShader = DriverShader::getOrAdd(material);
		auto driverGeometry = DriverGeometry::getOrAdd(geometry);
		auto driverMaterial = DriverMaterial::getOrAdd(material);

		//着色器预处理
		shaderPreprocess(driverShader, material);

		driverShader->use();
		driverGeometry->use();
		driverMaterial->use();

		auto modelMatrix = renderableObject->getModelMatrix();
		auto viewMatrix = camera->getModelMatrixInverse();
		auto projectionMatrix = camera->getProjectionMatrix();

		driverShader->setMat4("model", modelMatrix);
		driverShader->setMat4("view", viewMatrix);
		driverShader->setMat4("projection", projectionMatrix);

		//着色器资源绑定
		shaderResourcesBind(driverShader, material, camera);

		if(geometry->getIndex())
		{
			glDrawElements(GL_TRIANGLES, geometry->getIndex()->getCount(), toGL(geometry->getIndex()->getDataType()), 0);
		}
		else 
		{
			glDrawArrays(GL_TRIANGLES, 0, geometry->getAttribute(AttributeType::Position)->getCount());
		}
			
		driverGeometry->nonUse();
		driverShader->nonUse();
		driverMaterial->nonUse();
	}

	void Renderer::shaderPreprocess(DriverShader::Ptr driverShader, Material::Ptr material)
	{
		if (material->getMaterialType() == MaterialType::PhongLightingMaterial)
		{
			//片段着色器不可创建长度为0的数组
			auto dirLightsNr = std::max(m_driverLightList->getDriverDirLights().size(), (size_t)1);
			driverShader->replace("{NR_DIR_LIGHTS}", std::to_string(dirLightsNr));
		}
	}

	void Renderer::shaderResourcesBind(DriverShader::Ptr driverShader, Material::Ptr material, Camera::Ptr camera)
	{
		switch (material->getMaterialType())
		{
		case MaterialType::BaseMaterial:
		{
			auto baseMaterial = std::static_pointer_cast<BaseMaterial>(material);
			if (baseMaterial->getTexture())
			{
				int textureUnits = DriverTexture::getTextureUnits(baseMaterial->getTexture()->getId());
				driverShader->setInt("texture1", textureUnits);
			}
			break;
		}
		case MaterialType::CubeMaterial:
		{
			auto cubeMaterial = std::static_pointer_cast<CubeMaterial>(material);
			int textureUnits = DriverTexture::getTextureUnits(cubeMaterial->getTexture()->getId());
			driverShader->setInt("skybox", textureUnits);
			break;
		}
		case MaterialType::PhongLightingMaterial:
		{
			auto phongLightingMaterial = std::static_pointer_cast<PhongLightingMaterial>(material);
			GLSL::Material glslMaterial;
			glslMaterial.diffuse = DriverTexture::getTextureUnits(phongLightingMaterial->getDiffuse()->getId());
			glslMaterial.specular = DriverTexture::getTextureUnits(phongLightingMaterial->getSpecular()->getId());
			glslMaterial.shininess = phongLightingMaterial->getShininess();
			driverShader->setGLSLMaterial("material", glslMaterial);

			auto driverDirLights = m_driverLightList->getDriverDirLights();
			std::vector<GLSL::DirLight> glslDirLightList;
			for (auto& driverDirLight : driverDirLights)
			{
				auto dirLightPtr = driverDirLight.light.lock();
				if (!dirLightPtr)
					continue;

				auto lightCamera = dirLightPtr->getLightShadow()->getCamera();
				auto depthAttachment = driverDirLight.driverShadow->getDriverRenderTarget()->getDriverDepthAttachment();
				depthAttachment->use();

				GLSL::DirLight glslDirLights;
				glslDirLights.direction = dirLightPtr->getWorldPosition();
				glslDirLights.ambient = dirLightPtr->getAmbient();
				glslDirLights.diffuse = dirLightPtr->getDiffuse();
				glslDirLights.specular = dirLightPtr->getSpecular();
				glslDirLights.castShadow = dirLightPtr->getCastShadow();
				glslDirLights.shadowMap = DriverTexture::getTextureUnits(dirLightPtr->getLightShadow()->getRenderTarget()->getDepthAttachment()->getId());
				glslDirLights.lightSpaceMatrix = lightCamera->getProjectionMatrix() * lightCamera->getModelMatrixInverse();
				glslDirLightList.push_back(std::move(glslDirLights));
			}
			driverShader->setGLSLDirectionalLightsArray("dirLight", glslDirLightList);

			driverShader->setVec3("viewPos", camera->getWorldPosition());
			break;
		}
		default:
			break;
		}
	}
}
