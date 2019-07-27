#pragma once

#include "pluto/service/base_factory.h"

#include <string>
#include <memory>

namespace pluto
{
    class PLUTO_API Path
    {
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit Path(const std::string& path);

        Path(const Path& other);
        Path(Path&& other) noexcept;

        ~Path();

        Path& operator=(const Path& rhs);
        Path& operator=(Path&& rhs) noexcept;

        friend PLUTO_API std::ostream& operator<<(std::ostream& os, const Path& path);

        std::string GetName() const;
        std::string GetNameWithoutExtension() const;

        bool HasExtension() const;
        std::string GetExtension() const;
        Path GetDirectory() const;

        Path GetRelativePath(const Path& directoryPath);

        std::string Str() const;

        void ChangeExtension(const std::string& extension);
        void RemoveExtension();
    };
}
