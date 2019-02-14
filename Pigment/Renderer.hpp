#ifndef PIGMENT_RENDERER_HPP
#define PIGMENT_RENDERER_HPP

#include <Pigment/CommandBuffer.hpp>
#include <Pigment/RenderStateSettings.hpp>
#include <Pigment/Results.hpp>
#include <Pigment/Settings.hpp>
#include <Stick/Error.hpp>
#include <Stick/Result.hpp>
#include <Stick/UniquePtr.hpp>

namespace pigment
{
struct SamplerSettings;

namespace detail
{
struct RendererImpl;
typedef stick::UniquePtr<RendererImpl> RendererImplUniquePtr;
} // namespace detail

class STICK_API Renderer
{
  public:
    Renderer(stick::Allocator & _alloc = stick::defaultAllocator());

    ~Renderer();

    CommandBufferResult createCommandBuffer();

    void destroyCommandBuffer(CommandBuffer & _buffer);

    TextureHandleResult createTexture();

    void destroyTexture(const TextureHandle & _tex);

    SamplerHandleResult createSampler(const SamplerSettings & _settings);

    void destroySampler(const SamplerHandle & _sampler);

    RenderBufferResult createRenderBuffer(const RenderBufferSettings & _settings);

    void destroyRenderBuffer(const RenderBufferHandle & _renderBufferHandle,
                             bool _bDestroyRenderTargets = true);

    ProgramHandleResult createProgram(const ProgramSettings & _settings);

    void destroyProgram(const ProgramHandle & _handle);

    MeshHandleResult createMesh(const MeshSettings & _settings);

    void destroyMesh(const MeshHandle & _mesh);

    VertexBufferHandleResult createVertexBuffer(const VertexLayout & _layout, BufferUsage _usage);

    void destroyVertexBuffer(const VertexBufferHandle & _handle);

    IndexBufferHandleResult createIndexBuffer(BufferUsage _usage);

    void destroyIndexBuffer(const IndexBufferHandle & _handle);

    RenderStateHandleResult createRenderState(const RenderStateSettings & _settings);

    void destoyRenderState(const RenderStateHandle & _handle);

    void addCommandBuffer(const CommandBuffer & _buffer);

    stick::Error submit();

    stick::Error submitCommandBuffer(const CommandBuffer & _buffer);

    // functions to read pixels from the default/window framebuffer
    // TODO: should these be commands?
    void readPixels(stick::UInt32 _left,
                    stick::UInt32 _bottom,
                    stick::UInt32 _width,
                    stick::UInt32 _height,
                    TextureFormat _format,
                    void * _pixels);

  private:
    detail::RendererImplUniquePtr m_pimpl;
};
} // namespace pigment

#endif // PIGMENT_RENDERER_HPP
