#include <pluto/asset/material_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/texture_asset.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/file/path.h>
#include <pluto/file/file_writer.h>
#include <pluto/file/file_reader.h>

#include <pluto/service/service_collection.h>
#include <pluto/memory/resource.h>

#include <pluto/math/vector2f.h>
#include <pluto/math/vector2i.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>
#include <pluto/math/vector4f.h>
#include <pluto/math/vector4i.h>

#include <pluto/math/matrix4x4.h>

#include <pluto/guid.h>
#include <pluto/exception.h>

#include <fmt/format.h>
#include <unordered_map>
#include <utility>

namespace pluto
{
    class MaterialAsset::Impl
    {
        Guid guid;
        std::string name;
        Resource<ShaderAsset> shaderAsset;
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, Vector4F> vectors;
        std::unordered_map<std::string, Matrix4X4> matrices;
        std::unordered_map<std::string, Resource<TextureAsset>> textures;

    public:
        Impl(const Guid& guid, Resource<ShaderAsset> shaderAsset)
            : guid(guid),
              shaderAsset(std::move(shaderAsset))
        {
            UpdateProperties();
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(const std::string& value)
        {
            name = value;
        }

        void Dump(FileWriter& fileWriter) const
        {
            fileWriter.Write(&Guid::PLUTO_IDENTIFIER, sizeof(Guid));
            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));
            auto assetType = static_cast<uint8_t>(Type::Material);
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            Guid shaderGuid = shaderAsset.GetObjectId();
            fileWriter.Write(&shaderGuid, sizeof(Guid));

            uint8_t floatsCount = floats.size();
            fileWriter.Write(&floatsCount, sizeof(uint8_t));
            for (const auto& it : floats)
            {
                uint8_t uniformNameLength = it.first.size();
                fileWriter.Write(&uniformNameLength, sizeof(uint8_t));
                fileWriter.Write(it.first.data(), uniformNameLength);
                fileWriter.Write(&it.second, sizeof(float));
            }

            uint8_t vectorsCount = vectors.size();
            fileWriter.Write(&vectorsCount, sizeof(uint8_t));
            for (const auto& it : vectors)
            {
                uint8_t uniformNameLength = it.first.size();
                fileWriter.Write(&uniformNameLength, sizeof(uint8_t));
                fileWriter.Write(it.first.data(), uniformNameLength);
                fileWriter.Write(&it.second, sizeof(Vector4F));
            }

            uint8_t matricesCount = matrices.size();
            fileWriter.Write(&matricesCount, sizeof(uint8_t));
            for (const auto& it : matrices)
            {
                uint8_t uniformNameLength = it.first.size();
                fileWriter.Write(&uniformNameLength, sizeof(uint8_t));
                fileWriter.Write(it.first.data(), uniformNameLength);
                fileWriter.Write(&it.second, sizeof(Matrix4X4));
            }

            uint8_t texturesCount = textures.size();
            fileWriter.Write(&texturesCount, sizeof(uint8_t));
            for (const auto& it : textures)
            {
                uint8_t uniformNameLength = it.first.size();
                fileWriter.Write(&uniformNameLength, sizeof(uint8_t));
                fileWriter.Write(it.first.data(), uniformNameLength);
                Guid textureGuid = it.second.GetObjectId();
                fileWriter.Write(&textureGuid, sizeof(Guid));
            }
        }

        Resource<ShaderAsset> GetShader() const
        {
            return shaderAsset;
        }

        void SetShader(const Resource<ShaderAsset>& value)
        {
            shaderAsset = value;
            UpdateProperties();
        }

        float GetFloat(const std::string& propertyName) const
        {
            const auto it = floats.find(propertyName);
            if (it == floats.end())
            {
                Exception::Throw(std::runtime_error(
                    fmt::format("The {0} property with name {1} not found in material {2}.", typeid(Matrix4X4).name(),
                                propertyName, name)));
            }
            return it->second;
        }

