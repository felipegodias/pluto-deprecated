#pragma once

#include "pluto/service/base_service.h"
#include "pluto/service/base_factory.h"
#include <memory>

namespace pluto
{
    class Vector2I;

    class PLUTO_API WindowManager final : public BaseService
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& diContainer);
            std::unique_ptr<WindowManager> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        explicit WindowManager(std::unique_ptr<Impl> impl);
        ~WindowManager();

        bool IsOpen() const;
        void Close();

        const Vector2I& GetWindowSize() const;
        void SetWindowSize(Vector2I value);

        float GetWindowAspectRatio() const;

        void* GetNativeWindow() const;
    };
}
