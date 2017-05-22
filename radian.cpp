#include "radian.h"
#include "degree.h"

namespace pk
{
    radian::operator degree() const
    {
        return degree{value * 57.295779513f};
    }
}

pk::radian operator""_rad(long double value)
{
    return pk::radian{static_cast<float>(value)};
}