        void SetFloat(const std::string& propertyName, const float value)
        {
            const auto it = floats.find(propertyName);
            if (it != floats.end())
            {
                it->second = value;
                return;
            }

            floats.emplace(propertyName, value);
        }

        const Vector4F& GetVector4F(const std::string& propertyName) const
        {
            const auto it = vectors.find(propertyName);
            if (it == vectors.end())
            {
                Exception::Throw(std::runtime_error(
                    fmt::format("The {0} property with name {1} not found in material {2}.", typeid(Vector4F).name(),
                                propertyName, name)));
            }
            return it->second;
        }

        void SetVector4F(const std::string& propertyName, const Vector4F& value)
        {
            auto it = vectors.find(propertyName);
            if (it != vectors.end())
            {
                it->second = value;
                return;
            }

            vectors.emplace(propertyName, value);
        }

        const Matrix4X4& GetMatrix4X4(const std::string& propertyName) const
        {
            const auto it = matrices.find(propertyName);
            if (it == matrices.end())
            {
                Exception::Throw(std::runtime_error(
                    fmt::format("The {0} property with name {1} not found in material {2}.", typeid(Matrix4X4).name(),
                                propertyName, name)));
            }
            return it->second;
        }

        void SetMatrix4X4(const std::string& propertyName, const Matrix4X4& value)
        {
            auto it = matrices.find(propertyName);
            if (it != matrices.end())
            {
                it->second = value;
                return;
            }

            matrices.emplace(propertyName, value);
        }

        Resource<TextureAsset> GetTexture(const std::string& propertyName) const
        {
            const auto it = textures.find(propertyName);
            if (it == textures.end())
            {
                Exception::Throw(std::runtime_error(
                    fmt::format("The {0} property with name {1} not found in material {2}.",
                                typeid(TextureAsset).name(),
                                propertyName, name)));
            }
            return it->second;
        }

        void SetTexture(const std::string& propertyName, const Resource<TextureAsset>& textureAsset)
        {
            auto it = textures.find(propertyName);
            if (it != textures.end())
            {
                it->second = textureAsset;
                return;
            }

            textures.emplace(propertyName, textureAsset);
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            shaderAsset = other.shaderAsset;
        }

