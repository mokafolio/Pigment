#ifndef PIGMENT_RESULTS_HPP
#define PIGMENT_RESULTS_HPP

#include <Pigment/Handles.hpp>
#include <Stick/Result.hpp>

namespace pigment
{
    class CommandBuffer;

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
}

#endif //PIGMENT_RESULTS_HPP
