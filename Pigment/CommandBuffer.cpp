#include <Pigment/OpenGL/GLCommandBufferImpl.hpp>

namespace pigment
{
    using namespace stick;

    CommandBuffer::CommandBuffer(Allocator & _allocator) :
    m_pimpl(_allocator.create<detail::CommandBufferImpl>(_allocator))
    {

    }

    CommandBuffer::~CommandBuffer()
    {

    }

    void CommandBuffer::loadPixelsRaw(const TextureHandle & _handle, Size _width, Size _height, Size _depth,
                                      void * _pixels, PixelDataType _dataType, TextureFormat _format, UInt32 _mipmapLevelCount)
    {
        m_pimpl->loadPixelsRaw(_handle, _width, _height, _depth, _pixels, _dataType, _format, _mipmapLevelCount);
    }

    void CommandBuffer::allocatePixels(const TextureHandle & _handle, Size _width, Size _height, Size _depth, TextureFormat _format, UInt32 _mipmapLevelCount)
    {
        loadPixelsRaw(_handle, _width, _height, _depth, nullptr, PixelDataType::UInt8, _format, _mipmapLevelCount);
    }

    void CommandBuffer::allocatePixels(const TextureHandle & _handle, Size _width, Size _height, TextureFormat _format, UInt32 _mipmapLevelCount)
    {
        loadPixelsRaw(_handle, _width, _height, 1, nullptr, PixelDataType::UInt8, _format, _mipmapLevelCount);
    }

    void CommandBuffer::allocatePixels(const TextureHandle & _handle, Size _width, TextureFormat _format, UInt32 _mipmapLevelCount)
    {
        loadPixelsRaw(_handle, _width, 1, 1, nullptr, PixelDataType::UInt8, _format, _mipmapLevelCount);
    }

    void CommandBuffer::updatePixelsRaw(const TextureHandle & _handle, Size _x, Size _y, Size _z,
                                        Size _width, Size _height, Size _depth, void * _pixels, PixelDataType _dataType)
    {
        m_pimpl->updatePixelsRaw(_handle, _x, _y, _z, _width, _height, _depth, _pixels, _dataType);
    }

    void CommandBuffer::setProgramVariable(const ProgramHandle & _program, const String & _name, const ProgramVariableValue & _val)
    {
        m_pimpl->setProgramVariable(_program, _name, _val);
    }

    void CommandBuffer::setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a)
    {
        ProgramVariableValue v;
        v.floatValue = _a;
        setProgramVariable(_program, _name, v);
    }

    void CommandBuffer::setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b)
    {
        ProgramVariableValue v;
        v.vec2fValue = {_a, _b};
        setProgramVariable(_program, _name, v);
    }

    void CommandBuffer::setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b, Float32 _c)
    {
        ProgramVariableValue v;
        v.vec3fValue = {_a, _b, _c};
        setProgramVariable(_program, _name, v);
    }

    void CommandBuffer::setProgramVariablef(const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b, Float32 _c, Float32 _d)
    {
        ProgramVariableValue v;
        v.vec4fValue = {_a, _b, _c, _d};
        setProgramVariable(_program, _name, v);
    }

    void CommandBuffer::setProgramVariableMatrix3f(const ProgramHandle & _program, const String & _name, Float32 * _ptr)
    {
        ProgramVariableValue v;
        memcpy(v.mat3fValue.v, _ptr, sizeof(ProgramVariableValue::Mat3f));
        setProgramVariable(_program, _name, v);
    }

    void CommandBuffer::setProgramVariableMatrix4f(const ProgramHandle & _program, const String & _name, Float32 * _ptr)
    {
        ProgramVariableValue v;
        memcpy(v.mat3fValue.v, _ptr, sizeof(ProgramVariableValue::Mat3f));
        setProgramVariable(_program, _name, v);
    }

    void CommandBuffer::attachTexture(const ProgramHandle & _program, const String & _name, const TextureHandle & _texture, const SamplerHandle & _sampler)
    {
        m_pimpl->attachTexture(_program, _name, _texture, _sampler);
    }

    void CommandBuffer::loadVertexDataRaw(const VertexBufferHandle & _handle, Size _byteCount, const void * _dataSource)
    {
        m_pimpl->loadVertexDataRaw(_handle, _byteCount, _dataSource);
    }

    void CommandBuffer::updateVertexDataRaw(const VertexBufferHandle & _handle, Size _byteOffset, Size _byteCount, const void * _dataSource)
    {
        m_pimpl->updateVertexDataRaw(_handle, _byteCount, _byteCount, _dataSource);
    }

    void CommandBuffer::loadIndexDataRaw(const IndexBufferHandle & _handle, Size _byteCount, const void * _dataSource)
    {
        m_pimpl->loadIndexDataRaw(_handle, _byteCount, _dataSource);
    }

    void CommandBuffer::updateIndexDataRaw(const IndexBufferHandle & _handle, Size _byteOffset, Size _byteCount, const void * _dataSource)
    {
        m_pimpl->updateIndexDataRaw(_handle, _byteCount, _byteCount, _dataSource);
    }

    void CommandBuffer::bindRenderBuffer(const RenderBufferHandle & _handle)
    {
        m_pimpl->bindRenderBuffer(_handle);
    }

    void CommandBuffer::clearBuffers(BufferType _buffersMask, const ClearBuffersSettings & _settings)
    {
        m_pimpl->clearBuffers(_buffersMask, _settings);
    }

    void CommandBuffer::setViewport(Float32 _x, Float32 _y, Float32 _width, Float32 _height)
    {
        m_pimpl->setViewport(_x, _y, _width, _height);
    }

    void CommandBuffer::draw(const RenderStateHandle & _renderState, const ProgramHandle & _program, const MeshHandle & _mesh, VertexDrawMode _mode, Size _vertexOffset, Size _vertexCount, Size _instanceCount)
    {
        m_pimpl->draw(_renderState, _program, _mesh, _mode, _vertexOffset, _vertexCount, _instanceCount);
    }

    void CommandBuffer::clear()
    {
        m_pimpl->clear();
    }

    Size CommandBuffer::commandCount() const
    {
        return m_pimpl->commandCount();
    }
}
