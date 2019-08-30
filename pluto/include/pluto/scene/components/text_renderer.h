#pragma once

#include "pluto/scene/components/renderer.h"

#include <memory>
#include <string>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class FontAsset;

    class PLUTO_API TextRenderer final : public Renderer
    {
    public:
        enum class Anchor
        {
            UpperLeft = 0,
            UpperCenter = 1,
            UpperRight = 2,
            MiddleLeft = 3,
            MiddleCenter = 4,
            MiddleRight = 5,
            LowerLeft = 6,
            LowerCenter = 7,
            LowerRight = 8,
            Default = MiddleLeft
        };

        class PLUTO_API Factory final : public Component::Factory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Component> Create(const Resource<GameObject>& gameObject) const override;
        };

    private:
        class PLUTO_API Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~TextRenderer() override;
        explicit TextRenderer(std::unique_ptr<Impl> impl);

        TextRenderer(const TextRenderer& other) = delete;
        TextRenderer(TextRenderer&& other) noexcept;
        TextRenderer& operator=(const TextRenderer& rhs) = delete;
        TextRenderer& operator=(TextRenderer&& rhs) noexcept;

        const Guid& GetId() const override;
        const std::string& GetName() const override;
        void SetName(const std::string& value) override;

        Resource<GameObject> GetGameObject() const override;

        Bounds GetBounds() override;

        Resource<MeshAsset> GetMesh() const override;

        Resource<MaterialAsset> GetMaterial() const override;

        const std::string& GetText() const;
        void SetText(const std::string& value);

        Resource<FontAsset> GetFont() const;
        void SetFont(const Resource<FontAsset>& value);

        Anchor GetAnchor() const;
        void SetAnchor(Anchor value);

        void OnUpdate() override;
    };
}
