#ifndef PIGMENT_BUFFERLAYOUT_HPP
#define PIGMENT_BUFFERLAYOUT_HPP

#include <Pigment/Constants.hpp>
#include <Stick/DynamicArray.hpp>
#include <Stick/String.hpp>
#include <type_traits> //for remove_cv

namespace pigment
{
struct STICK_API BufferElement
{
    stick::String m_name;
    stick::Size m_elementCount;
    stick::Size m_offset; // the offset to the first element
    stick::Size m_stride; // the offset between the elements

    stick::UInt32 m_location;
    BufferDataType m_type;

    // if 0, the element is set for each vertex, if 1, the element is set for each instance, if 2
    // the element is set for every two instances etc. (only used by VertexLayout)
    stick::Size m_numberOfInstances;

    BufferElement(stick::Size _elementCount,
                  stick::Size _stride,
                  const stick::String & _name,
                  BufferDataType _dataType,
                  stick::UInt32 _location,
                  stick::Size _numberOfInstances) :
        m_name(_name),
        m_elementCount(_elementCount),
        m_stride(_stride),
        m_location(_location),
        m_type(_dataType),
        m_numberOfInstances(_numberOfInstances)
    {
    }

    // returns the byte size of the element (m_elementCount * sizeOf(m_dataType))
    stick::Size size() const;
};

class STICK_API BufferLayout
{
  public:
    typedef stick::DynamicArray<BufferElement> BufferElementArray;

    BufferLayout(stick::Allocator & _alloc = stick::defaultAllocator());

    BufferLayout(const BufferLayout & _other);

    virtual ~BufferLayout();

    BufferLayout & addElement(const stick::String & _name,
                              stick::Size _elementCount,
                              BufferDataType _dataType = BufferDataType::Float32);

    BufferLayout & addElement(const stick::String & _name,
                              stick::Size _elementCount,
                              stick::UInt32 _location,
                              BufferDataType _dataType = BufferDataType::Float32);

    // needs to be called after all elements have been added to calculate some internalts (stride,
    // offset of the elements)
    void finish();

    // returns the byte size of the layout (sum of byte size of all elements)
    stick::Size byteCount() const;

    // checks if the layout contains an element named _name
    bool hasElement(const stick::String & _name) const;

    const BufferElementArray & elements() const;

    BufferElementArray & elements();

    bool isEmpty() const;

  protected:
    // calculates the byte stride and offsets for all added alements
    void calculateStridesAndOffsets();

    BufferElementArray m_elements;

    // size in bytes of the layout
    stick::Size m_size;
};

/*
namespace detail
{
    //helper functions

    template<class T>
    BufferDataType detectBufferDataTypeInternal(stick::ErrorReport & _error)
    {
        _error = stick::ErrorReport(error::BadVertexBuffer, "Unsupported buffer data type!",
__FILE__, __LINE__); return BufferDataType::UInt32; //just to return anything to satisfy the
compiler, will be ignored
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::UInt8>(stick::ErrorReport & _error)
    {
        return BufferDataType::UInt8;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::Int8>(stick::ErrorReport & _error)
    {
        return BufferDataType::Int8;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::UInt16>(stick::ErrorReport & _error)
    {
        return BufferDataType::UInt16;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::Int16>(stick::ErrorReport & _error)
    {
        return BufferDataType::Int16;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::UInt32>(stick::ErrorReport & _error)
    {
        return BufferDataType::UInt32;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::Int32>(stick::ErrorReport & _error)
    {
        return BufferDataType::Int32;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::Float32>(stick::ErrorReport & _error)
    {
        return BufferDataType::Float32;
    }

    template<>
    BufferDataType detectBufferDataTypeInternal<stick::Float64>(stick::ErrorReport & _error)
    {
        return BufferDataType::Float64;
    }

    template<class T>
    BufferDataType detectBufferDataType(stick::ErrorReport & _error)
    {
        typedef typename std::remove_cv<T>::type TheType;

        return detectBufferDataTypeInternal<T>(_error);
    }
}*/
} // namespace pigment

#endif // PIGMENT_BUFFERLAYOUT_HPP
