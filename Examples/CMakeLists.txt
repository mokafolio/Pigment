find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})

set (EXAMPLESDEPS Stick ${GLFW_LIBRARIES})

add_executable (HelloTriangleExample HelloTriangleExample.cpp)
target_link_libraries(HelloTriangleExample ${GLFW_LIBRARIES} ${PIGMENTDEPS} Pigment Xrandr X11 m Xrandr Xi X11 Xinerama Xcursor Xxf86vm pthread dl)