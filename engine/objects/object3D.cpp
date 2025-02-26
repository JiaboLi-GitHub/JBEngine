﻿#include "object3D.h"
#include "../tools/idAllocator.h"

namespace JB
{
	Object3D::Object3D()
	{
		m_id = IDAllocator::getInstance().allocateID();
		m_type = Object3DType::Object3D;
		m_position = glm::vec3(0.0f);
		m_localMatrix = glm::mat4(1.0f);
		m_modelMatrix = glm::mat4(1.0f);
	}

	void Object3D::decompose()
	{
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(m_localMatrix, m_scale, m_quaternion, m_position, skew, perspective);
	}

	Object3D::Ptr Object3D::create()
	{
		return std::shared_ptr<Object3D>(new Object3D());
	}

	Object3D::~Object3D()
	{
		IDAllocator::getInstance().releaseID(m_id);
	}

	size_t Object3D::getId()
	{
		return m_id;
	}

	Object3DType Object3D::getType()
	{
		return m_type;
	}

	glm::mat4x4 Object3D::getLocalMatrix()
	{
		return m_localMatrix;
	}

	glm::mat4x4 Object3D::getModelMatrix()
	{
		return m_modelMatrix;
	}

	glm::vec3 Object3D::getPosition()
	{
		return m_position;
	}

	void Object3D::addChild(Object3D::Ptr child)
	{
		Object3D::Ptr thisPtr = shared_from_this();

		if (thisPtr == child)
			return;

		child->m_parent = thisPtr;
		if (std::find(m_children.begin(), m_children.end(), child) == m_children.end())
			m_children.push_back(child);
	}

	std::vector<Object3D::Ptr>& Object3D::getChildren()
	{
		return m_children;
	}

	void Object3D::updateModelMatrix(bool updateParent, bool updateChildren)
	{
		auto parent = m_parent.lock();
		if (parent && updateParent)
			parent->updateModelMatrix(true, false);

		updateMatrix();

		if (parent)
			m_modelMatrix = parent->m_modelMatrix * m_localMatrix;
		else
			m_modelMatrix = m_localMatrix;

		if (updateChildren)
		{
			for (auto& child : m_children)
			{
				child->updateModelMatrix(false, true);
			}
		}
	}

	void Object3D::updateMatrix()
	{
		auto translateMatrix = glm::translate(glm::mat4(1.0f), m_position);
		auto rotateMatrix = glm::mat4_cast(m_quaternion);
		auto scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

		m_localMatrix = translateMatrix * rotateMatrix * scaleMatrix;
	}

	void Object3D::setPosition(float x, float y, float z)
	{
		setPosition(glm::vec3(x, y, z));
	}

	void Object3D::setPosition(const glm::vec3& position)
	{
		m_localMatrix[3].x = position.x;
		m_localMatrix[3].y = position.y;
		m_localMatrix[3].z = position.z;

		m_position = position;

		decompose();
	}

	void Object3D::lookAt(const glm::vec3& target, const glm::vec3& up)
	{
		float scaleX = glm::length(glm::vec3(m_localMatrix[0]));
		float scaleY = glm::length(glm::vec3(m_localMatrix[1]));
		float scaleZ = glm::length(glm::vec3(m_localMatrix[2]));

		glm::vec3 position = glm::vec3(m_localMatrix[3]);

		auto nTarget = glm::normalize(target - position) * scaleZ;
		auto nRight = glm::normalize(glm::cross(up, -nTarget)) * scaleX;
		auto nUp = glm::normalize(glm::cross(nRight, nTarget)) * scaleY;


		m_localMatrix[0] = glm::vec4(nRight, 0.0f);
		m_localMatrix[1] = glm::vec4(nUp, 0.0f);
		m_localMatrix[2] = glm::vec4(-nTarget, 0.0f);
		m_localMatrix[3] = glm::vec4(position, 1.0f);

		decompose();
	}

	void Object3D::setLocalMatrix(const glm::mat4x4& matrix)
	{
		m_localMatrix = matrix;
	}

	glm::vec3 Object3D::getFront()
	{
		return glm::normalize(-glm::vec3(m_localMatrix[2]));
	}
	
	glm::vec3 Object3D::getUp()
	{
		return glm::normalize(glm::vec3(m_localMatrix[1]));
	}

	glm::vec3 Object3D::getRight()
	{
		return glm::normalize(glm::vec3(m_localMatrix[0]));
	}

	glm::vec3 Object3D::getWorldPosition()
	{
		return glm::vec3(m_modelMatrix[3]);
	}

	void Object3D::rotateAroundAxis(const glm::vec3& axis, float angle)
	{
		m_localMatrix = glm::rotate(m_localMatrix, glm::radians(angle), axis);

		decompose();
	}

	void Object3D::setScale(float x, float y, float z)
	{
		//拿到某一列，normalize去掉之前的scale影响,再乘以当前的相关scale
		auto col0 = glm::normalize(glm::vec3(m_localMatrix[0])) * x;
		auto col1 = glm::normalize(glm::vec3(m_localMatrix[1])) * y;
		auto col2 = glm::normalize(glm::vec3(m_localMatrix[2])) * z;

		//将设置好的前三列，重新设置到localmatrix
		m_localMatrix[0] = glm::vec4(col0, 0.0f);
		m_localMatrix[1] = glm::vec4(col1, 0.0f);
		m_localMatrix[2] = glm::vec4(col2, 0.0f);

		decompose();
	}

	void Object3D::setScale(float scale)
	{
		setScale(scale, scale, scale);
	}

	void Object3D::setQuaternion(float x, float y, float z, float w)
	{
		//在四元数情况下，glm的初始化，w xyz
		glm::quat quaternion(w, x, y, z);

		//考虑到，localMatrix可能已经被施加了scale方面的变换
		float scaleX = glm::length(glm::vec3(m_localMatrix[0]));
		float scaleY = glm::length(glm::vec3(m_localMatrix[1]));
		float scaleZ = glm::length(glm::vec3(m_localMatrix[2]));

		//将glm的四元数转换为一个旋转矩阵
		glm::mat4 rotateMatrix = glm::mat4_cast(quaternion);

		//将scale变换恢复进去
		m_localMatrix[0] = rotateMatrix[0] * scaleX;
		m_localMatrix[1] = rotateMatrix[1] * scaleY;
		m_localMatrix[2] = rotateMatrix[2] * scaleX;

		decompose();
	}
}
