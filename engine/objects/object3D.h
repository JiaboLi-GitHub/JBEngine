﻿#pragma once
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include "../global/constant.h"

namespace JB
{
	class Renderer;
	class Scene;
	class Camera;
	class Object3D : public std::enable_shared_from_this<Object3D>
	{
	public:
		using Ptr = std::shared_ptr<Object3D>;

		static Object3D::Ptr create();

		~Object3D();

		size_t getId();

		Object3DType getType();

		glm::mat4x4 getLocalMatrix();

		glm::mat4x4 getModelMatrix();

		glm::vec3 getPosition();

		void addChild(Object3D::Ptr child);

		std::vector<Object3D::Ptr>& getChildren();

		virtual void updateModelMatrix(bool updateParent = false, bool updateChildren = false);

		virtual void updateMatrix();

		void setPosition(float x, float y, float z);

		void setPosition(const glm::vec3& position);

		void lookAt(const glm::vec3& target, const glm::vec3& up);

		void setLocalMatrix(const glm::mat4x4& matrix);

		glm::vec3 getFront();

		glm::vec3 getUp();

		glm::vec3 getRight();

		glm::vec3 getWorldPosition();

		void rotateAroundAxis(const glm::vec3& axis, float angle);

	protected:
		Object3D();

	protected:
		size_t m_id;
		Object3DType m_type;
		std::weak_ptr<Object3D> m_parent;
		std::vector<Object3D::Ptr> m_children;
		glm::vec3 m_position;
		glm::mat4x4 m_localMatrix;
		glm::mat4x4 m_modelMatrix;
	};
}