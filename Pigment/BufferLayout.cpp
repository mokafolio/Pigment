#include <Pigment/BufferLayout.hpp>

namespace pigment
{
    using namespace stick;

    Size BufferElement::size() const
    {
        return m_elementCount * s_bufferDataTypeInfos[static_cast<Size>(m_type)].byteCount;
    }

    BufferLayout::BufferLayout(Allocator & _alloc) :
    m_elements(_alloc)
    {

    }

    BufferLayout::BufferLayout(const BufferLayout & _other) :
        m_elements(_other.m_elements),
        m_size(_other.m_size)
    {
        
    }

    BufferLayout::~BufferLayout()
    {

    }

    BufferLayout & BufferLayout::addElement(const String & _name, Size _elementCount, BufferDataType _dataType)
    {
        m_elements.append(BufferElement(_elementCount, 0, _name, _dataType, (UInt32)m_elements.count(), 0));
        return *this;
    }

    BufferLayout & BufferLayout::addElement(const String & _name, Size _elementCount, UInt32 _location, BufferDataType _dataType)
    {
        m_elements.append(BufferElement(_elementCount, 0, _name, _dataType, _location, 0));
        return *this;
    }

    void BufferLayout::calculateStridesAndOffsets()
    {
        BufferElementArray::Iter it = m_elements.begin();

        UInt32 byteOffset = 0;
        UInt32 stride = 0;

        for (; it != m_elements.end(); ++it)
        {
            stride += (*it).size();

            (*it).m_offset = byteOffset;

            byteOffset += (*it).size();
        }

        it = m_elements.begin();
        for (; it != m_elements.end(); ++it)
        {
            (*it).m_stride = stride;
        }

        //set the overall byte size of the layout
        m_size = byteOffset;
    }

    void BufferLayout::finish()
    {
        calculateStridesAndOffsets();
    }

    const BufferLayout::BufferElementArray & BufferLayout::elements() const
    {
        return m_elements;
    }

    BufferLayout::BufferElementArray & BufferLayout::elements()
    {
        return m_elements;
    }

    Size BufferLayout::byteCount() const
    {
        return m_size;
    }

    bool BufferLayout::hasElement(const String & _name) const
    {
        auto it = m_elements.begin();

        for (; it != m_elements.end(); ++it)
        {
            if ((*it).m_name == _name)
                return true;
        }
        return false;
    }

    bool BufferLayout::isEmpty() const
    {
        return m_elements.count() ? true : false;
    }
}
