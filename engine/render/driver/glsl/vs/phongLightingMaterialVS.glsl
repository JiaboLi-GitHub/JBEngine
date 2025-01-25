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