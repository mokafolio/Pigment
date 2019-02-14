#include <Pigment/VertexLayout.hpp>

namespace pigment
{
using namespace stick;

VertexLayout::VertexLayout()
{
}

VertexLayout::~VertexLayout()
{
}

VertexLayout & VertexLayout::addInstanceElement(Size _numInstances,
                                                const String & _name,
                                                Size _size,
                                                BufferDataType _dataType)
{
    m_elements.append(
        BufferElement(_size, 0, _name, _dataType, (UInt32)m_elements.count(), _numInstances));
    return *this;
}

VertexLayout & VertexLayout::addInstanceElement(Size _numInstances,
                                                const String & _name,
                                                Size _size,
                                                UInt32 _location,
                                                BufferDataType _dataType)
{
    m_elements.append(BufferElement(_size, 0, _name, _dataType, _location, _numInstances));
    return *this;
}

} // namespace pigment
