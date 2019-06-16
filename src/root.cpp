#include <pluto/root.h>
#include <glm/glm.hpp>
#include <iostream>

namespace Pluto
{
    class Root::Impl
    {
    public:
        Impl(const std::string& configFileName, const std::string& logFileName,
             const std::string& assetsDirectoryName)
        {
            std::cout << "Pluto Engine startup..." << std::endl;
        }

        Impl(const Impl& other) = delete;

        Impl(Impl&& other) noexcept = delete;

        ~Impl()
        {
            std::cout << "Pluto Engine shutdown..." << std::endl;
        }

        Impl& operator=(const Impl& other) = delete;

        Impl& operator=(Impl&& other) noexcept = delete;

        int Run() const
        {
            const glm::vec3 vec(1, 2, 3);
            std::cout << vec.x << " " << vec.y << " " << vec.z << " pluto!" << std::endl;
            return 0;
        }
    };

    Root::Root(const std::string& configFileName, const std::string& logFileName,
               const std::string& assetsDirectoryName) : impl(
        std::make_unique<Impl>(configFileName, logFileName, assetsDirectoryName))
    {
    }

    Root::~Root() = default;

    int Root::Run() const
    {
        return impl->Run();
    }
}
