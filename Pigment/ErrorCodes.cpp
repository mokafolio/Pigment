#include <Pigment/ErrorCodes.hpp>
#include <Stick/Error.hpp>

namespace pigment
{
using namespace stick;

PigmentErrorCategory::PigmentErrorCategory() : ErrorCategory("Pigment")
{
}

String PigmentErrorCategory::description(const Error & _code) const
{
    switch (_code.code())
    {
    case ec::UnsupportedRenderFeature:
        return "A render feature is not supported";
    case ec::BadRenderBuffer:
        return "A render buffer is invalid";
    case ec::BadGPUProgram:
        return "A GPU program could not be compiled or linked";
    case ec::BadProgramVariable:
        return "A GPU program variable could not be found or set";
    case ec::UnspecifiedError:
        return "An unspecified rendering error occured";
    default:
        return "Pigment Error";
    }
}

namespace detail
{
const PigmentErrorCategory & pigmentErrorCategory()
{
    static PigmentErrorCategory s_cat;
    return s_cat;
}
} // namespace detail
} // namespace pigment
