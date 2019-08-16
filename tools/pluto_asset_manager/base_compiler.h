#pragma once

#include <pluto/guid.h>
#include <pluto/service/base_service.h>

#include <string>
#include <vector>

namespace pluto::compiler
{
    class BaseCompiler : public BaseService
    {
    public:
        struct CompiledAsset
        {
            Guid assetId;
            std::string virtualPath;
        };

        virtual ~BaseCompiler() = 0;
        BaseCompiler();

        BaseCompiler(const BaseCompiler& other) = delete;
        BaseCompiler(BaseCompiler&& other) noexcept;
        BaseCompiler& operator=(const BaseCompiler& rhs) = delete;
        BaseCompiler& operator=(BaseCompiler&& rhs) noexcept;

        virtual std::vector<std::string> GetExtensions() const = 0;
        virtual std::vector<CompiledAsset> Compile(const std::string& input, const std::string& outputDir) const = 0;
    };
}
