#ifndef PIGMENT_HANDLES_HPP
#define PIGMENT_HANDLES_HPP

#include <Stick/Platform.hpp>

#define PIGMENT_HANDLE(name)                                                                       \
    class STICK_API name                                                                           \
    {                                                                                              \
        friend struct detail::RendererImpl;                                                        \
                                                                                                   \
      public:                                                                                      \
        name() : m_index(-1), m_rimpl(nullptr)                                                     \
        {                                                                                          \
        }                                                                                          \
        bool isValid() const                                                                       \
        {                                                                                          \
            return m_index != -1;                                                                  \
        }                                                                                          \
        bool operator==(const name & _other) const                                                 \
        {                                                                                          \
            if (!isValid() || !_other.isValid())                                                   \
                return m_index == _other.m_index;                                                  \
            else                                                                                   \
                return m_index == _other.m_index && m_rimpl == _other.m_rimpl;                     \
        }                                                                                          \
        bool operator!=(const name & _other) const                                                 \
        {                                                                                          \
            return !(*this == _other);                                                             \
        }                                                                                          \
                                                                                                   \
      private:                                                                                     \
        name(stick::Size _index, detail::RendererImpl * _renderer) :                               \
            m_index(_index),                                                                       \
            m_rimpl(_renderer)                                                                     \
        {                                                                                          \
        }                                                                                          \
        stick::Size m_index;                                                                       \
        detail::RendererImpl * m_rimpl;                                                            \
    };

namespace pigment
{
namespace detail
{
struct RendererImpl;
}

PIGMENT_HANDLE(TextureHandle)
PIGMENT_HANDLE(ShaderVariableHandle)
PIGMENT_HANDLE(ProgramHandle)
PIGMENT_HANDLE(SamplerHandle)
PIGMENT_HANDLE(MeshHandle)
PIGMENT_HANDLE(VertexBufferHandle)
PIGMENT_HANDLE(IndexBufferHandle)
PIGMENT_HANDLE(RenderStateHandle)
PIGMENT_HANDLE(RenderBufferHandle)
} // namespace pigment

#endif // PIGMENT_HANDLES_HPP
