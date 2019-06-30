#pragma once

#include "asset.h"
#include "../di/base_factory.h"
#include <memory>
#include <string>
#include <vector>

namespace pluto
{
    class FileReader;

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
     * | uint32_t     | 4    | Program binary size.         |
     * | uint8_t[]    | *    | Program binary bytes.        |
     * +--------------+------+------------------------------+
     */
    class PLUTO_API ShaderAsset final : public Asset
    {
    public:
        enum class BlendFunction
        {
            None = 0,
            Add = 1,
            Subtract = 2,
            ReverseSubtract = 3,
            Min = 4,
            Max = 5,
            Default = None,
            Last = Max
        };

        enum class BlendFactor
        {
            Zero = 0,
            One = 1,
            SrcColor = 2,
            DstColor = 3,
            SrcAlpha = 4,
            DstAlpha = 5,
            OneMinusSrcColor = 6,
            OneMinusDstColor = 7,
            OneMinusSrcAlpha = 8,
            OneMinusDstAlpha = 9,
            Default = Zero,
            Last = OneMinusDstAlpha
        };

        enum class ZTest
        {
            None = 0,
            Never = 1,
            Less = 2,
            LessEqual = 3,
            Equal = 4,
            NotEqual = 5,
            GreaterEqual = 6,
            Greater = 7,
            Always = 8,
            Default = None,
            Last = Always
        };

        enum class CullMode
        {
            Front = 0,
            Back = 1,
            FrontAndBack = 2,
            Default = Front,
            Last = FrontAndBack
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<ShaderAsset> Create() const;
            std::unique_ptr<ShaderAsset> Create(const ShaderAsset& original) const;
            std::unique_ptr<ShaderAsset> Create(std::istream& is) const;
            std::unique_ptr<ShaderAsset> Create(FileReader& fileReader) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit ShaderAsset(std::unique_ptr<Impl> impl);
        ShaderAsset(const ShaderAsset& other) = delete;
        ShaderAsset(ShaderAsset&& other) noexcept;
        ~ShaderAsset() override;

        ShaderAsset& operator=(const ShaderAsset& rhs);
        ShaderAsset& operator=(ShaderAsset&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(std::string name) override;
        void Dump(std::ostream& os) const override;
        void Dump(FileWriter& fileWriter) const override;

        BlendFunction GetBlendFunction() const;
        void SetBlendFunction(BlendFunction value);

        BlendFactor GetSrcBlendFactor() const;
        void SetSrcBlendFactor(BlendFactor value);

        BlendFactor GetDstBlendFactor() const;
        void SetDstBlendFactor(BlendFactor value);

        ZTest GetZTest() const;
        void SetZTest(ZTest value);

        CullMode GetCullMode() const;
        void SetCullMode(CullMode value);

        const std::vector<uint8_t>& GetBinary() const;
        void SetBinary(std::vector<uint8_t> value);
    };
}
