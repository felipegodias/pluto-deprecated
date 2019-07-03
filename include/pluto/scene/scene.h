#pragma once

#include "../di/base_factory.h"

#include <memory>

namespace pluto
{
    class Guid;
    class GameObject;

    class PLUTO_API Scene
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<Scene> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit Scene(std::unique_ptr<Impl> impl);

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) noexcept;

        ~Scene();

        Scene& operator=(const Scene& rhs) = delete;
        Scene& operator=(Scene&& rhs) noexcept;

        const Guid& GetId() const;

        GameObject& GetRootGameObject() const;

        void Destroy();
    };
}
