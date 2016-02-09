#ifndef PIGMENT_OPENGL_GLPREREQUISITES_HPP
#define PIGMENT_OPENGL_GLPREREQUISITES_HPP

#include <Stick/Platform.hpp>

#if STICK_PLATFORM == STICK_PLATFORM_OSX
#include <OpenGL/OpenGL.h> //for CGL functions
//check if opengl 3+ is available
#if CGL_VERSION_1_3
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif //CGL_VERSION_1_3
#elif STICK_PLATFORM == STICK_PLATFORM_LINUX
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#undef GL_GLEXT_PROTOTYPES
#endif

#endif //PIGMENT_OPENGL_GLPREREQUISITES_HPP
