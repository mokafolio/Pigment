deps = [dependency('glfw3')]

#we manually need to link glfw3 dependencies...hmm :/
if host_machine.system() == 'darwin'
    deps += [dependency('Cocoa'), dependency('IOKit'), dependency('CoreFoundation'), dependency('CoreVideo')]
elif host_machine.system() == 'linux'
    cc = meson.get_compiler('c')
    deps += [dependency('x11'), 
            cc.find_library('dl', required : true),
            cc.find_library('m', required : true),
            dependency('threads')]
endif

helloTriangleExample = executable('HelloTriangleExample', 'HelloTriangleExample.cpp', 
    dependencies: [pigmentDep] + deps, 
    include_directories : incDirs)

renderBufferExample = executable('RenderBufferExample', 'RenderBufferExample.cpp', 
    dependencies: [pigmentDep] + deps, 
    include_directories : incDirs)

#@todo add tile saver example (depends on Pic)