#pragma once

#include "pluto/api.h"
#include "pluto/service/base_factory.h"

#include "pluto/memory/object.h"

#include <string>
#include <memory>

namespace pluto
{
    class Guid;
    class FileStreamWriter;
    class StreamReader;

    class PLUTO_API Asset : public Object
    {
    public:
        class PLUTO_API Factory : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            virtual std::unique_ptr<Asset> Create(StreamReader& reader) const = 0;
        };

        enum class Type
        {
            Unknown = 0,
            PackageManifest = 1,
            Text = 2,
            Mesh = 3,
            Shader = 4,
            Texture = 5,
            Material = 6,
            Font = 7
        };

        virtual ~Asset() = 0;
        Asset();

        Asset(const Asset& other) = delete;
        Asset(Asset&& other) noexcept;
        Asset& operator=(const Asset& rhs) = delete;
        Asset& operator=(Asset&& rhs) noexcept;

        virtual void Dump(FileStreamWriter& fileWriter) const = 0;
    };
}
