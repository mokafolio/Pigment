
// we use GLFW to open a simple window
#include <Crunch/TiledFrustum.hpp>
#include <GLFW/glfw3.h>
#include <Pic/Image.hpp>
#include <Pigment/CommandBuffer.hpp>
#include <Pigment/Renderer.hpp>
#include <stdlib.h>
//#include <Crunch/StringConversion.hpp>

using namespace pigment;
using namespace stick;
using namespace pic;
using namespace crunch;

class HighResFrameSaver
{
  public:
    using DrawTileCallback =
        std::function<void(const SubFrustumResult &, const FrustumTile & _tile)>;
};

int main(int _argc, const char * _args[])
{
    // initialize glfw
    if (!glfwInit())
        return EXIT_FAILURE;

    // and set some hints
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(800, 600, "Hello Triangle Example", NULL, NULL);
    if (window)
    {
        glfwMakeContextCurrent(window);

        // the pigment renderer instance
        Renderer renderer;

        // create a command buffer to submit rendering commands to the GPU
        CommandBuffer & cbuffer = renderer.createCommandBuffer().ensure();

        // a simple vertex layout for our mesh consisting of a 2D vertex
        // and an RGBA color.
        VertexLayout layout;
        layout.addElement("vertex", 2).addElement("color", 4).finish();

        // create a vertex buffer with the layout
        VertexBufferHandle vb =
            renderer.createVertexBuffer(layout, BufferUsage::StaticDraw).ensure();

        // make a mesh with the vertex buffer
        MeshSettings ms;
        ms.vertexBuffers.append(vb);
        MeshHandle mesh = renderer.createMesh(ms).ensure();

        // load the vertex data for the triangle into the vertex buffer
        Float32 triangleData[18] = { -0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 0.0, -0.5, 0.0,
                                     1.0,  0.0, 1.0, 0.5, 0.5, 0.0, 0.0, 1.0,  1.0 };
        cbuffer.loadVertexData(vb, 18, triangleData);

        // a very simple vertex shader for the triangle
        String vertexShader =
            "#version 150 \n"
            "uniform mat4 transformProjection; \n"
            "in vec2 vertex; \n"
            "in vec4 color; \n"
            "out vec4 ocol; \n"
            "void main() \n"
            "{ \n"
            "   ocol = color; \n"
            "   mat4 tt = transformProjection;\n"
            "   gl_Position = transformProjection * vec4(vertex, 0.0, 1.0); \n"
            "} \n";

        // a simple fragment shader for the triangle
        String fragmentShader =
            "#version 150 \n"
            "in vec4 ocol; \n"
            "out vec4 fragColor; \n"
            "void main() \n"
            "{ \n"
            "   fragColor = ocol; \n"
            "} \n";

        // compile and link the shaders into a program
        ProgramSettings ps(layout, vertexShader, fragmentShader);
        ProgramHandle program = renderer.createProgram(ps).ensure();

        // create a very simple render state
        RenderStateSettings rss;
        rss.setMultisampling(true);
        RenderStateHandle renderState = renderer.createRenderState(rss).ensure();

        Mat4f transform = Mat4f::lookAt(Vec3f(0.0, 0.0, 1.5), Vec3f(0.0f), Vec3f(0, 1, 0));
        transform = Mat4f::identity();
        Mat4f proj = Mat4f::perspective(50.0f, 800.0 / 600.0, 0.01, 100.0);
        proj = Mat4f::ortho(-1.0, 1.0, -1.0, 1.0, -1, 1);
        // Mat4f proj = Mat4f::ortho(-1.0, 1.0, -1.0, 1.0, 0.1, 1000.0);
        // printf("transform %s\n", toString(transform).cString());
        // proj = Mat4f::identity();
        // transform = Mat4f::identity();
        Mat4f tp = proj * transform;
        Frustum frustum = Frustum::fromPerspective(50.0f, 800.0 / 600.0, 0.01, 100.0);
        frustum = Frustum::fromOrtho(-1.0, 1.0, -1.0, 1.0, -1, 1);
        auto spres = subFrustum(frustum, 1600.0f, 1200.0f, 3, 100.0f);
        printf("SUB PERSPECTIVES: %lu\n", spres.tiles.count());

        ImageRGB8 img(spres.pixelWidth, spres.pixelHeight);
        ImageRGB8 tmpImg(spres.tilePixelWidth, spres.tilePixelHeight);

        // the main loop
        while (!glfwWindowShouldClose(window))
        {
            // get the window size from the glfw window
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            for (auto & sp : spres.tiles)
            {
                // add command to clear the frame buffer to a dark gray
                cbuffer.clearBuffers(BufferType::Color, { { 0.15, 0.15, 0.15, 1.0 }, 1.0 });
                // set the viewport based on the window size
                cbuffer.setViewport(0, 0, width, height);
                Mat4f tp = sp.frustum * transform;
                cbuffer.setProgramVariableMatrix4f(program, "transformProjection", tp.ptr());
                // schedule a drawing command using the renderstate, program and mesh for the
                // triangle
                cbuffer.draw(renderState, program, mesh, VertexDrawMode::Triangles, 0, 3);
                // submit and clear the command buffer, this will actually do the GPU lifting.
                Error err = renderer.submitCommandBuffer(cbuffer);

                if (err)
                    return EXIT_FAILURE;

                cbuffer.clear();
                glReadPixels(spres.pixelBorderX,
                             spres.pixelBorderY,
                             spres.tilePixelWidth,
                             spres.tilePixelHeight,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             tmpImg.ptr());

                // swap the glfw windows buffer & poll the window events
                glfwSwapBuffers(window);
                glfwPollEvents();

                img.updatePixels(sp.x * tmpImg.width(),
                                 sp.y * tmpImg.height(),
                                 tmpImg.width(),
                                 tmpImg.height(),
                                 tmpImg.ptr());
            }

            img.saveFile("test.png");

            return EXIT_SUCCESS;
        }
    }
    else
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // clean up glfw
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
