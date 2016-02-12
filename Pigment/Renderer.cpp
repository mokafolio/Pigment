#include <Pigment/Renderer.hpp>
#include <Pigment/OpenGL/GLRendererImpl.hpp>

namespace pigment
{
    using namespace stick;

    //class declaration is in Handles.hpp
    RenderBuffer::RenderBuffer(const RenderBufferHandle & _handle, const TextureHandleArray & _colorTargets, const TextureHandle & _depthTarget) :
        m_handle(_handle),
        m_colorTargets(_colorTargets),
        m_depthTarget(_depthTarget)
    {

    }

    const RenderBuffer::TextureHandleArray & RenderBuffer::colorTargets() const
    {
        return m_colorTargets;
    }

    const TextureHandle & RenderBuffer::depthTarget() const
    {
        return m_depthTarget;
    }

    const RenderBufferHandle & RenderBuffer::handle() const
    {
        return m_handle;
    }


    Renderer::Renderer(Allocator & _alloc) :
        m_pimpl(_alloc.create<detail::RendererImpl>(_alloc))
    {

    }

    Renderer::~Renderer()
    {

    }

    CommandBufferResult Renderer::createCommandBuffer()
    {
        return m_pimpl->createCommandBuffer();
    }

    void Renderer::destroyCommandBuffer(CommandBuffer & _buffer)
    {
        return m_pimpl->destroyCommandBuffer(_buffer);
    }

    TextureHandleResult Renderer::createTexture()
    {
        return m_pimpl->createTexture();
    }

    void Renderer::destroyTexture(const TextureHandle & _tex)
    {
        m_pimpl->destroyTexture(_tex);
    }

    SamplerHandleResult Renderer::createSampler(const SamplerSettings & _settings)
    {
        return m_pimpl->createSampler(_settings);
    }

    void Renderer::destroySampler(const SamplerHandle & _sampler)
    {
        m_pimpl->destroySampler(_sampler);
    }

    RenderBufferResult Renderer::createRenderBuffer(const RenderBufferSettings & _settings)
    {
        return m_pimpl->createRenderBuffer(_settings);
    }

    void Renderer::destroyRenderBuffer(const RenderBufferHandle & _renderBufferHandle, bool _bDestroyRenderTargets)
    {
        m_pimpl->destroyRenderBuffer(_renderBufferHandle, _bDestroyRenderTargets);
    }

    ProgramHandleResult Renderer::createProgram(const ProgramSettings & _settings)
    {
        return m_pimpl->createProgram(_settings);
    }

    void Renderer::destroyProgram(const ProgramHandle & _handle)
    {
        m_pimpl->destroyProgram(_handle);
    }

    MeshHandleResult Renderer::createMesh(const MeshSettings & _settings)
    {
        return m_pimpl->createMesh(_settings);
    }

    void Renderer::destroyMesh(const MeshHandle & _mesh)
    {
        m_pimpl->destroyMesh(_mesh);
    }

    VertexBufferHandleResult Renderer::createVertexBuffer(const VertexLayout & _layout, BufferUsage _usage)
    {
        return m_pimpl->createVertexBuffer(_layout, _usage);
    }

    void Renderer::destroyVertexBuffer(const VertexBufferHandle & _handle)
    {
        m_pimpl->destroyVertexBuffer(_handle);
    }

    IndexBufferHandleResult Renderer::createIndexBuffer(BufferUsage _usage)
    {
        return m_pimpl->createIndexBuffer(_usage);
    }

    void Renderer::destroyIndexBuffer(const IndexBufferHandle & _handle)
    {
        m_pimpl->destroyIndexBuffer(_handle);
    }

    RenderStateHandleResult Renderer::createRenderState(const RenderStateSettings & _settings)
    {
        return m_pimpl->createRenderState(_settings);
    }

    void Renderer::destoyRenderState(const RenderStateHandle & _handle)
    {
        m_pimpl->destoyRenderState(_handle);
    }

    void Renderer::addCommandBuffer(const CommandBuffer & _buffer)
    {
        m_pimpl->addCommandBuffer(_buffer);
    }

    Error Renderer::submit()
    {
        return m_pimpl->submit();
    }

    Error Renderer::submitCommandBuffer(const CommandBuffer & _buffer)
    {
        return m_pimpl->submitCommandBuffer(_buffer);
    }
}
