#pragma once
#include <memory>
#include "renderTarget.h"
#include "driver/driverWindow.h"
#include "driver/driverRenderList.h"
#include "driver/driverGeometry.h"
#include "driver/driverShader.h"
#include "driver/driverTexture.h"
#include "driver/driverState.h"
#include "driver/driverMaterial.h"
#include "driver/driverRenderTarget.h"
#include "driver/driverLightList.h"
#include "driver/driverInfo.h"
#include "../objects/renderableObject.h"
#include "../objects/object3D.h"
#include "../material/baseMaterial.h"

namespace JB
{
	class Renderer : public std::enable_shared_from_this<Renderer>
	{
	public:
		using Ptr = std::shared_ptr<Renderer>;

		struct Descriptor {
			int width = 800;
			int height = 600;
		};

		static Renderer::Ptr create(const Descriptor& descriptor);

		~Renderer();

		void init();

		bool render(Scene::Ptr scene, Camera::Ptr camera);

		//渲染场景
		void renderScene(DriverRenderList::Ptr driverRenderList, Scene::Ptr scene, Camera::Ptr camera);

		//渲染可渲染列表
		void renderRenderItemList(std::vector<RenderItem>& renderItems, Scene::Ptr scene, Camera::Ptr camera);

		//渲染可渲染物体
		void renderRenderItem(RenderItem& renderItems, Scene::Ptr scene, Camera::Ptr camera);

		void swap();

		void setSize(int width, int height);

		int getWidth();

		int getHeight();

		void setFrameSizeCallBack(const DriverWindow::FrameSizeCallback& callback);

		void setMouseMoveCallBack(const DriverWindow::MouseMoveCallback& callback);

		void setMouseActionCallback(const DriverWindow::MouseActionCallback& callback);

		void setKeyboardActionCallBack(const DriverWindow::KeyboardActionCallback& callback);

		void setClearColor(float r, float g, float b, float a);

		void setRenderTarget(const RenderTarget::Ptr& renderTarget);

	protected:
		Renderer(const Descriptor& descriptor);

	private:
		//构建渲染列表
		void buildRenderList(Object3D::Ptr object, Camera::Ptr camera);

		//构建天空盒
		void buildRenderSkyBox(Scene::Ptr scene, Camera::Ptr camera);

		//着色器预处理
		void shaderPreprocess(DriverShader::Ptr driverShader, Material::Ptr material);

		//着色器资源绑定
		void shaderResourcesBind(DriverShader::Ptr driverShader, Material::Ptr material, Camera::Ptr camera);

		void shaderResourcesBind_byObject(DriverShader::Ptr driverShader, RenderableObject::Ptr renderableObject);

	private:
		DriverWindow::Ptr m_driverWindow;
		DriverRenderList::Ptr m_driverRenderList;
		DriverState::Ptr m_driverState;
		DriverRenderTarget::Ptr m_driverRenderTarget;
		DriverLightList::Ptr m_driverLightList;
		DriverInfo::Ptr m_driverInfo;
		int m_width;
		int m_height;
	};
}
