cmake_minimum_required (VERSION 2.8.11)
project (Pic)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions")

find_package(OpenGL REQUIRED)
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${OPENGL_INCLUDE_DIRS} /usr/local/include)
link_directories(/usr/local/lib)

set (PIGMENTDEPS Stick ${OPENGL_LIBRARIES})

set (PIGMENTINC 
Pigment/BufferLayout.hpp
Pigment/CommandBuffer.hpp
Pigment/Constants.hpp
Pigment/ErrorCodes.hpp
Pigment/Handles.hpp
Pigment/Renderer.hpp
Pigment/Results.hpp
Pigment/RenderStateSettings.hpp
Pigment/Settings.hpp
Pigment/VertexLayout.hpp
Pigment/OpenGL/GLPrerequisites.hpp
Pigment/OpenGL/GLRendererImpl.hpp
Pigment/OpenGL/GLCommandBufferImpl.hpp
)

set (PIGMENTSRC
Pigment/BufferLayout.cpp
Pigment/CommandBuffer.cpp
Pigment/ErrorCodes.cpp
Pigment/Renderer.cpp
Pigment/RenderStateSettings.cpp
Pigment/Settings.cpp
Pigment/VertexLayout.cpp
Pigment/OpenGL/GLRendererImpl.cpp
Pigment/OpenGL/GLCommandBufferImpl.cpp
)

add_library (Pigment SHARED ${PIGMENTSRC})
add_library (PigmentStatic STATIC ${PIGMENTSRC})
target_link_libraries(Pigment ${PIGMENTDEPS})
target_link_libraries(PigmentStatic ${PIGMENTDEPS})
install (TARGETS Pigment PigmentStatic DESTINATION /usr/local/lib)
install (FILES ${PIGMENTINC} DESTINATION /usr/local/include/Pigment)

add_subdirectory (Examples)
