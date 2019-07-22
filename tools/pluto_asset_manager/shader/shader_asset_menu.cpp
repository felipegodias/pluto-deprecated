#include "shader_asset_menu.h"
#include "shader_asset_manager.h"
#include "../asset_dumper.h"

#include <pluto/guid.h>
#include <pluto/asset/shader_asset.h>
#include <pluto/file/path.h>

#include <fmt/format.h>

#include <iostream>
#include <unordered_map>

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
        {ShaderAsset::Property::Type::Sampler2D, "Sampler2D"},
    };

    ShaderAssetMenu::~ShaderAssetMenu() = default;

    ShaderAssetMenu::ShaderAssetMenu(const std::function<void()>& backCallback) :
        shaderAssetManager(std::make_unique<ShaderAssetManager>()),
        mainMenu("Shader Asset"), manageMenu("Manage Shader Asset")
    {
        mainMenu.AddOption(0, "Cancel", backCallback);
        mainMenu.AddOption(1, "Create", std::bind(&ShaderAssetMenu::OnCreateShaderOptionSelected, this));
        mainMenu.AddOption(2, "Load", std::bind(&ShaderAssetMenu::OnManageShaderOptionSelected, this));

        manageMenu.AddOption(0, "Cancel", std::bind(&ShaderAssetMenu::OnManageShaderCancelOptionSelected, this));
        manageMenu.AddOption(1, "Show Id", std::bind(&ShaderAssetMenu::OnManageShaderShowIdOptionSelected, this));
        manageMenu.AddOption(2, "Show Name", std::bind(&ShaderAssetMenu::OnManageShaderShowNameOptionSelected, this));
        manageMenu.AddOption(3, "Show Blend Function",
                             std::bind(&ShaderAssetMenu::OnManageShaderShowBlendFunctionOptionSelected, this));
        manageMenu.AddOption(4, "Show Source Blend Factor",
                             std::bind(&ShaderAssetMenu::OnManageShaderShowSrcFactorOptionSelected, this));
        manageMenu.AddOption(5, "Show Dest Blend Factor",
                             std::bind(&ShaderAssetMenu::OnManageShaderShowDstFactorOptionSelected, this));
        manageMenu.AddOption(6, "Show Depth Test",
                             std::bind(&ShaderAssetMenu::OnManageShaderShowDepthTestOptionSelected, this));
        manageMenu.AddOption(7, "Show Cull Mode",
                             std::bind(&ShaderAssetMenu::OnManageShaderShowCullModeOptionSelected, this));
        manageMenu.AddOption(8, "Show Properties",
                             std::bind(&ShaderAssetMenu::OnManageShaderShowPropertiesOptionSelected, this));
        manageMenu.AddOption(9, "Show All", std::bind(&ShaderAssetMenu::OnManageShaderShowAllOptionSelected, this));

        currentMenu = &mainMenu;
    }

    const MenuOptions& ShaderAssetMenu::GetCurrentMenuOptions() const
    {
        return *currentMenu;
    }

    void ShaderAssetMenu::OnCreateShaderOptionSelected()
    {
        std::cout << "Enter the shader file path: ";
        std::string input;
        std::cin >> input;

        const Path path(input);
        const auto shaderAsset = shaderAssetManager->Create(path);
        DumpAsset(path.GetDirectory(), *shaderAsset);
    }

    void ShaderAssetMenu::OnManageShaderOptionSelected()
    {
        std::cout << "Enter the shader asset path: ";
        std::string input;
        std::cin >> input;

        currentManagedAsset = shaderAssetManager->Load(Path(input));
        currentMenu = &manageMenu;
    }

    void ShaderAssetMenu::OnManageShaderShowIdOptionSelected()
    {
        std::cout << "Id: " << currentManagedAsset->GetId() << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowNameOptionSelected()
    {
        std::cout << "Name: " << currentManagedAsset->GetName() << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowBlendFunctionOptionSelected()
    {
        std::cout << "Blend Function: " << blendFunctions.at(currentManagedAsset->GetBlendFunction()) << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowSrcFactorOptionSelected()
    {
        std::cout << "Blend Source Factor: " << blendFactors.at(currentManagedAsset->GetSrcBlendFactor()) << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowDstFactorOptionSelected()
    {
        std::cout << "Blend Dest Factor: " << blendFactors.at(currentManagedAsset->GetDstBlendFactor()) << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowDepthTestOptionSelected()
    {
        std::cout << "Depth Test: " << zTests.at(currentManagedAsset->GetZTest()) << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowCullModeOptionSelected()
    {
        std::cout << "Cull Mode: " << cullModes.at(currentManagedAsset->GetCullMode()) << std::endl;
    }

    void ShaderAssetMenu::OnManageShaderShowPropertiesOptionSelected()
    {
        std::cout << "Properties: " << std::endl;
        for (const auto& property : currentManagedAsset->GetProperties())
        {
            std::cout << fmt::format("[Id: {0}; Name: \"{1}\"; Type: {2}]", property.id, property.name,
                                     propertyTypes.at(property.type)) << std::endl;
        }
    }

    void ShaderAssetMenu::OnManageShaderShowAllOptionSelected()
    {
        OnManageShaderShowIdOptionSelected();
        OnManageShaderShowNameOptionSelected();
        OnManageShaderShowBlendFunctionOptionSelected();
        OnManageShaderShowSrcFactorOptionSelected();
        OnManageShaderShowDstFactorOptionSelected();
        OnManageShaderShowDepthTestOptionSelected();
        OnManageShaderShowCullModeOptionSelected();
        OnManageShaderShowPropertiesOptionSelected();
    }

    void ShaderAssetMenu::OnManageShaderCancelOptionSelected()
    {
        currentMenu = &mainMenu;
        currentManagedAsset.reset();
    }
}
