#ifndef PIGMENT_SETTINGS_HPP
#define PIGMENT_SETTINGS_HPP

#include <Pigment/Constants.hpp>
#include <Pigment/Handles.hpp>
#include <Pigment/VertexLayout.hpp>

namespace pigment
{
struct STICK_API SamplerSettings
{
    SamplerSettings();

    TextureWrap wrapS;
    TextureWrap wrapT;
    TextureWrap wrapR;
    TextureFiltering filtering;
    bool useMipMapping;
};

struct STICK_API ProgramSettings
{
    ProgramSettings(const VertexLayout & _layout,
                    const stick::String & _vs,
                    const stick::String & _fs);
    ProgramSettings(const VertexLayout & _layout,
                    const stick::String & _vs,
                    const stick::String & _gs,
                    const stick::String & _fs);

    VertexLayout vertexLayout;
    stick::String vertexShaderCode;
    stick::String geometryShaderCode;
    stick::String fragmentShaderCode;
};

struct STICK_API MeshSettings
{
    stick::DynamicArray<VertexBufferHandle> vertexBuffers;
    IndexBufferHandle indexBuffer;
};

struct STICK_API RenderBufferSettings
{
    struct RenderTarget
    {
        TextureFormat format;
        stick::UInt32 mipmapLevelCount;
    };

    stick::Size width;
    stick::Size height;
    stick::UInt32 sampleCount;
    stick::DynamicArray<RenderTarget> renderTargets;
};
} // namespace pigment

#endif // PIGMENT_SETTINGS_HPP
