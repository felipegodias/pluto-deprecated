#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <string>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class ShaderAsset;
    class TextureAsset;
    class FileStreamReader;
    class Vector2I;
    class Vector2F;
    class Vector3I;
    class Vector3F;
    class Vector4I;
    class Vector4F;
    class Matrix2X2;
    class Matrix3X3;
    class Matrix4X4;

    /*
     * File layout in disk. (Version 1)
     * +--------------+------+------------------------------+
     * | Type         | Size | Description                  |
     * +--------------+------+------------------------------+
     * | GUID         | 16   | File signature.              |
     * | uint8_t      | 1    | Serializer version.          |
     * | uint8_t      | 1    | Asset type.                  |
     * | GUID         | 16   | Asset unique identifier.     |
     * | uint8_t      | 1    | Asset name length.           |
     * | string       | *    | Asset name.                  |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API MaterialAsset final : public Asset
    {
    public:
        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<MaterialAsset> Create(const Resource<ShaderAsset>& shaderAsset) const;
            std::unique_ptr<MaterialAsset> Create(const MaterialAsset& original) const;
            std::unique_ptr<Asset> Create(StreamReader& reader) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~MaterialAsset() override;
        explicit MaterialAsset(std::unique_ptr<Impl> impl);

        MaterialAsset(const MaterialAsset& other) = delete;
        MaterialAsset(MaterialAsset&& other) noexcept;
        MaterialAsset& operator=(const MaterialAsset& rhs) = delete;
        MaterialAsset& operator=(MaterialAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        void Dump(FileStreamWriter& fileWriter) const override;

        Resource<ShaderAsset> GetShader() const;
        void SetShader(const Resource<ShaderAsset>& value);

        bool GetBool(const std::string& propertyName) const;
        void SetBool(const std::string& propertyName, bool value);

        int GetInt(const std::string& propertyName) const;
        void SetInt(const std::string& propertyName, int value);

        float GetFloat(const std::string& propertyName) const;
        void SetFloat(const std::string& propertyName, float value);

        Vector2I GetVector2I(const std::string& propertyName) const;
        void SetVector2I(const std::string& propertyName, const Vector2I& value);

        Vector2F GetVector2F(const std::string& propertyName) const;
        void SetVector2F(const std::string& propertyName, const Vector2F& value);

        Vector3I GetVector3I(const std::string& propertyName) const;
        void SetVector3I(const std::string& propertyName, const Vector3I& value);

        Vector3F GetVector3F(const std::string& propertyName) const;
        void SetVector3F(const std::string& propertyName, const Vector3F& value);

        Vector4I GetVector4I(const std::string& propertyName) const;
        void SetVector4I(const std::string& propertyName, const Vector4I& value);

        const Vector4F& GetVector4F(const std::string& propertyName) const;
        void SetVector4F(const std::string& propertyName, const Vector4F& value);

        const Matrix4X4& GetMatrix4X4(const std::string& propertyName) const;
        void SetMatrix4X4(const std::string& propertyName, const Matrix4X4& value);

        Resource<TextureAsset> GetTexture(const std::string& propertyName) const;
        void SetTexture(const std::string& propertyName, const Resource<TextureAsset>& textureAsset);
    };
}
