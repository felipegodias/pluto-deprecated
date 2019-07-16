#include <pluto/asset/material_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/events/on_asset_unload_event.h>
#include <pluto/asset/asset_manager.h>
#include <pluto/file/path.h>

#include <pluto/di/di_container.h>

#include <pluto/event/event_manager.h>

#include <pluto/math/vector2f.h>
#include <pluto/math/vector2i.h>
#include <pluto/math/vector3f.h>
#include <pluto/math/vector3i.h>
#include <pluto/math/vector4f.h>
#include <pluto/math/vector4i.h>

#include <pluto/math/matrix2x2.h>
#include <pluto/math/matrix3x3.h>
#include <pluto/math/matrix4x4.h>

#include <pluto/guid.h>
#include <pluto/exception.h>

#include <fmt/format.h>
#include <unordered_map>

namespace pluto
{
    class MaterialAsset::Impl
    {
    private:
        Guid guid;
        std::string name;
        ShaderAsset& shaderAsset;
        std::unordered_map<std::string, float> floats;
        std::unordered_map<std::string, Vector4F> vectors;
        std::unordered_map<std::string, Matrix4X4> matrices;

        Guid onAssetUnloadListenerId;
        EventManager& eventManager;
        AssetManager& assetManager;

    public:
        Impl(Guid guid, ShaderAsset& shaderAsset, EventManager& eventManager, AssetManager& assetManager) :
            guid(std::move(guid)), shaderAsset(shaderAsset), eventManager(eventManager), assetManager(assetManager)
        {
            onAssetUnloadListenerId = eventManager.Subscribe<OnAssetUnloadEvent>(
                std::bind(&Impl::OnAssetUnload, this, std::placeholders::_1));
        }

        ~Impl()
        {
            eventManager.Unsubscribe<OnAssetUnloadEvent>(onAssetUnloadListenerId);
        }

        const Guid& GetId() const
        {
            return guid;
        }

        const std::string& GetName() const
        {
            return name;
        }

        void SetName(std::string value)
        {
            name = std::move(value);
        }

        void Dump(FileWriter& fileWriter) const
        {
        }

        ShaderAsset& GetShader() const
        {
            return shaderAsset;
        }

        void SetShader(ShaderAsset& value)
        {
            shaderAsset = value;
            UpdateProperties();
        }

        bool GetBool(const std::string& propertyName) const
        {
            return GetProperty<float>(propertyName, floats)->second != 0;
        }

        void SetBool(const std::string& propertyName, const bool value)
        {
            GetProperty<float>(propertyName, floats)->second = value ? 1 : 0;
        }

        int GetInt(const std::string& propertyName) const
        {
            return GetProperty<float>(propertyName, floats)->second;
        }

        void SetInt(const std::string& propertyName, const int value)
        {
            GetProperty<int>(propertyName, floats)->second = value;
        }

        float GetFloat(const std::string& propertyName) const
        {
            return GetProperty<float>(propertyName, floats)->second;
        }

        void SetFloat(const std::string& propertyName, const float value)
        {
            GetProperty<float>(propertyName, floats)->second = value;
        }

        Vector2I GetVector2I(const std::string& propertyName) const
        {
            const Vector4F& v = GetProperty<Vector2I>(propertyName, vectors)->second;
            return {static_cast<int>(v.x), static_cast<int>(v.y)};
        }

        void SetVector2I(const std::string& propertyName, const Vector2I& value)
        {
            GetProperty<Vector2I>(propertyName, vectors)->second = value;
        }

        Vector2F GetVector2F(const std::string& propertyName) const
        {
            return GetProperty<Vector2F>(propertyName, vectors)->second;
        }

        void SetVector2F(const std::string& propertyName, const Vector2F& value)
        {
            GetProperty<Vector2F>(propertyName, vectors)->second = value;
        }

