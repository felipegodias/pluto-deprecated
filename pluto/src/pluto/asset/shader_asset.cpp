#include <pluto/asset/shader_asset.h>
#include <pluto/render/shader_program.h>
#include <pluto/service/service_collection.h>
#include <pluto/file/reader.h>
#include <pluto/file/file_writer.h>
#include <pluto/guid.h>
#include <utility>
#include <vector>

namespace pluto
{
    class ShaderAsset::Impl
    {
        Guid guid;
        std::string name;
        BlendEquation blendEquation;
        BlendEquation blendAlphaEquation;

        BlendFactor blendSrcFactor;
        BlendFactor blendDstFactor;

        BlendFactor blendSrcAlphaFactor;
        BlendFactor blendDstAlphaFactor;

        DepthTest depthTest;
        CullFace cullFace;

        std::vector<Property> attributes;
        std::vector<Property> uniforms;

        uint32_t binaryFormat;
        std::vector<uint8_t> binaryData;

        std::unique_ptr<ShaderProgram> shaderProgram;

    public:
        Impl(const Guid& guid, const BlendEquation blendEquation, const BlendEquation blendAlphaEquation,
             const BlendFactor blendSrcFactor, const BlendFactor blendDstFactor, const BlendFactor blendSrcAlphaFactor,
             const BlendFactor blendDstAlphaFactor, const DepthTest depthTest, const CullFace cullFace,
             std::vector<Property> attributes, std::vector<Property> uniforms, const uint32_t binaryFormat,
             std::vector<uint8_t> binaryData)
            : guid(guid),
              blendEquation(blendEquation),
              blendAlphaEquation(blendAlphaEquation),
              blendSrcFactor(blendSrcFactor),
              blendDstFactor(blendDstFactor),
              blendSrcAlphaFactor(blendSrcAlphaFactor),
              blendDstAlphaFactor(blendDstAlphaFactor),
              depthTest(depthTest),
              cullFace(cullFace),
              attributes(std::move(attributes)),
              uniforms(std::move(uniforms)),
              binaryFormat(binaryFormat),
              binaryData(std::move(binaryData))
        {
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
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t serializerVersion = 1;
            fileWriter.Write(&serializerVersion, sizeof(uint8_t));
            uint8_t assetType = 1;
            fileWriter.Write(&assetType, sizeof(uint8_t));
            fileWriter.Write(&guid, sizeof(Guid));
            uint8_t assetNameLength = name.size();
            fileWriter.Write(&assetNameLength, sizeof(uint8_t));
            fileWriter.Write(name.data(), assetNameLength);

            auto blendFunction = static_cast<uint8_t>(this->blendEquation);
            fileWriter.Write(&blendFunction, sizeof(uint8_t));

            auto blendAlphaEquation = static_cast<uint8_t>(this->blendAlphaEquation);
            fileWriter.Write(&blendAlphaEquation, sizeof(uint8_t));

            auto blendSrcFactor = static_cast<uint8_t>(this->blendSrcFactor);
            fileWriter.Write(&blendSrcFactor, sizeof(uint8_t));

            auto blendDstFactor = static_cast<uint8_t>(this->blendDstFactor);
            fileWriter.Write(&blendDstFactor, sizeof(uint8_t));

            auto blendSrcAlphaFactor = static_cast<uint8_t>(this->blendSrcAlphaFactor);
            fileWriter.Write(&blendSrcAlphaFactor, sizeof(uint8_t));

            auto blendDstAlphaFactor = static_cast<uint8_t>(this->blendDstAlphaFactor);
            fileWriter.Write(&blendDstAlphaFactor, sizeof(uint8_t));

            auto depthTest = static_cast<uint8_t>(this->depthTest);
            fileWriter.Write(&depthTest, sizeof(uint8_t));

            auto cullFace = static_cast<uint8_t>(this->cullFace);
            fileWriter.Write(&cullFace, sizeof(uint8_t));

            uint8_t attributesCount = attributes.size();
            fileWriter.Write(&attributesCount, sizeof(uint8_t));
            for (const auto& attribute : attributes)
            {
                fileWriter.Write(&attribute.id, sizeof(uint8_t));

                uint8_t attributeNameSize = attribute.name.size();
                fileWriter.Write(&attributeNameSize, sizeof(uint8_t));
                fileWriter.Write(attribute.name.data(), attributeNameSize);

                auto attributeType = static_cast<uint8_t>(attribute.type);
                fileWriter.Write(&attributeType, sizeof(uint8_t));
            }

            uint8_t uniformsCount = uniforms.size();
            fileWriter.Write(&uniformsCount, sizeof(uint8_t));
            for (const auto& uniform : uniforms)
            {
                fileWriter.Write(&uniform.id, sizeof(uint8_t));

                uint8_t uniformNameSize = uniform.name.size();
                fileWriter.Write(&uniformNameSize, sizeof(uint8_t));
                fileWriter.Write(uniform.name.data(), uniformNameSize);

                auto uniformType = static_cast<uint8_t>(uniform.type);
                fileWriter.Write(&uniformType, sizeof(uint8_t));
            }

            fileWriter.Write(&binaryFormat, sizeof(uint32_t));

            uint32_t binaryDataSize = binaryData.size();
            fileWriter.Write(&binaryDataSize, sizeof(uint32_t));
            fileWriter.Write(binaryData.data(), binaryDataSize);

            fileWriter.Flush();
        }

