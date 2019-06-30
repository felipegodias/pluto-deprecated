#pragma once

#include "../api.h"
#include <string>

namespace pluto
{
    class Guid;
    class FileWriter;

    class PLUTO_API Asset
    {
    public:
        virtual ~Asset() = 0;

        virtual const Guid& GetId() const = 0;
        virtual const std::string& GetName() const = 0;
        virtual void SetName(std::string name) = 0;
        virtual void Dump(FileWriter& fileWriter) const = 0;

        bool operator==(const Asset& rhs) const;
        bool operator!=(const Asset& rhs) const;
    };
}
