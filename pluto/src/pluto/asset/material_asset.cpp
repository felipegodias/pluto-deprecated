#include <pluto/asset/material_asset.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/asset/events/on_asset_unload_event.h>
#include <pluto/asset/asset_manager.h>

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

#include <unordered_map>

namespace pluto
{
    class MaterialAsset::Impl
    {
    private:
        class BaseProperty
        {
        public:
            virtual ~BaseProperty() = default;
        };

        template <typename T>
        class Property final : public BaseProperty
        {
        public:
            T data;

            explicit Property(T data) : data(std::move(data))
            {
            }
        };

        Guid guid;
        std::string name;
        ShaderAsset& shaderAsset;
        std::unordered_map<std::string, BaseProperty> properties;

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
        }

        bool HasProperty(const std::string& propertyName) const
        {
            return properties.find(propertyName) != properties.end();
        }

        void RemoveProperty(const std::string& propertyName)
        {
            const auto it = properties.find(propertyName);
            if (properties.find(propertyName) != properties.end())
            {
                properties.erase(it);
            }
        }

        bool GetBool(const std::string& propertyName) const
        {
            return GetProperty<bool>(propertyName);
        }

        void SetBool(const std::string& propertyName, const bool value)
        {
            SetProperty<bool>(propertyName, value);
        }

        int GetInt(const std::string& propertyName) const
        {
            return GetProperty<int>(propertyName);
        }

        void SetInt(const std::string& propertyName, const int value)
        {
            SetProperty<int>(propertyName, value);
        }

        float GetFloat(const std::string& propertyName) const
        {
            return GetProperty<float>(propertyName);
        }

        void SetFloat(const std::string& propertyName, const float value)
        {
            SetProperty<float>(propertyName, value);
        }

        const Vector2I& GetVector2I(const std::string& propertyName) const
        {
            return GetProperty<const Vector2I&>(propertyName);
        }

        void SetVector2I(const std::string& propertyName, Vector2I value)
        {
            SetProperty<Vector2I>(propertyName, std::move(value));
        }

        const Vector2F& GetVector2F(const std::string& propertyName) const
        {
            return GetProperty<const Vector2F&>(propertyName);
        }

        void SetVector2F(const std::string& propertyName, Vector2F value)
        {
            SetProperty<Vector2F>(propertyName, std::move(value));
        }

        const Vector3I& GetVector3I(const std::string& propertyName) const
        {
            return GetProperty<const Vector3I&>(propertyName);
        }

        void SetVector3I(const std::string& propertyName, Vector3I value)
        {
            SetProperty<Vector3I>(propertyName, std::move(value));
        }

        const Vector3F& GetVector3F(const std::string& propertyName) const
        {
            return GetProperty<const Vector3F&>(propertyName);
        }

        void SetVector3F(const std::string& propertyName, Vector3F value)
        {
            SetProperty<Vector3F>(propertyName, std::move(value));
        }

        const Vector4I& GetVector4I(const std::string& propertyName) const
        {
            return GetProperty<const Vector4I&>(propertyName);
        }

        void SetVector4I(const std::string& propertyName, Vector4I value)
        {
            SetProperty<Vector4I>(propertyName, std::move(value));
        }

        const Vector4F& GetVector4F(const std::string& propertyName) const
        {
            return GetProperty<const Vector4F&>(propertyName);
        }

        void SetVector4F(const std::string& propertyName, Vector4F value)
        {
            SetProperty<Vector4F>(propertyName, std::move(value));
        }

        const Matrix2X2& GetMatrix2X2(const std::string& propertyName) const
        {
            return GetProperty<const Matrix2X2&>(propertyName);
        }

        void SetMatrix2X2(const std::string& propertyName, Matrix2X2 value)
        {
            SetProperty<Matrix2X2>(propertyName, std::move(value));
        }

        const Matrix3X3& GetMatrix3X3(const std::string& propertyName) const
        {
            return GetProperty<const Matrix3X3&>(propertyName);
        }

        void SetMatrix3X3(const std::string& propertyName, Matrix3X3 value)
        {
            SetProperty<Matrix3X3>(propertyName, std::move(value));
        }

        const Matrix4X4& GetMatrix4X4(const std::string& propertyName) const
        {
            return GetProperty<const Matrix4X4&>(propertyName);
        }

        void SetMatrix4X4(const std::string& propertyName, Matrix4X4 value)
        {
            SetProperty<Matrix4X4>(propertyName, std::move(value));
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            shaderAsset = other.shaderAsset;
            properties = other.properties;
        }

