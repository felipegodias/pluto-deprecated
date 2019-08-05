#pragma once

#include "api.h"
#include <string>
#include <memory>
#include <functional>

namespace pluto
{
    class ServiceCollection;

    class PLUTO_API Root
    {
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        Root(const std::string& configFileName, const std::string& logFileName,
             const std::string& dataDirectoryName);

        ~Root();

        int Run(const std::function<void(ServiceCollection& serviceCollection)>& onInit) const;
    };
}
