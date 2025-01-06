#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aTexCoords;
layout(location = 4) in vec4 boneIds;
layout(location = 5) in vec4 weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

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

    FragPos = vec3(model * totalPosition);

    Normal = mat3(transpose(inverse(model))) * aNormal;

    TexCoords = aTexCoords;
}
