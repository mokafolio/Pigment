#ifndef PIGMENT_VERTEXLAYOUT_HPP
#define PIGMENT_VERTEXLAYOUT_HPP

#include <Pigment/BufferLayout.hpp>

namespace pigment
{
    class STICK_API VertexLayout : public BufferLayout
    {
    public:

        VertexLayout();

        ~VertexLayout();


        VertexLayout & addInstanceElement(stick::Size _numberOfInstances, const stick::String & _name, stick::Size _size, stick::UInt32 _location, BufferDataType _dataType = BufferDataType::Float32);

        VertexLayout & addInstanceElement(stick::Size _numberOfInstances, const stick::String & _name, stick::Size _size, BufferDataType _dataType = BufferDataType::Float32);
    };
}

#endif //PIGMENT_VERTEXLAYOUT_HPP
