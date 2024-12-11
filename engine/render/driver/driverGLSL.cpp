
#include "driverGLSL.h"

namespace JB
{
    std::string MeshBasicMaterialVSCode = 
    R"(

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 3) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    ourColor = aColor;
    TexCoord = aTexCoord;
}
    )";
    
    std::string MeshBasicMaterialFSCode = 
    R"(

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;

void main()
{
#ifdef USE_TEXTURE
	FragColor = texture(texture1, TexCoord);
#else
	FragColor = vec4(ourColor, 1.0f);
#endif
}
    )";
    
    std::string CubeMaterialVSCode = 
    R"(
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 ourColor;
out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * model * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
    )";
    
    std::string CubeMaterialFSCode = 
    R"(
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}
    )";

    std::string PhongLightingMaterialVSCode = 
    R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}
    )";
    
    std::string PhongLightingMaterialFSCode = 
    R"(
#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool castShadow;
    sampler2D shadowMap;

    mat4 lightSpaceMatrix;
};

#define NR_DIR_LIGHTS {NR_DIR_LIGHTS}

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight[NR_DIR_LIGHTS];
uniform Material material;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
float shadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0);

    for (int i = 0; i < NR_DIR_LIGHTS; i++)
        result += calcDirLight(dirLight[i], norm, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    float shadow = light.castShadow ? shadowCalculation(light.shadowMap, light.lightSpaceMatrix * vec4(FragPos, 1.0), normal, lightDir) : 0.0;
    return ambient + (diffuse + specular) * (1.0 - shadow);
}

float shadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords.xyz * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
    )";
    
    std::string DepthMaterialVSCode = 
    R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
    )";
    
    std::string DepthMaterialFSCode = 
    R"(
#version 330 core
out vec4 FragColor;



 float near_plane = 0.1f;
 float far_plane = 10.f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main()
{
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
    )";
}
