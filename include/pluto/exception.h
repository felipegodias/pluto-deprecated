#pragma once

#include "api.h"
#include <exception>
#include <memory>
#include <string>

namespace pluto
{
    class PLUTO_API Exception final : public std::exception
    {
    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit Exception(std::unique_ptr<Impl> impl);
        Exception(const Exception& other) = delete;
        Exception(Exception&& other) noexcept;
        ~Exception() noexcept override;

        Exception& operator=(const Exception& rhs) = delete;
        Exception& operator=(Exception&& rhs) noexcept;

        char const* what() const override;
        const std::string& GetStackTrace() const;

        static void Throw(const std::exception& e);
    };
}
