#pragma once

#include <string>
#include <memory>

namespace pluto
{
    class ShaderAsset;
    class Guid;

    std::unique_ptr<ShaderAsset> CreateShaderAsset(const ShaderAsset& other);
    std::unique_ptr<ShaderAsset> CreateShaderAsset(const std::string& path);
    std::unique_ptr<ShaderAsset> LoadShaderAsset(const Guid& guid);
}
