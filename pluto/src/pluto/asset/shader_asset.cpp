#include <pluto/asset/shader_asset.h>
#include <pluto/render/shader_program.h>
#include <pluto/di/di_container.h>
#include <pluto/file/file_reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/guid.h>
#include <vector>

namespace pluto
{
    class ShaderAsset::Impl
    {
    private:
        ShaderAsset* instance;

        Guid guid;
        std::string name;
        BlendFunction blendFunction;
        BlendFactor srcBlendFactor;
        BlendFactor dstBlendFactor;
        ZTest zTest;
        CullMode cullMode;
        uint32_t binaryFormat;
        std::vector<Property> properties;
        std::vector<uint8_t> binary;

        bool isDirty;
        std::unique_ptr<ShaderProgram> shaderProgram;

        const ShaderProgram::Factory& shaderProgramFactory;

    public:
        Impl(Guid guid, const ShaderProgram::Factory& shaderProgramFactory) : instance(nullptr), guid(std::move(guid)),
                                                                              blendFunction(BlendFunction::Default),
                                                                              srcBlendFactor(BlendFactor::Default),
                                                                              dstBlendFactor(BlendFactor::Default),
                                                                              zTest(ZTest::Default),
                                                                              cullMode(CullMode::Default),
                                                                              binaryFormat(0), isDirty(true),
                                                                              shaderProgram(nullptr),
                                                                              shaderProgramFactory(shaderProgramFactory)
        {
        }

        void SetInstance(ShaderAsset& value)
        {
            instance = &value;
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
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 1;
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            auto blendFunction = static_cast<uint8_t>(this->blendFunction);
            fileWriter.Write(&blendFunction, sizeof(uint8_t));

            auto srcBlendFactor = static_cast<uint8_t>(this->srcBlendFactor);
            fileWriter.Write(&srcBlendFactor, sizeof(uint8_t));

            auto dstBlendFactor = static_cast<uint8_t>(this->dstBlendFactor);
            fileWriter.Write(&dstBlendFactor, sizeof(uint8_t));

            auto zTest = static_cast<uint8_t>(this->zTest);
            fileWriter.Write(&zTest, sizeof(uint8_t));

            auto cullMode = static_cast<uint8_t>(this->cullMode);
            fileWriter.Write(&cullMode, sizeof(uint8_t));

            uint8_t propertiesCount = properties.size();
            fileWriter.Write(&propertiesCount, sizeof(uint8_t));

            for (const auto& property : properties)
            {
                fileWriter.Write(&property.id, sizeof(uint8_t));

                uint8_t propertyNameSize = property.name.size();
                fileWriter.Write(&propertyNameSize, sizeof(uint8_t));
                fileWriter.Write(property.name.data(), propertyNameSize);

                auto propertyType = static_cast<uint8_t>(property.type);
                fileWriter.Write(&propertyType, sizeof(uint8_t));
            }

            fileWriter.Write(&binaryFormat, sizeof(uint32_t));

            uint32_t binarySize = binary.size();
            fileWriter.Write(&binarySize, sizeof(uint32_t));
            fileWriter.Write(binary.data(), binarySize);

            fileWriter.Flush();
        }

        BlendFunction GetBlendFunction() const
        {
            return blendFunction;
        }

        void SetBlendFunction(const BlendFunction value)
        {
            blendFunction = value;
        }

        BlendFactor GetSrcBlendFactor() const
        {
            return srcBlendFactor;
        }

        void SetSrcBlendFactor(const BlendFactor value)
        {
            srcBlendFactor = value;
        }

        BlendFactor GetDstBlendFactor() const
        {
            return dstBlendFactor;
        }

        void SetDstBlendFactor(const BlendFactor value)
        {
            dstBlendFactor = value;
        }

        ZTest GetZTest() const
        {
            return zTest;
        }

        void SetZTest(const ZTest value)
        {
            zTest = value;
        }

        CullMode GetCullMode() const
        {
            return cullMode;
        }

        void SetCullMode(const CullMode value)
        {
            cullMode = value;
        }

