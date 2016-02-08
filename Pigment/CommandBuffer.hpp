#ifndef PIGMENT_COMMANDBUFFER_HPP
#define PIGMENT_COMMANDBUFFER_HPP

#include <Stick/DynamicArray.hpp>
#include <Stick/String.hpp>
#include <Stick/UniquePtr.hpp>
#include <Pigment/Constants.hpp>
#include <Pigment/Handles.hpp>

namespace pigment
{
    struct STICK_API Color
    {
        stick::Float32 r, g, b, a;
    };

    union STICK_API ProgramVariableValue
    {
        struct Vec2f
        {
            stick::Float32 x, y;
        };

        struct Vec3f
        {
            stick::Float32 x, y, z;
        };

        struct Vec4f
        {
            stick::Float32 x, y, z, w;
        };

        struct Mat3f
        {
            stick::Float32 v[9];
        };

        struct Mat4f
        {
            stick::Float32 v[16];
        };

        stick::Float32 floatValue;
        Vec2f vec2fValue;
        Vec3f vec3fValue;
        Vec4f vec4fValue;
        Mat3f mat3fValue;
        Mat4f mat4fValue;
    };

    struct STICK_API ClearBuffersSettings
    {
        ClearBuffersSettings(const Color & _clearColor = {1.0, 1.0, 1.0, 1.0},
                             stick::Float32 _clearDepth = 1.0f,
                             stick::UInt32 _stencilWrite = 0,
                             stick::Int32 _clearStencil = 0) :
            clearColor(_clearColor),
            clearDepth(_clearDepth),
            stencilBackFaceWriteMask(_stencilWrite),
            stencilFrontFaceWriteMask(_stencilWrite),
            clearStencil(_clearStencil)
        {

        }

        Color clearColor;
        stick::Float32 clearDepth;
        stick::UInt32 stencilFrontFaceWriteMask;
        stick::UInt32 stencilBackFaceWriteMask;
        stick::Int32 clearStencil;
    };

    namespace detail
    {
        class RendererImpl;
        class CommandBufferImpl;
        typedef stick::UniquePtr<CommandBufferImpl> CommandBufferImplPtr;
    }

    class STICK_API CommandBuffer
    {
        friend class detail::RendererImpl;

    public:

        CommandBuffer();

        ~CommandBuffer();

        void loadPixelsRaw(const TextureHandle & _handle, stick::Size _width, stick::Size _height, stick::Size _depth,
                           void * _pixels, PixelDataType _dataType, TextureFormat _format, stick::UInt32 _mipmapLevelCount = 0);

        void allocatePixels(const TextureHandle & _handle, stick::Size _width, stick::Size _height, stick::Size _depth, TextureFormat _format, stick::UInt32 _mipmapLevelCount = 0);

        void allocatePixels(const TextureHandle & _handle, stick::Size _width, stick::Size _height, TextureFormat _format, stick::UInt32 _mipmapLevelCount = 0);

        void allocatePixels(const TextureHandle & _handle, stick::Size _width, TextureFormat _format, stick::UInt32 _mipmapLevelCount = 0);

        void updatePixelsRaw(const TextureHandle & _handle, stick::Size _x, stick::Size _y, stick::Size _z,
                             stick::Size _width, stick::Size _height, stick::Size _depth, void * _pixels, PixelDataType _dataType);

        void setProgramVariable(const ProgramHandle & _program, const stick::String & _name, const ProgramVariableValue & _val);

        void setProgramVariablef(const ProgramHandle & _program, const stick::String & _name, stick::Float32 _a);

        void setProgramVariablef(const ProgramHandle & _program, const stick::String & _name, stick::Float32 _a, stick::Float32 _b);

        void setProgramVariablef(const ProgramHandle & _program, const stick::String & _name, stick::Float32 _a, stick::Float32 _b, stick::Float32 _c);

        void setProgramVariablef(const ProgramHandle & _program, const stick::String & _name, stick::Float32 _a, stick::Float32 _b, stick::Float32 _c, stick::Float32 _d);

        void setProgramVariableMatrix3f(const ProgramHandle & _program, const stick::String & _name, stick::Float32 * _ptr);

        void setProgramVariableMatrix4f(const ProgramHandle & _program, const stick::String & _name, stick::Float32 * _ptr);

        void attachTexture(const ProgramHandle & _program, const stick::String & _name, const TextureHandle & _texture, const SamplerHandle & _sampler);

        void addVertexBuffer(const MeshHandle & _mesh, const VertexBufferHandle & _vertexBuffer);

        void setIndexBuffer(const MeshHandle & _mesh, const IndexBufferHandle & _indexBuffer);

        void loadVertexDataRaw(const VertexBufferHandle & _handle, stick::Size _byteCount, const void * _dataSource);

        template<class T>
        void loadVertexData(const VertexBufferHandle & _handle, stick::Size _elementCount, const T * _dataSource)
        {
            loadVertexDataRaw(_handle, sizeof(T) * _elementCount, _dataSource);
        }

        void updateVertexDataRaw(const VertexBufferHandle & _handle, stick::Size _byteOffset, stick::Size _byteCount, const void * _dataSource);

        template<class T>
        void updateVertexData(const VertexBufferHandle & _handle, stick::Size _elementOffset, stick::Size _elementCount, const T * _dataSource)
        {
            updateVertexDataRaw(_handle, sizeof(T) * _elementOffset, sizeof(T) * _elementCount, _dataSource);
        }

        void loadIndexDataRaw(const IndexBufferHandle & _handle, stick::Size _byteCount, const void * _dataSource);

        template<class T>
        void loadIndexData(const IndexBufferHandle & _handle, stick::Size _elementCount, const T * _dataSource)
        {
            loadIndexDataRaw(_handle, _elementCount * sizeof(T), _dataSource);
        }

        void updateIndexDataRaw(const IndexBufferHandle & _handle, stick::Size _byteOffset, stick::Size _byteCount, const void * _dataSource);

        template<class T>
        void updateIndexData(const IndexBufferHandle & _handle, stick::Size _elementOffset, stick::Size _elementCount, const T * _dataSource)
        {
            updateIndexDataRaw(_handle, sizeof(T) * _elementOffset, sizeof(T) * _elementCount, _dataSource);
        }

        void clearBuffers(BufferType _buffersMask, const ClearBuffersSettings & _settings);

        void setViewport(stick::Float32 _x, stick::Float32 _y, stick::Float32 _width, stick::Float32 _height);

        void draw(const RenderStateHandle & _renderState, const ProgramHandle & _program, const MeshHandle & _mesh, VertexDrawMode _mode, stick::Size _vertexOffset, stick::Size _vertexCount, stick::Size _instanceCount = 1);

        void clear();

        stick::Size commandCount() const;


    private:

        CommandBuffer(stick::Allocator & _allocator = stick::defaultAllocator());


        detail::CommandBufferImplPtr m_pimpl;
    };
}

#endif //PIGMENT_COMMANDBUFFER_HPP
