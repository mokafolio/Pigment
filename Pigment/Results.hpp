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

    STICK_RESULT_HOLDER(TextureHandleResultHolder, texture);
    typedef stick::Result<TextureHandle, TextureHandleResultHolder> TextureHandleResult;
    STICK_RESULT_HOLDER(SamplerHandleResultHolder, sampler);
    typedef stick::Result<SamplerHandle, SamplerHandleResultHolder> SamplerHandleResult;
    STICK_RESULT_HOLDER(ProgramHandleResultHolder, program);
    typedef stick::Result<ProgramHandle, ProgramHandleResultHolder> ProgramHandleResult;
    STICK_RESULT_HOLDER(MeshHandleResultHolder, mesh);
    typedef stick::Result<MeshHandle, MeshHandleResultHolder> MeshHandleResult;
    STICK_RESULT_HOLDER(VertexBufferHandleResultHolder, vertexBuffer);
    typedef stick::Result<VertexBufferHandle, VertexBufferHandleResultHolder> VertexBufferHandleResult;
    STICK_RESULT_HOLDER(IndexBufferHandleResultHolder, indexBuffer);
    typedef stick::Result<IndexBufferHandle, IndexBufferHandleResultHolder> IndexBufferHandleResult;
    STICK_RESULT_HOLDER(RenderStateHandleResultHolder, renderState);
    typedef stick::Result<RenderStateHandle, RenderStateHandleResultHolder> RenderStateHandleResult;
    STICK_RESULT_HOLDER(CommandBufferResultHolder, commandBuffer);
    typedef stick::Result<CommandBuffer &, CommandBufferResultHolder> CommandBufferResult;
    STICK_RESULT_HOLDER(RenderBufferResultHolder, renderBuffer);
    typedef stick::Result<RenderBuffer, RenderBufferResultHolder> RenderBufferResult;
}

#endif //PIGMENT_RESULTS_HPP
