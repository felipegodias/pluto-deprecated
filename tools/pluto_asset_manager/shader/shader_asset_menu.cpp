#include "shader_asset_menu.h"
#include "shader_asset_manager.h"

#include <pluto/guid.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/di/di_container.h>
#include <pluto/file/file_writer.h>

#include <iostream>
#include <unordered_map>
#include <fmt/format.h>

namespace pluto
{
    std::unordered_map<ShaderAsset::BlendFunction, std::string> blendFunctions = {
        {ShaderAsset::BlendFunction::None, "None"},
        {ShaderAsset::BlendFunction::Add, "Add"},
        {ShaderAsset::BlendFunction::Subtract, "Subtract"},
        {ShaderAsset::BlendFunction::ReverseSubtract, "ReverseSubtract"},
        {ShaderAsset::BlendFunction::Min, "Min"},
        {ShaderAsset::BlendFunction::Max, "Max"}
    };

    std::unordered_map<ShaderAsset::BlendFactor, std::string> blendFactors = {
        {ShaderAsset::BlendFactor::Zero, "Zero"},
        {ShaderAsset::BlendFactor::One, "One"},
        {ShaderAsset::BlendFactor::SrcAlpha, "SrcAlpha"},
        {ShaderAsset::BlendFactor::DstColor, "DstColor"},
        {ShaderAsset::BlendFactor::SrcAlpha, "SrcAlpha"},
        {ShaderAsset::BlendFactor::DstAlpha, "DstAlpha"},
        {ShaderAsset::BlendFactor::OneMinusSrcColor, "OneMinusSrcColor"},
        {ShaderAsset::BlendFactor::OneMinusSrcAlpha, "OneMinusSrcAlpha"},
        {ShaderAsset::BlendFactor::OneMinusDstColor, "OneMinusDstColor"},
        {ShaderAsset::BlendFactor::OneMinusDstAlpha, "OneMinusDstAlpha"}
    };

    std::unordered_map<ShaderAsset::ZTest, std::string> zTests = {
        {ShaderAsset::ZTest::None, "None"},
        {ShaderAsset::ZTest::Never, "Never"},
        {ShaderAsset::ZTest::Less, "Less"},
        {ShaderAsset::ZTest::LessEqual, "LessEqual"},
        {ShaderAsset::ZTest::Equal, "Equal"},
        {ShaderAsset::ZTest::NotEqual, "NotEqual"},
        {ShaderAsset::ZTest::Greater, "Greater"},
        {ShaderAsset::ZTest::GreaterEqual, "GreaterEqual"},
        {ShaderAsset::ZTest::Always, "Always"},
    };

    std::unordered_map<ShaderAsset::CullMode, std::string> cullModes = {
        {ShaderAsset::CullMode::Front, "Front"},
        {ShaderAsset::CullMode::Back, "Back"},
        {ShaderAsset::CullMode::FrontAndBack, "FrontAndBack"},
    };

    std::unordered_map<ShaderAsset::Property::Type, std::string> propertyTypes = {
        {ShaderAsset::Property::Type::Bool, "Bool"},
        {ShaderAsset::Property::Type::Int, "Int"},
        {ShaderAsset::Property::Type::Float, "Float"},
        {ShaderAsset::Property::Type::Vector2I, "Vector2I"},
        {ShaderAsset::Property::Type::Vector2F, "Vector2F"},
        {ShaderAsset::Property::Type::Vector3I, "Vector3I"},
        {ShaderAsset::Property::Type::Vector3F, "Vector3F"},
        {ShaderAsset::Property::Type::Vector4I, "Vector4I"},
        {ShaderAsset::Property::Type::Vector4F, "Vector4F"},
        {ShaderAsset::Property::Type::Matrix2X2, "Matrix2X2"},
        {ShaderAsset::Property::Type::Matrix3X3, "Matrix3X3"},
        {ShaderAsset::Property::Type::Matrix4X4, "Matrix4X4"},
    };

