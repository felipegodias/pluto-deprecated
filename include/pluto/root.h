#pragma once

#include "api.h"
#include <string>
#include <memory>

namespace Pluto
{
    class PLUTO_API Root
    {
    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        Root(const std::string& configFileName, const std::string& logFileName,
             const std::string& assetsDirectoryName);

        Root(const Root& other) = delete;

        Root(Root&& other) noexcept = delete;

        ~Root();

        Root& operator=(const Root& other) = delete;

        Root& operator=(Root&& other) noexcept = delete;

        void Run() const;
    };
}
