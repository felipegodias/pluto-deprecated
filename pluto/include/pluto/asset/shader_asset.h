#pragma once

#include "asset.h"
#include "pluto/service/base_factory.h"

#include <memory>
#include <string>
#include <vector>

namespace pluto
{
    class FileReader;
    class ShaderProgram;

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
     * | uint8_t      | 1    | Blend function.              |
     * | uint8_t      | 1    | Source blend factor.         |
     * | uint8_t      | 1    | Destination blend factor.    |
     * | uint8_t      | 1    | ZTest.                       |
     * | uint8_t      | 1    | Culling mode.                |
     * | uint32_t     | 4    | Program binary format.       |
     * | uint32_t     | 4    | Program binary size.         |
     * | uint8_t[]    | *    | Program binary bytes.        |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API ShaderAsset final : public Asset
    {
    public:
        enum class BlendEquation
        {
            Off = 0,
            Add = 1,
            Subtract = 2,
            ReverseSubtract = 3,
            Min = 4,
            Max = 5,
            Default = Off,
            Last = Max,
            Count = Last + 1
        };

        enum class BlendFactor
        {
            Zero = 0,
            One = 1,
            SrcColor = 2,
            OneMinusSrcColor = 3,
            DstColor = 4,
            OneMinusDstColor = 5,
            SrcAlpha = 6,
            OneMinusSrcAlpha = 7,
            DstAlpha = 8,
            OneMinusDstAlpha = 9,
            Default = Zero,
            Last = OneMinusDstAlpha,
            Count = Last + 1
        };

        enum class DepthTest
        {
            Off = 0,
            Always = 1,
            Never = 2,
            Less = 3,
            Equal = 4,
            LessEqual = 5,
            Greater = 6,
            NotEqual = 7,
            GreaterEqual = 8,
            Default = LessEqual,
            Last = GreaterEqual,
            Count = Last + 1
        };

        enum class CullFace
        {
            Off = 0,
            Front = 1,
            Back = 2,
            Default = Front,
            Last = Back,
            Count = Last + 1
        };

        class Property
        {
        public:
            enum class Type
            {
                Bool = 0,
                Int = 1,
                Float = 2,
                Vector2I = 3,
                Vector2F = 4,
                Vector3I = 5,
                Vector3F = 6,
                Vector4I = 7,
                Vector4F = 8,
                Matrix2X2 = 9,
                Matrix3X3 = 10,
                Matrix4X4 = 11,
                Sampler2D = 12,
                Last = Sampler2D,
                Count = Last + 1
            };

            uint8_t id;
            std::string name;
            Type type;
        };

        class PLUTO_API Factory final : public Asset::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<ShaderAsset> Create(BlendEquation blendEquation, BlendEquation blendAlphaEquation,
                                                BlendFactor blendSrcFactor, BlendFactor blendDstFactor,
                                                BlendFactor blendSrcAlphaFactor, BlendFactor blendDstAlphaFactor,
                                                DepthTest depthTest, CullFace cullFace,
                                                const std::vector<Property>& attributes,
                                                const std::vector<Property>& uniforms, uint32_t binaryFormat,
                                                const std::vector<uint8_t>& binaryData) const;

            std::unique_ptr<Asset> Create(Reader& reader) const override;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~ShaderAsset() override;
        explicit ShaderAsset(std::unique_ptr<Impl> impl);

        ShaderAsset(const ShaderAsset& other) = delete;
        ShaderAsset(ShaderAsset&& other) noexcept;
        ShaderAsset& operator=(const ShaderAsset& rhs) = delete;
        ShaderAsset& operator=(ShaderAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        void Dump(FileWriter& fileWriter) const override;

        BlendEquation GetBlendEquation() const;

        BlendEquation GetBlendAlphaEquation() const;

        BlendFactor GetBlendSrcFactor() const;

        BlendFactor GetBlendDstFactor() const;

        BlendFactor GetBlendSrcAlphaFactor() const;

        BlendFactor GetBlendDstAlphaFactor() const;

        DepthTest GetDepthTest() const;

        CullFace GetCullFace() const;

        const std::vector<Property>& GetUniforms() const;

        const std::vector<Property>& GetAttributes() const;

        uint32_t GetBinaryFormat() const;

        const std::vector<uint8_t>& GetBinaryData() const;

        ShaderProgram& GetShaderProgram();
    };
}