        Vector3I GetVector3I(const std::string& propertyName) const
        {
            const Vector4F& v = GetProperty<Vector3I>(propertyName, vectors)->second;
            return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z)};
        }

        void SetVector3I(const std::string& propertyName, const Vector3I& value)
        {
            GetProperty<Vector3I>(propertyName, vectors)->second = value;
        }

        Vector3F GetVector3F(const std::string& propertyName) const
        {
            return GetProperty<Vector3F>(propertyName, vectors)->second;
        }

        void SetVector3F(const std::string& propertyName, const Vector3F& value)
        {
            GetProperty<Vector3F>(propertyName, vectors)->second = value;
        }

        Vector4I GetVector4I(const std::string& propertyName) const
        {
            const Vector4F& v = GetProperty<Vector4I>(propertyName, vectors)->second;
            return {static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w)};
        }

        void SetVector4I(const std::string& propertyName, const Vector4I& value)
        {
            GetProperty<Vector4I>(propertyName, vectors)->second = value;
        }

        const Vector4F& GetVector4F(const std::string& propertyName) const
        {
            return GetProperty<Vector4F>(propertyName, vectors)->second;
        }

        void SetVector4F(const std::string& propertyName, Vector4F value)
        {
            GetProperty<Vector4F>(propertyName, vectors)->second = std::move(value);
        }

        const Matrix4X4& GetMatrix4X4(const std::string& propertyName) const
        {
            return GetProperty<Matrix4X4>(propertyName, matrices)->second;
        }

        void SetMatrix4X4(const std::string& propertyName, Matrix4X4 value)
        {
            GetProperty<Matrix4X4>(propertyName, matrices)->second = std::move(value);
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            shaderAsset = other.shaderAsset;
        }

    private:
        void OnAssetUnload(const OnAssetUnloadEvent& onAssetUnload)
        {
            if (shaderAsset == onAssetUnload.GetAsset())
            {
                SetShader(assetManager.Load<ShaderAsset>(Path("shaders/pink.glsl")));
            }
        }

        void UpdateProperties()
        {
            for (auto& property : shaderAsset.GetProperties())
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
                    //glUniform4fv(property.id, 1, lastMaterialAsset->GetVector4F(property.name).Data());
                    break;
                default: ;
                }
            }
        }

        template <typename T, typename Map>
        typename Map::const_iterator GetProperty(const std::string& propertyName, const Map& map) const
        {
            auto it = map.find(propertyName);
            if (it == map.end())
            {
                Exception::Throw(std::runtime_error(
                    fmt::format("The {0} property with name {1} not found in material {2}.", typeid(T).name(),
                                propertyName, name)));
            }
            return it;
        }

        template <typename T, typename Map>
        typename Map::iterator GetProperty(const std::string& propertyName, Map& map)
        {
            auto it = map.find(propertyName);
            if (it == map.end())
            {
                Exception::Throw(std::runtime_error(
                    fmt::format("The {0} property with name {1} not found in material {2}.", typeid(T).name(),
                                propertyName, name)));
            }
            return it;
        }
    };

    MaterialAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<MaterialAsset> MaterialAsset::Factory::Create() const
    {
        auto& assetManager = diContainer.GetSingleton<AssetManager>();
        auto& pinkShader = assetManager.Load<ShaderAsset>(Path("shaders/pink.glsl"));
        auto& eventManager = diContainer.GetSingleton<EventManager>();

        return std::make_unique<MaterialAsset>(
            std::make_unique<Impl>(Guid::New(), pinkShader, eventManager, assetManager));
    }

    std::unique_ptr<MaterialAsset> MaterialAsset::Factory::Create(const MaterialAsset& original) const
    {
        auto materialAsset = Create();
        materialAsset->impl->Clone(*original.impl);
        return materialAsset;
    }

    std::unique_ptr<MaterialAsset> MaterialAsset::Factory::Create(FileReader& fileReader) const
    {
        return Create();
    }

    MaterialAsset::MaterialAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    MaterialAsset::MaterialAsset(MaterialAsset&& other) noexcept : impl(std::move(other.impl))
    {
    }

    MaterialAsset::~MaterialAsset() = default;

    MaterialAsset& MaterialAsset::operator=(MaterialAsset&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    const Guid& MaterialAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& MaterialAsset::GetName() const
    {
        return impl->GetName();
    }

    void MaterialAsset::SetName(std::string value)
    {
        impl->SetName(std::move(value));
    }

    void MaterialAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    ShaderAsset& MaterialAsset::GetShader() const
    {
        return impl->GetShader();
    }

    void MaterialAsset::SetShader(ShaderAsset& value)
    {
        impl->SetShader(value);
    }

    bool MaterialAsset::GetBool(const std::string& propertyName) const
    {
        return impl->GetBool(propertyName);
    }

    void MaterialAsset::SetBool(const std::string& propertyName, const bool value)
    {
        impl->SetBool(propertyName, value);
    }

    int MaterialAsset::GetInt(const std::string& propertyName) const
    {
        return impl->GetInt(propertyName);
    }

    void MaterialAsset::SetInt(const std::string& propertyName, const int value)
    {
        impl->SetInt(propertyName, value);
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
        return impl->GetVector2I(propertyName);
    }

    void MaterialAsset::SetVector2I(const std::string& propertyName, const Vector2I& value)
    {
        impl->SetVector2I(propertyName, value);
    }

    Vector2F MaterialAsset::GetVector2F(const std::string& propertyName) const
    {
        return impl->GetVector2F(propertyName);
    }

    void MaterialAsset::SetVector2F(const std::string& propertyName, const Vector2F& value)
    {
        impl->SetVector2F(propertyName, value);
    }

    Vector3I MaterialAsset::GetVector3I(const std::string& propertyName) const
    {
        return impl->GetVector3I(propertyName);
    }

    void MaterialAsset::SetVector3I(const std::string& propertyName, const Vector3I& value)
    {
        impl->SetVector3I(propertyName, value);
    }

    Vector3F MaterialAsset::GetVector3F(const std::string& propertyName) const
    {
        return impl->GetVector3F(propertyName);
    }

    void MaterialAsset::SetVector3F(const std::string& propertyName, const Vector3F& value)
    {
        impl->SetVector3F(propertyName, value);
    }

    Vector4I MaterialAsset::GetVector4I(const std::string& propertyName) const
    {
        return impl->GetVector4I(propertyName);
    }

    void MaterialAsset::SetVector4I(const std::string& propertyName, const Vector4I& value)
    {
        impl->SetVector4I(propertyName, value);
    }

    const Vector4F& MaterialAsset::GetVector4F(const std::string& propertyName) const
    {
        return impl->GetVector4F(propertyName);
    }

    void MaterialAsset::SetVector4F(const std::string& propertyName, Vector4F value)
    {
        impl->SetVector4F(propertyName, std::move(value));
    }

    const Matrix4X4& MaterialAsset::GetMatrix4X4(const std::string& propertyName) const
    {
        return impl->GetMatrix4X4(propertyName);
    }

    void MaterialAsset::SetMatrix4X4(const std::string& propertyName, Matrix4X4 value)
    {
        impl->SetMatrix4X4(propertyName, std::move(value));
    }
}
