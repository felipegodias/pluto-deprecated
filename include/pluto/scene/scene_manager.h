#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
#include <memory>

namespace pluto
{
    class Scene;

    class PLUTO_API SceneManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
            std::unique_ptr<SceneManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit SceneManager(std::unique_ptr<Impl> impl);

        SceneManager(const SceneManager& other) = delete;
        SceneManager(SceneManager&& other) noexcept;
        ~SceneManager();

        SceneManager& operator=(const SceneManager& rhs) = delete;
        SceneManager& operator=(SceneManager&& rhs) noexcept;

        Scene& GetActiveScene() const;

        void LoadEmptyScene();
    };
}
