#include "pluto/math/math.h"
#include <limits>

namespace pluto
{
    const float Math::PI = 3.14159265358979;
    const float Math::DEG_2_RAD = (PI * 2) / 360;
    const float Math::RAD_2_DEG = 360 / (PI * 2);
    const float Math::EPSILON = std::numeric_limits<float>::epsilon();

    float Math::Radians(const float degree)
    {
        return degree * DEG_2_RAD;
    }

    float Math::Degrees(const float radian)
    {
        return radian * RAD_2_DEG;
    }
}
