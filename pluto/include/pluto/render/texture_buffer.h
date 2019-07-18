#pragma once

#include "pluto/di/base_factory.h"
#include <memory>

namespace pluto
{
    class TextureAsset;

    class PLUTO_API TextureBuffer
    {
    public:
        class PLUTO_API Factory : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            virtual std::unique_ptr<TextureBuffer> Create(const TextureAsset& textureAsset) const = 0;
        };

        TextureBuffer();
        virtual ~TextureBuffer() = 0;

        TextureBuffer(const TextureBuffer& other) = delete;
        TextureBuffer(TextureBuffer&& other) noexcept;
        TextureBuffer& operator=(const TextureBuffer& rhs) = delete;
        TextureBuffer& operator=(TextureBuffer&& rhs) noexcept;

        virtual void Update() = 0;
    };
}
