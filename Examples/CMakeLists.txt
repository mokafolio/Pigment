find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
set (EXAMPLESDEPS Stick ${GLFW_LIBRARIES})

add_executable (HelloTriangleExample HelloTriangleExample.cpp)
target_link_libraries(HelloTriangleExample ${GLFW_STATIC_LIBRARIES} ${PIGMENTDEPS} Pigment)
add_executable (RenderBufferExample RenderBufferExample.cpp)
target_link_libraries(RenderBufferExample ${GLFW_STATIC_LIBRARIES} ${PIGMENTDEPS} Pigment)
