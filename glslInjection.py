import os

# 定义文件路径
MeshBasicMaterialVSCode_file_path = 'engine/render/driver/glsl/vs/meshBasicMaterialVS.glsl'
MeshBasicMaterialFSCode_file_path = 'engine/render/driver/glsl/fs/meshBasicMaterialFS.glsl'

CubeMaterialVSCode_file_path = 'engine/render/driver/glsl/vs/cubeMaterialVS.glsl'
CubeMaterialFSCode_file_path = 'engine/render/driver/glsl/fs/cubeMaterialFS.glsl'

PhongLightingMaterialVSCode_file_path = 'engine/render/driver/glsl/vs/phongLightingMaterialVS.glsl'
PhongLightingMaterialFSCode_file_path = 'engine/render/driver/glsl/fs/phongLightingMaterialFS.glsl'

DepthMaterialVSCode_file_path = 'engine/render/driver/glsl/vs/depthMaterialVS.glsl'
DepthMaterialFSCode_file_path = 'engine/render/driver/glsl/fs/depthMaterialFS.glsl'

cpp_file_path = 'engine/render/driver/driverGLSL.cpp'

# CPP模板字符串
cpp_template = '''
#include "driverGLSL.h"

namespace JB
{
    std::string MeshBasicMaterialVSCode = 
    R"(
{MeshBasicMaterialVSCode}
    )";
    
    std::string MeshBasicMaterialFSCode = 
    R"(
{MeshBasicMaterialFSCode}
    )";
    
    std::string CubeMaterialVSCode = 
    R"(
{CubeMaterialVSCode}
    )";
    
    std::string CubeMaterialFSCode = 
    R"(
{CubeMaterialFSCode}
    )";

    std::string PhongLightingMaterialVSCode = 
    R"(
{PhongLightingMaterialVSCode}
    )";
    
    std::string PhongLightingMaterialFSCode = 
    R"(
{PhongLightingMaterialFSCode}
    )";
    
    std::string DepthMaterialVSCode = 
    R"(
{DepthMaterialVSCode}
    )";
    
    std::string DepthMaterialFSCode = 
    R"(
{DepthMaterialFSCode}
    )";
}
'''

# 读取GLSL文件内容
def read_glsl_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

# 将结果写入CPP文件
def write_cpp_file(file_path, content):
    with open(file_path, 'w') as file:
        file.write(content)

# 主函数
def main():
    # 替换占位符
    cpp_content = cpp_template

    cpp_content = cpp_content.replace('{MeshBasicMaterialVSCode}', read_glsl_file(MeshBasicMaterialVSCode_file_path))
    cpp_content = cpp_content.replace('{MeshBasicMaterialFSCode}', read_glsl_file(MeshBasicMaterialFSCode_file_path))

    cpp_content = cpp_content.replace('{CubeMaterialVSCode}', read_glsl_file(CubeMaterialVSCode_file_path))
    cpp_content = cpp_content.replace('{CubeMaterialFSCode}', read_glsl_file(CubeMaterialFSCode_file_path))

    cpp_content = cpp_content.replace('{PhongLightingMaterialVSCode}', read_glsl_file(PhongLightingMaterialVSCode_file_path))
    cpp_content = cpp_content.replace('{PhongLightingMaterialFSCode}', read_glsl_file(PhongLightingMaterialFSCode_file_path))

    cpp_content = cpp_content.replace('{DepthMaterialVSCode}', read_glsl_file(DepthMaterialVSCode_file_path))
    cpp_content = cpp_content.replace('{DepthMaterialFSCode}', read_glsl_file(DepthMaterialFSCode_file_path))

    # 写入更新后的内容到CPP文件
    write_cpp_file(cpp_file_path, cpp_content)

    print("GLSL注入成功。")

if __name__ == '__main__':
    main()
