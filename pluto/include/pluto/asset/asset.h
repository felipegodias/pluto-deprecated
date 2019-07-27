#pragma once

#include "pluto/api.h"
#include "pluto/service/base_factory.h"

#include <string>
#include <memory>

namespace pluto
{
    class Guid;
    class FileWriter;
    class FileReader;

    class PLUTO_API Asset
    {
    public:
        class Factory : public BaseFactory
        {
        public:
            ~Factory() override;
            explicit Factory(ServiceCollection& diContainer);

            Factory(const Factory& other) = delete;
            Factory(Factory&& other) noexcept;
            Factory& operator=(const Factory& rhs) = delete;
            Factory& operator=(Factory&& rhs) noexcept;

            virtual std::unique_ptr<Asset> Create(FileReader& fileReader) const = 0;
        };

        enum class Type
        {
            Unknown = 0,
            PackageManifest = 1,
            Text = 2,
            Mesh = 3,
            Shader = 4,
            Texture = 5,
            Material = 6
        };

        virtual ~Asset() = 0;
        Asset();

        Asset(const Asset& other) = delete;
        Asset(Asset&& other) noexcept;
        Asset& operator=(const Asset& rhs) = delete;
        Asset& operator=(Asset&& rhs) noexcept;

        virtual const Guid& GetId() const = 0;
        virtual const std::string& GetName() const = 0;
        virtual void SetName(const std::string& value) = 0;
        virtual void Dump(FileWriter& fileWriter) const = 0;

        bool operator==(const Asset& rhs) const;
        bool operator!=(const Asset& rhs) const;
    };
}
