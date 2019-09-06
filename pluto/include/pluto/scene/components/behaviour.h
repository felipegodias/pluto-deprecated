#pragma once

#pragma once

#include "pluto/scene/components/component.h"

#include <memory>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class GameObject;

    class PLUTO_API Behaviour : public Component
    {
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        virtual ~Behaviour() = 0;

        explicit Behaviour(std::unique_ptr<Impl> impl);

        Behaviour(const Behaviour& other) = delete;
        Behaviour(Behaviour&& other) noexcept;
        Behaviour& operator=(const Behaviour& rhs) = delete;
        Behaviour& operator=(Behaviour&& rhs) noexcept;

    protected:
        explicit Behaviour(const Resource<GameObject>& gameObject);
    };
}
