#version 330 core

layout(location = 0) in vec3 aPos;             // ����λ��
layout(location = 2) in vec3 aNormal;          // ���㷨��
layout(location = 3) in vec2 aTexCoords;       // ��������
layout(location = 4) in vec4 boneIds;          // ��������
layout(location = 5) in vec4 weights;          // Ȩ��

out vec3 FragPos;                              // Ƭ��λ��
out vec3 Normal;                               // Ƭ�η���
out vec2 TexCoords;                            // Ƭ����������

uniform mat4 model;                            // ģ�;���
uniform mat4 view;                             // ��ͼ����
uniform mat4 projection;                        // ͶӰ����
uniform bool animationStart;                   // ������ʼ��־

const int MAX_BONES = 100;                     // ����������
const int MAX_BONE_INFLUENCE = 4;              // ������Ӱ������
uniform mat4 finalBonesMatrices[MAX_BONES];    // �����任����

void main()
{
    vec4 totalPosition = vec4(0.0f);

    // ����任���λ��
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

    // �������յĲü��ռ�λ��
    gl_Position = projection * view * model * totalPosition;

    // ����Ƭ��λ��
    FragPos = vec3(model * totalPosition);

    // ���㷨��
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
                transformedNormal = aNormal; // �����б任
                break;
            }

            // ʹ�ù����任����任����
            vec3 localNormal = mat3(finalBonesMatrices[id]) * aNormal;
            transformedNormal += localNormal * weights[i];
        }
    }
    else
    {
        transformedNormal = aNormal;
    }

    // �任���ߵ�����ռ�
    Normal = normalize(mat3(transpose(inverse(model))) * transformedNormal);

    // ������������
    TexCoords = aTexCoords;
}