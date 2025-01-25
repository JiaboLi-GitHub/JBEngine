
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

layout(location = 0) in vec3 aPos;             // 顶点位置
layout(location = 2) in vec3 aNormal;          // 顶点法线
layout(location = 3) in vec2 aTexCoords;       // 纹理坐标
layout(location = 4) in vec4 boneIds;          // 骨骼索引
layout(location = 5) in vec4 weights;          // 权重

out vec3 FragPos;                              // 片段位置
out vec3 Normal;                               // 片段法线
out vec2 TexCoords;                            // 片段纹理坐标

uniform mat4 model;                            // 模型矩阵
uniform mat4 view;                             // 视图矩阵
uniform mat4 projection;                        // 投影矩阵
uniform bool animationStart;                   // 动画开始标志

const int MAX_BONES = 100;                     // 最大骨骼数量
const int MAX_BONE_INFLUENCE = 4;              // 最大骨骼影响数量
uniform mat4 finalBonesMatrices[MAX_BONES];    // 骨骼变换矩阵

void main()
{
    vec4 totalPosition = vec4(0.0f);

    // 计算变换后的位置
    if (animationStart)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            int id = int(boneIds[i]);

            if (id == -1)
                continue;

            if (id >= MAX_BONES)
            {
                totalPosition = vec4(aPos, 1.0f);
                break;
            }

            vec4 localPosition = finalBonesMatrices[id] * vec4(aPos, 1.0f);
            totalPosition += localPosition * weights[i];
        }
    }
    else
    {
        totalPosition = vec4(aPos, 1.0f);
    }

    // 计算最终的裁剪空间位置
    gl_Position = projection * view * model * totalPosition;

    // 计算片段位置
    FragPos = vec3(model * totalPosition);

    // 计算法线
    vec3 transformedNormal = vec3(0.0f);
    if (animationStart)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            int id = int(boneIds[i]);

            if (id == -1)
                continue;

            if (id >= MAX_BONES)
            {
                transformedNormal = aNormal; // 不进行变换
                break;
            }

            // 使用骨骼变换矩阵变换法线
            vec3 localNormal = mat3(finalBonesMatrices[id]) * aNormal;
            transformedNormal += localNormal * weights[i];
        }
    }
    else
    {
        transformedNormal = aNormal;
    }

    // 变换法线到世界空间
    Normal = normalize(mat3(transpose(inverse(model))) * transformedNormal);

    // 传递纹理坐标
    TexCoords = aTexCoords;
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

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

    )";
    
    std::string DepthMaterialVSCode = 
    R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 4) in vec4 boneIds;
layout(location = 5) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool animationStart;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0f);

    if (animationStart)
    {
        for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        {
            int id = int(boneIds[i]);

            if (id == -1)
                continue;

            if (id >= MAX_BONES)
            {
                totalPosition = vec4(aPos, 1.0f);
                break;
            }

            vec4 localPosition = finalBonesMatrices[id] * vec4(aPos, 1.0f);
            totalPosition += localPosition * weights[i];
        }
    }
    else
    {
        totalPosition = vec4(aPos, 1.0f);
    }

    gl_Position = projection * view * model * totalPosition;
}

    )";
    
    std::string DepthMaterialFSCode = 
    R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}

    )";
}