        BlendEquation GetBlendEquation() const
        {
            return blendEquation;
        }

        BlendEquation GetBlendAlphaEquation() const
        {
            return blendAlphaEquation;
        }

        BlendFactor GetBlendSrcFactor() const
        {
            return blendSrcFactor;
        }

        BlendFactor GetBlendDstFactor() const
        {
            return blendDstFactor;
        }

        BlendFactor GetBlendSrcAlphaFactor() const
        {
            return blendSrcAlphaFactor;
        }

        BlendFactor GetBlendDstAlphaFactor() const
        {
            return blendDstAlphaFactor;
        }

        DepthTest GetDepthTest() const
        {
            return depthTest;
        }

        CullFace GetCullFace() const
        {
            return cullFace;
        }

        const std::vector<Property>& GetUniforms() const
        {
            return uniforms;
        }

        const std::vector<Property>& GetAttributes() const
        {
            return attributes;
        }

        uint32_t GetBinaryFormat() const
        {
            return binaryFormat;
        }

        const std::vector<uint8_t>& GetBinaryData() const
        {
            return binaryData;
        }

        ShaderProgram& GetShaderProgram()
        {
            return *shaderProgram;
        }

        void SetShaderProgram(std::unique_ptr<ShaderProgram> value)
        {
            shaderProgram = std::move(value);
        }
    };

    ShaderAsset::Factory::Factory(ServiceCollection& serviceCollection)
        : Asset::Factory(serviceCollection)
    {
    }

    std::unique_ptr<ShaderAsset> ShaderAsset::Factory::Create(BlendEquation blendEquation,
                                                              BlendEquation blendAlphaEquation,
                                                              BlendFactor blendSrcFactor, BlendFactor blendDstFactor,
                                                              BlendFactor blendSrcAlphaFactor,
                                                              BlendFactor blendDstAlphaFactor, DepthTest depthTest,
                                                              CullFace cullFace,
                                                              const std::vector<Property>& attributes,
                                                              const std::vector<Property>& uniforms,
                                                              uint32_t binaryFormat,
                                                              const std::vector<uint8_t>& binaryData) const
    {
        auto shaderAsset = std::make_unique<ShaderAsset>(std::make_unique<Impl>(
            Guid::New(), blendEquation, blendAlphaEquation, blendSrcFactor, blendDstFactor, blendSrcAlphaFactor,
            blendDstAlphaFactor, depthTest, cullFace, attributes, uniforms, binaryFormat, binaryData));

        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& shaderProgramFactory = serviceCollection.GetFactory<ShaderProgram>();
        shaderAsset->impl->SetShaderProgram(shaderProgramFactory.Create(*shaderAsset));
        return shaderAsset;
    }

