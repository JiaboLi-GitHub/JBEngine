cmake_minimum_required(VERSION 3.12)
project(JBEngine_Project)

set(CMAKE_CXX_STANDARD 17)

file(GLOB copyResources "./assets" "./thirdParty/libs/assimp/assimp-vc143-mtd.dll")
file(COPY ${copyResources} DESTINATION ${CMAKE_BINARY_DIR})

include_directories(
SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/libs/glfw
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/libs/assimp
)

add_subdirectory(engine)

add_executable(triangle "examples/triangle/triangle.cpp")
add_executable(box "examples/box/box.cpp")
add_executable(framebuffers "examples/framebuffers/framebuffers.cpp")
add_executable(light "examples/light/light.cpp")
add_executable(shadow "examples/shadow/shadow.cpp")
add_executable(model "examples/model/model.cpp")
add_executable(animation "examples/animation/animation.cpp")

target_link_libraries(triangle JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
target_link_libraries(box JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
target_link_libraries(framebuffers JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
target_link_libraries(light JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
target_link_libraries(shadow JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
target_link_libraries(model JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
target_link_libraries(animation JBEngine_lib  glfw3.lib assimp-vc143-mtd.lib)
