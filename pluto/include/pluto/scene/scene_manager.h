#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"
#include <memory>
#include <string>

namespace pluto
{
    class Scene;
    class GameObject;

    class PLUTO_API SceneManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<SceneManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~SceneManager();
        explicit SceneManager(std::unique_ptr<Impl> impl);

        SceneManager(const SceneManager& other) = delete;
        SceneManager(SceneManager&& other) noexcept;
        SceneManager& operator=(const SceneManager& rhs) = delete;
        SceneManager& operator=(SceneManager&& rhs) noexcept;

        Scene& GetActiveScene() const;

        void LoadEmptyScene();

        void MainLoop();
    };
}