    void PrintShaderAssetMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Shader ***" << std::endl;
        std::cout << "1: Create    2: Load    0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void PrintShaderAssetLoadMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Shader Actions ***" << std::endl;
        std::cout << "1: Print guid              2: Print name     3: Print blendFunction    4: Print srcBlendFactor" <<
            std::endl;
        std::cout << "5: Print dstBlendFactor    6: Print zTest    7: Print cullMode         8: Print properties" << std
            ::endl;
        std::cout << "9: Print binaryFormat     10: Print all     11: Clone                  0: Exit" << std::endl;
        std::cout << std::endl;
    }

    void DumpShaderAsset(const ShaderAsset& shaderAsset)
    {
        DiContainer diContainer;
        const FileWriter::Factory fileWriterFactory(diContainer);
        const std::string guidStr = shaderAsset.GetId().Str();
        std::ofstream ofs(guidStr);
        const auto fileWriter = fileWriterFactory.Create(std::move(ofs));
        shaderAsset.Dump(*fileWriter);
        std::cout << "Shader Asset \"" << shaderAsset.GetName() << "\" saved with id " << guidStr << std::endl;
    }

    void CreateShaderMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Create Shader ***" << std::endl;
        std::cout << "Enter the shader file path: ";
        std::string filePath;
        std::cin >> filePath;

        const auto textAsset = CreateShaderAsset(filePath);
        DumpShaderAsset(*textAsset);
    }

    void PrintGuid(const ShaderAsset& shaderAsset)
    {
        std::cout << "Guid: " << shaderAsset.GetId() << std::endl;
    }

    void PrintName(const ShaderAsset& shaderAsset)
    {
        std::cout << "Name: " << shaderAsset.GetName() << std::endl;
    }

    void PrintBlendFunction(const ShaderAsset& shaderAsset)
    {
        std::cout << "Blend Function: " << blendFunctions.at(shaderAsset.GetBlendFunction()) << std::endl;
    }

    void PrintBlendSrcFactor(const ShaderAsset& shaderAsset)
    {
        std::cout << "Blend Src Factor: " << blendFactors.at(shaderAsset.GetSrcBlendFactor()) << std::endl;
    }

    void PrintBlendDstFactor(const ShaderAsset& shaderAsset)
    {
        std::cout << "Blend Dst Factor: " << blendFactors.at(shaderAsset.GetDstBlendFactor()) << std::endl;
    }

    void PrintZTest(const ShaderAsset& shaderAsset)
    {
        std::cout << "Z Test: " << zTests.at(shaderAsset.GetZTest()) << std::endl;
    }

    void PrintCullMode(const ShaderAsset& shaderAsset)
    {
        std::cout << "Cull Mode: " << cullModes.at(shaderAsset.GetCullMode()) << std::endl;
    }

    void PrintProperties(const ShaderAsset& shaderAsset)
    {
        std::cout << "Properties: " << std::endl;
        for (const auto& property : shaderAsset.GetProperties())
        {
            std::cout << fmt::format("[Id: {0}; Name: \"{1}\"; Type: {2}]", property.id, property.name,
                                     propertyTypes.at(property.type)) << std::endl;
        }
    }

    void PrintBinaryFormat(const ShaderAsset& shaderAsset)
    {
        std::cout << "Cull Mode: " << shaderAsset.GetBinaryFormat() << std::endl;
    }

    void PrintAll(const ShaderAsset& shaderAsset)
    {
        PrintGuid(shaderAsset);
        PrintName(shaderAsset);
        PrintBlendFunction(shaderAsset);
        PrintBlendSrcFactor(shaderAsset);
        PrintBlendDstFactor(shaderAsset);
        PrintZTest(shaderAsset);
        PrintCullMode(shaderAsset);
        PrintProperties(shaderAsset);
        PrintBinaryFormat(shaderAsset);
    }

    void CloneShaderAsset(const ShaderAsset& shaderAsset)
    {
        const auto newTextAsset = CreateShaderAsset(shaderAsset);
        DumpShaderAsset(*newTextAsset);
    }

    void LoadShaderMenu()
    {
        std::cout << std::endl;
        std::cout << "*** Load Shader ***" << std::endl;
        std::unique_ptr<ShaderAsset> shaderAsset = nullptr;
        std::cout << "Enter the shader asset guid: ";
        std::string guidStr;
        std::cin >> guidStr;
        try
        {
            const Guid guid(guidStr);
            shaderAsset = LoadShaderAsset(guid);
        }
        catch (std::exception& e)
        {
            std::cout << "Invalid guid or asset does not exist!" << std::endl;
            return;
        }

        int option = 0;
        do
        {
            PrintShaderAssetLoadMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                PrintGuid(*shaderAsset);
                break;
            case 2:
                PrintName(*shaderAsset);
                break;
            case 3:
                PrintBlendFunction(*shaderAsset);
                break;
            case 4:
                PrintBlendSrcFactor(*shaderAsset);
                break;
            case 5:
                PrintBlendDstFactor(*shaderAsset);
                break;
            case 6:
                PrintZTest(*shaderAsset);
                break;
            case 7:
                PrintCullMode(*shaderAsset);
                break;
            case 8:
                PrintProperties(*shaderAsset);
                break;
            case 9:
                PrintBinaryFormat(*shaderAsset);
                break;
            case 10:
                PrintAll(*shaderAsset);
                break;
            case 11:
                CloneShaderAsset(*shaderAsset);
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }

    void ShaderAssetMenu()
    {
        int option = 0;
        do
        {
            PrintShaderAssetMenu();
            std::cout << "Option: ";
            std::cin >> option;
            switch (option)
            {
            case 0:
                break;
            case 1:
                CreateShaderMenu();
                break;
            case 2:
                LoadShaderMenu();
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
            }
        }
        while (option != 0);
    }
}
