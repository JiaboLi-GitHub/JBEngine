#include "light.h"

namespace JB
{
    Light::Light()
    {
        m_type = Object3DType::Lights;
    }

    Light::~Light()
    {
    }

    void Light::setAmbient(const glm::vec3& color) 
    {
        m_ambient = color;
    }

    void Light::setDiffuse(const glm::vec3& color) 
    {
        m_diffuse = color;
    }

    void Light::setSpecular(const glm::vec3& color) 
    {
        m_specular = color;
    }

    void Light::setCastShadow(bool castShadow)
    {
        m_castShadow = castShadow;
    }

    LightsType Light::getLightsType() const
    {
        return m_lightsType;
    }

    glm::vec3 Light::getAmbient() const 
    {
        return m_ambient;
    }

    glm::vec3 Light::getDiffuse() const 
    {
        return m_diffuse;
    }

    glm::vec3 Light::getSpecular() const 
    {
        return m_specular;
    }

    bool Light::getCastShadow()
    {
        return m_castShadow;
    }

    LightShadow::Ptr Light::getLightShadow()
    {
        return m_shadow;
    }
}
