#pragma once

#include "pluto/api.h"

namespace pluto
{
    class PLUTO_API Math
    {
    public:
        static const float PI;
        static const float DEG_2_RAD;
        static const float RAD_2_DEG;
        static const float EPSILON;

        static float Radians(float degree);
        static float Degrees(float radian);
    };
}
