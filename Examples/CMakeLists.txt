find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
set (EXAMPLESDEPS Stick Pic freeimage ${GLFW_STATIC_LIBRARIES})

add_executable (HelloTriangleExample HelloTriangleExample.cpp)
target_link_libraries(HelloTriangleExample Pigment ${PIGMENTDEPS} ${EXAMPLESDEPS})
add_executable (RenderBufferExample RenderBufferExample.cpp)
target_link_libraries(RenderBufferExample Pigment ${PIGMENTDEPS} ${EXAMPLESDEPS})
add_executable (TileSaverExample TileSaverExample.cpp)
target_link_libraries(TileSaverExample Pigment ${PIGMENTDEPS} ${EXAMPLESDEPS})