    private:
        void OnAssetUnload(const OnAssetUnloadEvent& onAssetUnload)
        {
            if (shaderAsset == onAssetUnload.GetAsset())
            {
                shaderAsset = assetManager.Load<ShaderAsset>("assets/pink_shader.hlsl");
            }
        }

        template <typename T>
        T GetProperty(const std::string& propertyName) const
        {
            const auto it = properties.find(propertyName);
            if (it == properties.end())
            {
                Exception::Throw(std::runtime_error(""));
            }

            auto property = dynamic_cast<const Property<T>*>(&it->second);
            if (property == nullptr)
            {
                Exception::Throw(std::runtime_error(""));
            }

            return property->data;
        }

        template <typename T>
        void SetProperty(const std::string& propertyName, T value)
        {
            const auto it = properties.find(propertyName);
            if (it == properties.end())
            {
                Property<T> property(std::move(value));
                properties.emplace(propertyName, property);
            }
            else
            {
                const auto propertyPtr = dynamic_cast<Property<T>*>(&it->second);
                if (propertyPtr == nullptr)
                {
                    properties.erase(it);
                    Property<T> property(std::move(value));
                    properties.emplace(propertyName, property);
                }
                else
                {
                    propertyPtr->data = std::move(value);
                }
            }
        }
    };

    MaterialAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<MaterialAsset> MaterialAsset::Factory::Create() const
    {
        auto& assetManager = diContainer.GetSingleton<AssetManager>();
        auto& pinkShader = assetManager.Load<ShaderAsset>("assets/pink_shader.hlsl");
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

    bool MaterialAsset::HasProperty(const std::string& propertyName) const
    {
        return impl->GetBool(propertyName);
    }

    void MaterialAsset::RemoveProperty(const std::string& propertyName)
    {
        impl->RemoveProperty(propertyName);
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

    const Vector2I& MaterialAsset::GetVector2I(const std::string& propertyName) const
    {
        return impl->GetVector2I(propertyName);
    }

    void MaterialAsset::SetVector2I(const std::string& propertyName, Vector2I value)
    {
        impl->SetVector2I(propertyName, std::move(value));
    }

    const Vector2F& MaterialAsset::GetVector2F(const std::string& propertyName) const
    {
        return impl->GetVector2F(propertyName);
    }

    void MaterialAsset::SetVector2F(const std::string& propertyName, Vector2F value)
    {
        impl->SetVector2F(propertyName, std::move(value));
    }

    const Vector3I& MaterialAsset::GetVector3I(const std::string& propertyName) const
    {
        return impl->GetVector3I(propertyName);
    }

    void MaterialAsset::SetVector3I(const std::string& propertyName, Vector3I value)
    {
        impl->SetVector3I(propertyName, std::move(value));
    }

    const Vector3F& MaterialAsset::GetVector3F(const std::string& propertyName) const
    {
        return impl->GetVector3F(propertyName);
    }

    void MaterialAsset::SetVector3F(const std::string& propertyName, Vector3F value)
    {
        impl->SetVector3F(propertyName, std::move(value));
    }

    const Vector4I& MaterialAsset::GetVector4I(const std::string& propertyName) const
    {
        return impl->GetVector4I(propertyName);
    }

    void MaterialAsset::SetVector4I(const std::string& propertyName, Vector4I value)
    {
        impl->SetVector4I(propertyName, std::move(value));
    }

    const Vector4F& MaterialAsset::GetVector4F(const std::string& propertyName) const
    {
        return impl->GetVector4F(propertyName);
    }

    void MaterialAsset::SetVector4F(const std::string& propertyName, Vector4F value)
    {
        impl->SetVector4F(propertyName, std::move(value));
    }

    const Matrix2X2& MaterialAsset::GetMatrix2X2(const std::string& propertyName) const
    {
        return impl->GetMatrix2X2(propertyName);
    }

    void MaterialAsset::SetMatrix2X2(const std::string& propertyName, Matrix2X2 value)
    {
        impl->SetMatrix2X2(propertyName, std::move(value));
    }

    const Matrix3X3& MaterialAsset::GetMatrix3X3(const std::string& propertyName) const
    {
        return impl->GetMatrix3X3(propertyName);
    }

    void MaterialAsset::SetMatrix3X3(const std::string& propertyName, Matrix3X3 value)
    {
        impl->SetMatrix3X3(propertyName, std::move(value));
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