    private:
        void UpdateProperties()
        {
            if (shaderAsset == nullptr)
            {
                return;
            }

            for (auto& property : shaderAsset->GetUniforms())
            {
                switch (property.type)
                {
                case ShaderAsset::Property::Type::Bool:
                case ShaderAsset::Property::Type::Int:
                case ShaderAsset::Property::Type::Float:
                    floats.emplace(property.name, 0);
                    break;
                case ShaderAsset::Property::Type::Vector2I:
                case ShaderAsset::Property::Type::Vector2F:
                case ShaderAsset::Property::Type::Vector3I:
                case ShaderAsset::Property::Type::Vector3F:
                case ShaderAsset::Property::Type::Vector4I:
                case ShaderAsset::Property::Type::Vector4F:
                    vectors.emplace(property.name, Vector4F::ZERO);
                    break;
                case ShaderAsset::Property::Type::Matrix4X4:
                    matrices.emplace(property.name, Matrix4X4::IDENTITY);
                    break;
                case ShaderAsset::Property::Type::Sampler2D:
                    textures.emplace(property.name, nullptr);
                    break;
                default: ;
                }
            }
        }
    };

    MaterialAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<MaterialAsset> MaterialAsset::Factory::Create(const Resource<ShaderAsset>& shaderAsset) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        return std::make_unique<MaterialAsset>(std::make_unique<Impl>(Guid::New(), shaderAsset));
    }

    std::unique_ptr<MaterialAsset> MaterialAsset::Factory::Create(const MaterialAsset& original) const
    {
        auto materialAsset = Create(original.GetShader());
        materialAsset->impl->Clone(*original.impl);
        return materialAsset;
    }

    std::unique_ptr<Asset> MaterialAsset::Factory::Create(FileReader& fileReader) const
    {
        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& assetManager = serviceCollection.GetService<AssetManager>();

        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));

        if (signature != Guid::PLUTO_IDENTIFIER)
        {
            Exception::Throw(
                std::runtime_error("Trying to load a asset but file signature does not match with pluto."));
        }

        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));

        if (assetType != static_cast<uint8_t>(Type::Material))
        {
            Exception::Throw(
                std::runtime_error("Trying to load a material but file is not a material asset."));
        }

        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));
        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);

        Guid shaderGuid;
        fileReader.Read(&shaderGuid, sizeof(Guid));
        Resource<ShaderAsset> shader = assetManager.Load<ShaderAsset>(shaderGuid);

        auto materialAsset = std::make_unique<MaterialAsset>(std::make_unique<Impl>(assetId, shader));
        materialAsset->SetName(assetName);

        uint8_t floatsCount;
        fileReader.Read(&floatsCount, sizeof(uint8_t));
        for (uint8_t i = 0; i < floatsCount; ++i)
        {
            uint8_t uniformNameLength;
            fileReader.Read(&uniformNameLength, sizeof(uint8_t));
            std::string uniformName(uniformNameLength, ' ');
            fileReader.Read(uniformName.data(), uniformNameLength);
            float value;
            fileReader.Read(&value, sizeof(float));
            materialAsset->SetFloat("u_mat." + uniformName, value);
        }

        uint8_t vectorsCount;
        fileReader.Read(&vectorsCount, sizeof(uint8_t));
        for (uint8_t i = 0; i < vectorsCount; ++i)
        {
            uint8_t uniformNameLength;
            fileReader.Read(&uniformNameLength, sizeof(uint8_t));
            std::string uniformName(uniformNameLength, ' ');
            fileReader.Read(uniformName.data(), uniformNameLength);
            Vector4F value;
            fileReader.Read(&value, sizeof(Vector4F));
            materialAsset->SetVector4F("u_mat." + uniformName, value);
        }

        uint8_t matricesCount;
        fileReader.Read(&matricesCount, sizeof(uint8_t));
        for (uint8_t i = 0; i < matricesCount; ++i)
        {
            uint8_t uniformNameLength;
            fileReader.Read(&uniformNameLength, sizeof(uint8_t));
            std::string uniformName(uniformNameLength, ' ');
            fileReader.Read(uniformName.data(), uniformNameLength);
            Matrix4X4 value;
            fileReader.Read(&value, sizeof(Matrix4X4));
            materialAsset->SetMatrix4X4("u_mat." + uniformName, value);
        }

        uint8_t texturesCount;
        fileReader.Read(&texturesCount, sizeof(uint8_t));
        for (uint8_t i = 0; i < texturesCount; ++i)
        {
            uint8_t uniformNameLength;
            fileReader.Read(&uniformNameLength, sizeof(uint8_t));
            std::string uniformName(uniformNameLength, ' ');
            fileReader.Read(uniformName.data(), uniformNameLength);

            Guid textureGuid;
            fileReader.Read(&textureGuid, sizeof(Guid));
            Resource<TextureAsset> texture = assetManager.Load<TextureAsset>(textureGuid);
            materialAsset->SetTexture("u_mat." + uniformName, texture);
        }

        return materialAsset;
    }

    MaterialAsset::~MaterialAsset() = default;

    MaterialAsset::MaterialAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    MaterialAsset::MaterialAsset(MaterialAsset&& other) noexcept = default;

    MaterialAsset& MaterialAsset::operator=(MaterialAsset&& rhs) noexcept = default;

    const Guid& MaterialAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& MaterialAsset::GetName() const
    {
        return impl->GetName();
    }

    void MaterialAsset::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    void MaterialAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    Resource<ShaderAsset> MaterialAsset::GetShader() const
    {
        return impl->GetShader();
    }

    void MaterialAsset::SetShader(const Resource<ShaderAsset>& value)
    {
        impl->SetShader(value);
    }

    bool MaterialAsset::GetBool(const std::string& propertyName) const
    {
        return impl->GetFloat(propertyName) != 0;
    }

    void MaterialAsset::SetBool(const std::string& propertyName, const bool value)
    {
        impl->SetFloat(propertyName, value);
    }

    int MaterialAsset::GetInt(const std::string& propertyName) const
    {
        return impl->GetFloat(propertyName);
    }

    void MaterialAsset::SetInt(const std::string& propertyName, const int value)
    {
        impl->SetFloat(propertyName, value);
    }

    float MaterialAsset::GetFloat(const std::string& propertyName) const
    {
        return impl->GetFloat(propertyName);
    }

    void MaterialAsset::SetFloat(const std::string& propertyName, const float value)
    {
        impl->SetFloat(propertyName, value);
    }

    Vector2I MaterialAsset::GetVector2I(const std::string& propertyName) const
    {
        const Vector4F vec = impl->GetVector4F(propertyName);
        return {static_cast<int>(vec.x), static_cast<int>(vec.y)};
    }

    void MaterialAsset::SetVector2I(const std::string& propertyName, const Vector2I& value)
    {
        impl->SetVector4F(propertyName, Vector4F(value));
    }

    Vector2F MaterialAsset::GetVector2F(const std::string& propertyName) const
    {
        return Vector2F(impl->GetVector4F(propertyName));
    }

    void MaterialAsset::SetVector2F(const std::string& propertyName, const Vector2F& value)
    {
        impl->SetVector4F(propertyName, Vector4F(value));
    }

    Vector3I MaterialAsset::GetVector3I(const std::string& propertyName) const
    {
        const Vector4F vec = impl->GetVector4F(propertyName);
        return {static_cast<int>(vec.x), static_cast<int>(vec.y), static_cast<int>(vec.z)};
    }

    void MaterialAsset::SetVector3I(const std::string& propertyName, const Vector3I& value)
    {
        impl->SetVector4F(propertyName, Vector4F(value));
    }

    Vector3F MaterialAsset::GetVector3F(const std::string& propertyName) const
    {
        return Vector3F(impl->GetVector4F(propertyName));
    }

    void MaterialAsset::SetVector3F(const std::string& propertyName, const Vector3F& value)
    {
        impl->SetVector4F(propertyName, Vector4F(value));
    }

    Vector4I MaterialAsset::GetVector4I(const std::string& propertyName) const
    {
        const Vector4F vec = impl->GetVector4F(propertyName);
        return {static_cast<int>(vec.x), static_cast<int>(vec.y), static_cast<int>(vec.z), static_cast<int>(vec.w)};
    }

    void MaterialAsset::SetVector4I(const std::string& propertyName, const Vector4I& value)
    {
        impl->SetVector4F(propertyName, Vector4F(value));
    }

    const Vector4F& MaterialAsset::GetVector4F(const std::string& propertyName) const
    {
        return impl->GetVector4F(propertyName);
    }

    void MaterialAsset::SetVector4F(const std::string& propertyName, const Vector4F& value)
    {
        impl->SetVector4F(propertyName, value);
    }

    const Matrix4X4& MaterialAsset::GetMatrix4X4(const std::string& propertyName) const
    {
        return impl->GetMatrix4X4(propertyName);
    }

    void MaterialAsset::SetMatrix4X4(const std::string& propertyName, const Matrix4X4& value)
    {
        impl->SetMatrix4X4(propertyName, value);
    }

    Resource<TextureAsset> MaterialAsset::GetTexture(const std::string& propertyName) const
    {
        return impl->GetTexture(propertyName);
    }

    void MaterialAsset::SetTexture(const std::string& propertyName, const Resource<TextureAsset>& textureAsset)
    {
        impl->SetTexture(propertyName, textureAsset);
    }
}
