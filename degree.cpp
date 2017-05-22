#include "degree.h"
#include "radian.h"

namespace pk
{
    degree::operator radian() const
    {
        return radian{value * 0.017453292f};
    }
}

pk::degree operator""_deg(long double value)
{
    return pk::degree{static_cast<float>(value)};
}