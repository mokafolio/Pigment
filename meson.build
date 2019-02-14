project('Pigment', ['cpp', 'c'], default_options : ['cpp_std=c++14'])

if meson.is_subproject() == false
    # disable this bogus warning (should be deprecated in newer clang versions)
    add_global_arguments('-Wno-missing-braces', '-fno-exceptions', language: 'cpp')
else
    message('Building Pigment as subproject.')
endif

cppArgs = ['-DPUGIXML_NO_EXCEPTIONS']
if host_machine.system() == 'darwin'
    cppArgs += '-DGL_SILENCE_DEPRECATION'
endif

pigmentInc = [
    'Pigment/BufferLayout.hpp',
    'Pigment/CommandBuffer.hpp',
    'Pigment/Constants.hpp',
    'Pigment/ErrorCodes.hpp',
    'Pigment/Handles.hpp',
    'Pigment/Renderer.hpp',
    'Pigment/Results.hpp',
    'Pigment/RenderStateSettings.hpp',
    'Pigment/Settings.hpp',
    'Pigment/VertexLayout.hpp'
]
install_headers(pigmentInc, subdir: 'Pigment')

pigmentGLInc = [
    'Pigment/OpenGL/GLPrerequisites.hpp',
    'Pigment/OpenGL/GLRendererImpl.hpp',
    'Pigment/OpenGL/GLCommandBufferImpl.hpp'
]
install_headers(pigmentGLInc, subdir: 'Pigment/OpenGL')

pigmentSrc = [
    'Pigment/BufferLayout.cpp',
    'Pigment/CommandBuffer.cpp',
    'Pigment/ErrorCodes.cpp',
    'Pigment/Renderer.cpp',
    'Pigment/RenderStateSettings.cpp',
    'Pigment/Settings.cpp',
    'Pigment/VertexLayout.cpp',
    'Pigment/OpenGL/GLRendererImpl.cpp',
    'Pigment/OpenGL/GLCommandBufferImpl.cpp'
]

cc = meson.get_compiler('cpp')
if get_option('buildSubmodules') == false
    stickDep = cc.find_library('Stick')
    pigmentDeps = [stickDep, dependency('threads')]
else
    stickProj = subproject('Stick')
    pigmentDeps = [stickProj.get_variable('stickDep')]
endif

pigmentDeps += dependency('gl')

if host_machine.system() == 'linux'
    pigmentDeps += cc.find_library('dl', required : true)
endif

incDirs = include_directories('.')

pigment = library('Pigment', 
        pigmentSrc,
        dependencies: pigmentDeps, 
        include_directories : incDirs,
        cpp_args: cppArgs,
        install: true)

pigmentDep = declare_dependency(link_with : pigment, 
    dependencies: pigmentDeps, 
    include_directories: incDirs)

# only build examples if not build as a subproject.
if meson.is_subproject() == false
    subdir('Examples')
endif
