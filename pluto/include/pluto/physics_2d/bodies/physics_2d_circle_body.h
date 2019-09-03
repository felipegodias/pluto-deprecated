#pragma once

#include "physics_2d_body.h"
#include "pluto/service/base_factory.h"

#include <memory>

namespace pluto
{
    class PLUTO_API Physics2DCircleBody final : public Physics2DBody
    {
    public:
        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Physics2DCircleBody> Create() const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Physics2DCircleBody() override;
        explicit Physics2DCircleBody(std::unique_ptr<Impl> impl);

        Physics2DCircleBody(const Physics2DCircleBody& other) = delete;
        Physics2DCircleBody(Physics2DCircleBody&& other) noexcept;
        Physics2DCircleBody& operator=(const Physics2DCircleBody& other) = delete;
        Physics2DCircleBody& operator=(Physics2DCircleBody&& other) noexcept;

        float GetRadius() const;
        void SetRadius(float value);
    };
}
