#include <Pigment/OpenGL/GLCommandBufferImpl.hpp>

namespace pigment
{
namespace detail
{
CommandBufferImpl::CommandBufferImpl(Allocator & _allocator) :
    m_queue(_allocator),
    m_stringBuffer(_allocator)
{
}
void CommandBufferImpl::loadPixelsRaw(const TextureHandle & _handle,
                                      Size _width,
                                      Size _height,
                                      Size _depth,
                                      void * _pixels,
                                      PixelDataType _dataType,
                                      TextureFormat _format,
                                      UInt32 _mipmapLevelCount,
                                      UInt8 _alignment)
{
    Command cmd;
    cmd.loadPixelsCommand = { _handle,   _width,  _height,           _depth,    _pixels,
                              _dataType, _format, _mipmapLevelCount, _alignment };
    m_queue.append({ CommandType::LoadPixels, cmd });
}

void CommandBufferImpl::updatePixelsRaw(const TextureHandle & _handle,
                                        Size _x,
                                        Size _y,
                                        Size _z,
                                        Size _width,
                                        Size _height,
                                        Size _depth,
                                        void * _pixels,
                                        PixelDataType _dataType)
{
    Command cmd;
    cmd.updatePixelsCommand = { _handle, _x, _y, _z, _width, _height, _depth, _pixels, _dataType };
    m_queue.append({ CommandType::UpdatePixels, cmd });
}

void CommandBufferImpl::setProgramVariable(const ProgramHandle & _program,
                                           const String & _name,
                                           const ProgramVariableValue & _val)
{
    Command cmd;
    m_stringBuffer.append(_name);
    cmd.setProgramVariableCommand = { _program, m_stringBuffer.count() - 1, _val };
    m_queue.append({ CommandType::SetProgramVariable, cmd });
}

void CommandBufferImpl::setProgramVariablef(const ProgramHandle & _program,
                                            const String & _name,
                                            Float32 _a)
{
    ProgramVariableValue v;
    v.floatValue = _a;
    setProgramVariable(_program, _name, v);
}

void CommandBufferImpl::setProgramVariablef(const ProgramHandle & _program,
                                            const String & _name,
                                            Float32 _a,
                                            Float32 _b)
{
    ProgramVariableValue v;
    v.vec2fValue = { _a, _b };
    setProgramVariable(_program, _name, v);
}

void CommandBufferImpl::setProgramVariablef(
    const ProgramHandle & _program, const String & _name, Float32 _a, Float32 _b, Float32 _c)
{
    ProgramVariableValue v;
    v.vec3fValue = { _a, _b, _c };
    setProgramVariable(_program, _name, v);
}

void CommandBufferImpl::setProgramVariablef(const ProgramHandle & _program,
                                            const String & _name,
                                            Float32 _a,
                                            Float32 _b,
                                            Float32 _c,
                                            Float32 _d)
{
    ProgramVariableValue v;
    v.vec4fValue = { _a, _b, _c, _d };
    setProgramVariable(_program, _name, v);
}

void CommandBufferImpl::setProgramVariableMatrix3f(const ProgramHandle & _program,
                                                   const String & _name,
                                                   Float32 * _ptr)
{
    ProgramVariableValue v;
    memcpy(v.mat3fValue.v, _ptr, sizeof(ProgramVariableValue::Mat3f));
    setProgramVariable(_program, _name, v);
}

void CommandBufferImpl::setProgramVariableMatrix4f(const ProgramHandle & _program,
                                                   const String & _name,
                                                   Float32 * _ptr)
{
    ProgramVariableValue v;
    memcpy(v.mat3fValue.v, _ptr, sizeof(ProgramVariableValue::Mat3f));
    setProgramVariable(_program, _name, v);
}

void CommandBufferImpl::attachTexture(const ProgramHandle & _program,
                                      const String & _name,
                                      const TextureHandle & _texture,
                                      const SamplerHandle & _sampler)
{
    Command cmd;
    m_stringBuffer.append(_name);
    cmd.attachTextureCommand = { _program, m_stringBuffer.count() - 1, _texture, _sampler };
    m_queue.append({ CommandType::AttachTexture, cmd });
}

void CommandBufferImpl::loadVertexDataRaw(const VertexBufferHandle & _handle,
                                          Size _byteCount,
                                          const void * _dataSource)
{
    Command cmd;
    cmd.loadVertexDataCommand = { _handle, _byteCount, _dataSource };
    m_queue.append({ CommandType::LoadVertexData, cmd });
}

void CommandBufferImpl::updateVertexDataRaw(const VertexBufferHandle & _handle,
                                            Size _byteOffset,
                                            Size _byteCount,
                                            const void * _dataSource)
{
    Command cmd;
    cmd.updateVertexDataCommand = { _handle, _byteOffset, _byteCount, _dataSource };
    m_queue.append({ CommandType::UpdateVertexData, cmd });
}

void CommandBufferImpl::loadIndexDataRaw(const IndexBufferHandle & _handle,
                                         Size _byteCount,
                                         const void * _dataSource)
{
    Command cmd;
    cmd.loadIndexDataCommand = { _handle, _byteCount, _dataSource };
    m_queue.append({ CommandType::LoadIndexData, cmd });
}

void CommandBufferImpl::updateIndexDataRaw(const IndexBufferHandle & _handle,
                                           Size _byteOffset,
                                           Size _byteCount,
                                           const void * _dataSource)
{
    Command cmd;
    cmd.updateIndexDataCommand = { _handle, _byteOffset, _byteCount, _dataSource };
    m_queue.append({ CommandType::UpdateIndexData, cmd });
}

void CommandBufferImpl::bindRenderBuffer(const RenderBufferHandle & _handle)
{
    Command cmd;
    cmd.bindRenderBufferCommand = { _handle };
    m_queue.append({ CommandType::BindRenderBuffer, cmd });
}

void CommandBufferImpl::clearBuffers(BufferType _buffersMask,
                                     const ClearBuffersSettings & _settings)
{
    Command cmd;
    cmd.clearBuffersCommand = { _buffersMask, _settings };
    m_queue.append({ CommandType::ClearBuffers, cmd });
}

void CommandBufferImpl::setViewport(Float32 _x, Float32 _y, Float32 _width, Float32 _height)
{
    Command cmd;
    cmd.setViewportCommand = { _x, _y, _width, _height };
    m_queue.append({ CommandType::SetViewport, cmd });
}

void CommandBufferImpl::draw(const RenderStateHandle & _renderState,
                             const ProgramHandle & _program,
                             const MeshHandle & _mesh,
                             VertexDrawMode _mode,
                             Size _vertexOffset,
                             Size _vertexCount,
                             Size _instanceCount)
{
    Command cmd;
    cmd.drawCommand = { _renderState,  _program,     _mesh,         _mode,
                        _vertexOffset, _vertexCount, _instanceCount };
    m_queue.append({ CommandType::Draw, cmd });
}

void CommandBufferImpl::clear()
{
    m_queue.clear();
    m_stringBuffer.clear();
}

Size CommandBufferImpl::commandCount() const
{
    return m_queue.count();
}
} // namespace detail
} // namespace pigment
