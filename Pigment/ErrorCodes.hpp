#ifndef PIGMENT_ERRORCODES_HPP
#define PIGMENT_ERRORCODES_HPP

#include <Stick/ErrorCategory.hpp>

namespace pigment
{
    namespace ec
    {
        enum PigmentErrorCode
        {
            UnsupportedRenderFeature = 1,
            BadRenderBuffer,
            BadGPUProgram,
            BadProgramVariable,
            UnspecifiedError
        };
    }

    class STICK_API PigmentErrorCategory :
        public stick::ErrorCategory
    {
    public:

        PigmentErrorCategory();

        stick::String description(const stick::Error & _code) const;
    };

    namespace detail
    {
        STICK_API const PigmentErrorCategory & pigmentErrorCategory();
    }
}

namespace stick
{
    namespace detail
    {
        template<>
        struct isErrorEnum<pigment::ec::PigmentErrorCode>
        {
            static const bool value = true;
        };

        inline const stick::ErrorCategory & errorCategory(pigment::ec::PigmentErrorCode)
        {
            return pigment::detail::pigmentErrorCategory();
        }
    }
}

#endif //PIGMENT_ERRORCODES_HPP
