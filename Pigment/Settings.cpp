#include <Pigment/Settings.hpp>

namespace pigment
{
using namespace stick;

SamplerSettings::SamplerSettings() :
    wrapS(TextureWrap::ClampToEdge),
    wrapT(TextureWrap::ClampToEdge),
    wrapR(TextureWrap::ClampToEdge),
    filtering(TextureFiltering::Bilinear),
    useMipMapping(false)
{
}

ProgramSettings::ProgramSettings(const VertexLayout & _layout,
                                 const String & _vs,
                                 const String & _fs) :
    vertexLayout(_layout),
    vertexShaderCode(_vs),
    fragmentShaderCode(_fs)
{
}

ProgramSettings::ProgramSettings(const VertexLayout & _layout,
                                 const String & _vs,
                                 const String & _gs,
                                 const String & _fs) :
    vertexLayout(_layout),
    vertexShaderCode(_vs),
    geometryShaderCode(_gs),
    fragmentShaderCode(_fs)
{
}
} // namespace pigment