        const std::vector<Property>& GetProperties() const
        {
            return properties;
        }

        void SetProperties(std::vector<Property> value)
        {
            properties = std::move(value);
        }

        uint32_t GetBinaryFormat() const
        {
            return binaryFormat;
        }

        void SetBinaryFormat(const uint32_t value)
        {
            binaryFormat = value;
            isDirty = true;
        }

        const std::vector<uint8_t>& GetBinary() const
        {
            return binary;
        }

        void SetBinary(std::vector<uint8_t> value)
        {
            binary = std::move(value);
            isDirty = true;
        }

        ShaderProgram& GetShaderProgram()
        {
            if (isDirty)
            {
                shaderProgram = shaderProgramFactory.Create(*instance);
                isDirty = false;
            }

            return *shaderProgram;
        }

        void Clone(const Impl& other)
        {
            name = other.name;
            blendFunction = other.blendFunction;
            srcBlendFactor = other.srcBlendFactor;
            dstBlendFactor = other.dstBlendFactor;
            zTest = other.zTest;
            cullMode = other.cullMode;
            properties = other.properties;
            binaryFormat = other.binaryFormat;
            binary = other.binary;
        }
    };

    ShaderAsset::Factory::Factory(DiContainer& diContainer) : BaseFactory(diContainer)
    {
    }

    std::unique_ptr<ShaderAsset> ShaderAsset::Factory::Create() const
    {
        auto& shaderProgramFactory = diContainer.GetSingleton<ShaderProgram::Factory>();
        auto shaderAsset = std::make_unique<ShaderAsset>(std::make_unique<Impl>(Guid::New(), shaderProgramFactory));
        shaderAsset->impl->SetInstance(*shaderAsset);
        return shaderAsset;
    }

    std::unique_ptr<ShaderAsset> ShaderAsset::Factory::Create(const ShaderAsset& original) const
    {
        auto shaderAsset = Create();
        shaderAsset->impl->Clone(*original.impl);
        return shaderAsset;
    }

    std::unique_ptr<ShaderAsset> ShaderAsset::Factory::Create(FileReader& fileReader) const
    {
        Guid signature;
        fileReader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        fileReader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        fileReader.Read(&assetType, sizeof(uint8_t));
        Guid assetId;
        fileReader.Read(&assetId, sizeof(Guid));

        auto& shaderProgramFactory = diContainer.GetSingleton<ShaderProgram::Factory>();
        auto shaderAsset = std::make_unique<ShaderAsset>(std::make_unique<Impl>(assetId, shaderProgramFactory));
        shaderAsset->impl->SetInstance(*shaderAsset);

        uint8_t assetNameLength;
        fileReader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        fileReader.Read(assetName.data(), assetNameLength);
        shaderAsset->SetName(assetName);

        uint8_t blendFunction;
        fileReader.Read(&blendFunction, sizeof(uint8_t));
        shaderAsset->SetBlendFunction(static_cast<BlendFunction>(blendFunction));

        uint8_t srcBlendFactor;
        fileReader.Read(&srcBlendFactor, sizeof(uint8_t));
        shaderAsset->SetSrcBlendFactor(static_cast<BlendFactor>(srcBlendFactor));

        uint8_t dstBlendFactor;
        fileReader.Read(&dstBlendFactor, sizeof(uint8_t));
        shaderAsset->SetDstBlendFactor(static_cast<BlendFactor>(dstBlendFactor));

        uint8_t zTest;
        fileReader.Read(&zTest, sizeof(uint8_t));
        shaderAsset->SetZTest(static_cast<ZTest>(zTest));

        uint8_t cullMode;
        fileReader.Read(&cullMode, sizeof(uint8_t));
        shaderAsset->SetCullMode(static_cast<CullMode>(cullMode));

        uint8_t propertyCount;
        fileReader.Read(&propertyCount, sizeof(uint8_t));
        std::vector<Property> properties(propertyCount);
        for (int i = 0; i < propertyCount; ++i)
        {
            fileReader.Read(&properties[i].id, sizeof(uint8_t));
            uint8_t propertyNameSize;
            fileReader.Read(&propertyNameSize, sizeof(uint8_t));
            properties[i].name = std::string(propertyNameSize, ' ');
            fileReader.Read(properties[i].name.data(), propertyNameSize);
            uint8_t propertyType;
            fileReader.Read(&propertyType, sizeof(uint8_t));
            properties[i].type = static_cast<Property::Type>(propertyType);
        }
        shaderAsset->SetProperties(std::move(properties));

        uint32_t binaryFormat;
        fileReader.Read(&binaryFormat, sizeof(uint32_t));
        shaderAsset->SetBinaryFormat(binaryFormat);

        uint32_t binarySize;
        fileReader.Read(&binarySize, sizeof(uint32_t));
        std::vector<uint8_t> binary(binarySize);
        fileReader.Read(binary.data(), binarySize);
        shaderAsset->SetBinary(std::move(binary));

        return shaderAsset;
    }

