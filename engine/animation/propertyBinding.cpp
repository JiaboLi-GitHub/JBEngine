#include "propertyBinding.h"
#include <iostream>
#include "../model/bone.h"

namespace JB
{
    PropertyBinding::PropertyBinding(const Object3D::Ptr& root, const std::string& nodeName, TrackType trackType, const size_t& valueSize)
    {
        m_parseNames = { nodeName ,trackType };

        m_node = findeNode(root, m_parseNames.nodeName);

        m_buffer = new unsigned char[valueSize * sizeof(float)];
    }

    PropertyBinding::~PropertyBinding()
    {
        delete[] m_buffer;
    }
    
    void PropertyBinding::apply()
    {
        switch (m_parseNames.trackType)
        {
        case TrackType::Move:
        {
            float* buffer = static_cast<float*>(m_buffer);
            m_node->setPosition(buffer[0], buffer[1], buffer[2]);
            break;
        }
        case TrackType::Rotate:
        {
            float* buffer = static_cast<float*>(m_buffer);
            m_node->setQuaternion(buffer[0], buffer[1], buffer[2], buffer[3]);
            break;
        }
        case TrackType::Scale:
        {
            float* buffer = static_cast<float*>(m_buffer);
            m_node->setScale(buffer[0], buffer[1], buffer[2]);
            break;
        }
        default:
            break;
        }
    }

    void* PropertyBinding::getBuffer()
    {
        return m_buffer;;
    }
    
    Object3D::Ptr PropertyBinding::findeNode(const Object3D::Ptr& object, const std::string& nodeName)
    {
        auto rootBone = std::dynamic_pointer_cast<Bone>(object);
        if (rootBone)
        {
            if (rootBone->getName() == nodeName)
                return object;
        }

        auto children = object->getChildren();
        for (uint32_t i = 0; i < children.size(); ++i) 
        {
            auto child = children[i];
            auto bone = findeNode(child, nodeName);

            if (bone) 
            {
                return bone;
            }
        }

        return nullptr;
    }
}
