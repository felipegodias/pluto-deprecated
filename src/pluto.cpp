#include <pluto/pluto.h>
#include <glm/glm.hpp>
#include <iostream>

namespace pluto
{
    void Pluto::run()
    {
        const glm::vec3 vec(1, 2, 3);
        std::cout << vec.x << " " << vec.y << " " << vec.z << " pluto!" << std::endl;
    }
}