    std::unique_ptr<Asset> ShaderAsset::Factory::Create(Reader& reader) const
    {
        Guid signature;
        reader.Read(&signature, sizeof(Guid));
        uint8_t serializerVersion;
        reader.Read(&serializerVersion, sizeof(uint8_t));
        uint8_t assetType;
        reader.Read(&assetType, sizeof(uint8_t));

        Guid assetId;
        reader.Read(&assetId, sizeof(Guid));

        uint8_t assetNameLength;
        reader.Read(&assetNameLength, sizeof(uint8_t));
        std::string assetName(assetNameLength, ' ');
        reader.Read(assetName.data(), assetNameLength);

        uint8_t blendEquation;
        reader.Read(&blendEquation, sizeof(uint8_t));

        uint8_t blendAlphaEquation;
        reader.Read(&blendAlphaEquation, sizeof(uint8_t));

        uint8_t blendSrcFactor;
        reader.Read(&blendSrcFactor, sizeof(uint8_t));

        uint8_t blendDstFactor;
        reader.Read(&blendDstFactor, sizeof(uint8_t));

        uint8_t blendSrcAlphaFactor;
        reader.Read(&blendSrcAlphaFactor, sizeof(uint8_t));

        uint8_t blendDstAlphaFactor;
        reader.Read(&blendDstAlphaFactor, sizeof(uint8_t));

        uint8_t depthTest;
        reader.Read(&depthTest, sizeof(uint8_t));

        uint8_t cullFace;
        reader.Read(&cullFace, sizeof(uint8_t));

        uint8_t attributesCount;
        reader.Read(&attributesCount, sizeof(uint8_t));
        std::vector<Property> attributes(attributesCount);
        for (int i = 0; i < attributesCount; ++i)
        {
            reader.Read(&attributes[i].id, sizeof(uint8_t));
            uint8_t attributeNameSize;
            reader.Read(&attributeNameSize, sizeof(uint8_t));
            attributes[i].name = std::string(attributeNameSize, ' ');
            reader.Read(attributes[i].name.data(), attributeNameSize);
            uint8_t attributeType;
            reader.Read(&attributeType, sizeof(uint8_t));
            attributes[i].type = static_cast<Property::Type>(attributeType);
        }

        uint8_t uniformsCount;
        reader.Read(&uniformsCount, sizeof(uint8_t));
        std::vector<Property> uniforms(uniformsCount);
        for (int i = 0; i < uniformsCount; ++i)
        {
            reader.Read(&uniforms[i].id, sizeof(uint8_t));
            uint8_t uniformNameSize;
            reader.Read(&uniformNameSize, sizeof(uint8_t));
            uniforms[i].name = std::string(uniformNameSize, ' ');
            reader.Read(uniforms[i].name.data(), uniformNameSize);
            uint8_t uniformType;
            reader.Read(&uniformType, sizeof(uint8_t));
            uniforms[i].type = static_cast<Property::Type>(uniformType);
        }

        uint32_t binaryFormat;
        reader.Read(&binaryFormat, sizeof(uint32_t));

        uint32_t binarySize;
        reader.Read(&binarySize, sizeof(uint32_t));
        std::vector<uint8_t> binaryData(binarySize);
        reader.Read(binaryData.data(), binarySize);

        auto shaderAsset = std::make_unique<ShaderAsset>(std::make_unique<Impl>(
            assetId, static_cast<BlendEquation>(blendEquation), static_cast<BlendEquation>(blendAlphaEquation),
            static_cast<BlendFactor>(blendSrcFactor), static_cast<BlendFactor>(blendDstFactor),
            static_cast<BlendFactor>(blendSrcAlphaFactor), static_cast<BlendFactor>(blendDstAlphaFactor),
            static_cast<DepthTest>(depthTest), static_cast<CullFace>(cullFace), attributes, uniforms,
            binaryFormat, binaryData));

        shaderAsset->SetName(assetName);

        ServiceCollection& serviceCollection = GetServiceCollection();
        auto& shaderProgramFactory = serviceCollection.GetFactory<ShaderProgram>();
        shaderAsset->impl->SetShaderProgram(shaderProgramFactory.Create(*shaderAsset));
        return shaderAsset;
    }

    ShaderAsset::~ShaderAsset() = default;

    ShaderAsset::ShaderAsset(std::unique_ptr<Impl> impl)
        : impl(std::move(impl))
    {
    }

    ShaderAsset::ShaderAsset(ShaderAsset&& other) noexcept = default;

    ShaderAsset& ShaderAsset::operator=(ShaderAsset&& rhs) noexcept = default;

    const Guid& ShaderAsset::GetId() const
    {
        return impl->GetId();
    }

    const std::string& ShaderAsset::GetName() const
    {
        return impl->GetName();
    }

    void ShaderAsset::SetName(const std::string& value)
    {
        impl->SetName(value);
    }

    void ShaderAsset::Dump(FileWriter& fileWriter) const
    {
        impl->Dump(fileWriter);
    }

    ShaderAsset::BlendEquation ShaderAsset::GetBlendEquation() const
    {
        return impl->GetBlendEquation();
    }

    ShaderAsset::BlendEquation ShaderAsset::GetBlendAlphaEquation() const
    {
        return impl->GetBlendAlphaEquation();
    }

    ShaderAsset::BlendFactor ShaderAsset::GetBlendSrcFactor() const
    {
        return impl->GetBlendSrcFactor();
    }

    ShaderAsset::BlendFactor ShaderAsset::GetBlendDstFactor() const
    {
        return impl->GetBlendDstFactor();
    }

    ShaderAsset::BlendFactor ShaderAsset::GetBlendSrcAlphaFactor() const
    {
        return impl->GetBlendSrcAlphaFactor();
    }

    ShaderAsset::BlendFactor ShaderAsset::GetBlendDstAlphaFactor() const
    {
        return impl->GetBlendDstAlphaFactor();
    }

    ShaderAsset::DepthTest ShaderAsset::GetDepthTest() const
    {
        return impl->GetDepthTest();
    }

    ShaderAsset::CullFace ShaderAsset::GetCullFace() const
    {
        return impl->GetCullFace();
    }

    const std::vector<ShaderAsset::Property>& ShaderAsset::GetUniforms() const
    {
        return impl->GetUniforms();
    }

    const std::vector<ShaderAsset::Property>& ShaderAsset::GetAttributes() const
    {
        return impl->GetAttributes();
    }

    uint32_t ShaderAsset::GetBinaryFormat() const
    {
        return impl->GetBinaryFormat();
    }

    const std::vector<uint8_t>& ShaderAsset::GetBinaryData() const
    {
        return impl->GetBinaryData();
    }

    ShaderProgram& ShaderAsset::GetShaderProgram()
    {
        return impl->GetShaderProgram();
    }
}
