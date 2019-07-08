#pragma once

#include "api.h"
#include <string>
#include <memory>

namespace pluto
{
    class PLUTO_API Root
    {
    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        Root(const std::string& configFileName, const std::string& logFileName,
             const std::string& dataDirectoryName);

        ~Root();

        int Run() const;
    };
}
