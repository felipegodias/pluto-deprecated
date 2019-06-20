#pragma once

#include "../di/singleton.h"
#include "../di/base_factory.h"
#include <memory>

namespace pluto
{
    class PLUTO_API WindowManager final : public Singleton
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(DiContainer& diContainer);
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
        size_t GetWidth() const;
        size_t GetHeight() const;
        void SetSize(size_t width, size_t height);
        void* GetNativeWindow() const;
    };
}