    ShaderAsset::ShaderAsset(std::unique_ptr<Impl> impl) : impl(std::move(impl))
    {
    }

    ShaderAsset::ShaderAsset(ShaderAsset&& other) noexcept : ShaderAsset(std::move(other.impl))
    {
    }

    ShaderAsset::~ShaderAsset() = default;

    ShaderAsset& ShaderAsset::operator=(const ShaderAsset& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl->Clone(*rhs.impl);
        return *this;
    }

    ShaderAsset& ShaderAsset::operator=(ShaderAsset&& rhs) noexcept
    {
        if (this == &rhs)
        {
            return *this;
        }

        impl = std::move(rhs.impl);
        return *this;
    }

    const Guid& ShaderAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& ShaderAsset::GetName() const
    {
        return impl->GetName();
    }

    void ShaderAsset::SetName(std::string value)
    {
        impl->SetName(std::move(value));
    }

    void ShaderAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    ShaderAsset::BlendFunction ShaderAsset::GetBlendFunction() const
    {
        return impl->GetBlendFunction();
    }

    void ShaderAsset::SetBlendFunction(const BlendFunction value)
    {
        impl->SetBlendFunction(value);
    }

    ShaderAsset::BlendFactor ShaderAsset::GetSrcBlendFactor() const
    {
        return impl->GetSrcBlendFactor();
    }

    void ShaderAsset::SetSrcBlendFactor(const BlendFactor value)
    {
        impl->SetSrcBlendFactor(value);
    }

    ShaderAsset::BlendFactor ShaderAsset::GetDstBlendFactor() const
    {
        return impl->GetDstBlendFactor();
    }

    void ShaderAsset::SetDstBlendFactor(const BlendFactor value)
    {
        impl->SetDstBlendFactor(value);
    }

    ShaderAsset::ZTest ShaderAsset::GetZTest() const
    {
        return impl->GetZTest();
    }

    void ShaderAsset::SetZTest(const ZTest value)
    {
        impl->SetZTest(value);
    }

    ShaderAsset::CullMode ShaderAsset::GetCullMode() const
    {
        return impl->GetCullMode();
    }

    void ShaderAsset::SetCullMode(const CullMode value)
    {
        impl->SetCullMode(value);
    }

    const std::vector<ShaderAsset::Property>& ShaderAsset::GetProperties() const
    {
        return impl->GetProperties();
    }

    void ShaderAsset::SetProperties(std::vector<Property> value)
    {
        impl->SetProperties(std::move(value));
    }

    uint32_t ShaderAsset::GetBinaryFormat() const
    {
        return impl->GetBinaryFormat();
    }

    void ShaderAsset::SetBinaryFormat(const uint32_t value)
    {
        impl->SetBinaryFormat(value);
    }

    const std::vector<uint8_t>& ShaderAsset::GetBinary() const
    {
        return impl->GetBinary();
    }

    void ShaderAsset::SetBinary(std::vector<uint8_t> value)
    {
        impl->SetBinary(std::move(value));
    }

    ShaderProgram& ShaderAsset::GetShaderProgram()
    {
        return impl->GetShaderProgram();
    }
}
