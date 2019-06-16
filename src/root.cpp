#include <pluto/root.h>
#include <glm/glm.hpp>
#include <iostream>
#include <utility>

namespace Pluto
{
    class Root::Impl
    {
    private:
        std::string configFileName;
        std::string logFileName;
        std::string assetsDirectoryName;

    public:
        Impl(std::string configFileName, std::string logFileName,
             std::string assetsDirectoryName)
            : configFileName(std::move(configFileName)),
              logFileName(std::move(logFileName)),
              assetsDirectoryName(std::move(assetsDirectoryName))
        {
        }

        void Run()
        {
            const glm::vec3 vec(1, 2, 3);
            std::cout << vec.x << " " << vec.y << " " << vec.z << " pluto!" << std::endl;
        }
    };

    Root::Root(const std::string& configFileName, const std::string& logFileName,
               const std::string& assetsDirectoryName) : impl(
        std::make_unique<Impl>(configFileName, logFileName, assetsDirectoryName))
    {
    }

    Root::~Root() = default;

    void Root::Run() const
    {
        impl->Run();
    }
}
