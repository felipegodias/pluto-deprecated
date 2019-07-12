#pragma once

#include "pluto/di/base_factory.h"
#include <memory>

namespace pluto
{
    class ShaderAsset;

    class PLUTO_API ShaderProgram
    {
    public:
        class PLUTO_API Factory : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            virtual std::unique_ptr<ShaderProgram> Create(const ShaderAsset& shaderAsset) const = 0;
        };

        ShaderProgram();

        ShaderProgram(const ShaderProgram& other) = delete;
        ShaderProgram(ShaderProgram&& other) noexcept;

        virtual ~ShaderProgram() = 0;

        ShaderProgram& operator=(const ShaderProgram& rhs) = delete;
        ShaderProgram& operator=(ShaderProgram&& rhs) noexcept;
    };
}
