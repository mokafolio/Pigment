#ifndef PIGMENT_RESULTS_HPP
#define PIGMENT_RESULTS_HPP

#include <Pigment/Handles.hpp>
#include <Stick/Result.hpp>

namespace pigment
{
    class CommandBuffer;

    class STICK_API RenderBuffer
    {
        friend class detail::RendererImpl;

    public:

        typedef stick::DynamicArray<TextureHandle> TextureHandleArray;

        const TextureHandleArray & colorTargets() const;

        const TextureHandle & depthTarget() const;

        const RenderBufferHandle & handle() const;

    private:

        RenderBuffer(const RenderBufferHandle & _handle, const TextureHandleArray & _colorTargets, const TextureHandle & _depthTarget = TextureHandle());

        RenderBufferHandle m_handle;
        TextureHandleArray m_colorTargets;
        TextureHandle m_depthTarget;
    };

    typedef stick::Result<TextureHandle> TextureHandleResult;
    typedef stick::Result<SamplerHandle> SamplerHandleResult;
    typedef stick::Result<ProgramHandle> ProgramHandleResult;
    typedef stick::Result<MeshHandle> MeshHandleResult;
    typedef stick::Result<VertexBufferHandle> VertexBufferHandleResult;
    typedef stick::Result<IndexBufferHandle> IndexBufferHandleResult;
    typedef stick::Result<RenderStateHandle> RenderStateHandleResult;
    typedef stick::Result<CommandBuffer &> CommandBufferResult;
    typedef stick::Result<RenderBuffer> RenderBufferResult;
}

#endif //PIGMENT_RESULTS_HPP
