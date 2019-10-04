#pragma once

#include "pluto/api.h"
#include "pluto/service/base_factory.h"
#include <memory>

namespace pluto
{
    template <typename T, typename Enable = void>
    class Resource;

    class GameObject;

    class Guid;
    class Vector2F;
    class Physics2DShape;
    class Physics2DCircleShape;
    class Physics2DBoxShape;

    class PLUTO_API Physics2DBody final
    {
    public:
        enum class Type
        {
            Static = 0,
            Kinematic = 1,
            Dynamic = 2,
            Default = Static
        };

        class PLUTO_API Factory final : public BaseFactory
        {
        public:
            explicit Factory(ServiceCollection& serviceCollection);
            std::unique_ptr<Physics2DBody> Create(Resource<GameObject> gameObject, const Vector2F& position,
                                                  float angle) const;
        };

    private:
        class Impl;
        std::unique_ptr<Impl> impl;

    public:
        ~Physics2DBody();

        explicit Physics2DBody(std::unique_ptr<Impl> impl);

        Physics2DBody(const Physics2DBody& other) = delete;
        Physics2DBody(Physics2DBody&& other) noexcept;
        Physics2DBody& operator=(const Physics2DBody& rhs) = delete;
        Physics2DBody& operator=(Physics2DBody&& rhs) noexcept;

        float GetDensity() const;
        void SetDensity(float value);

        Type GetType(Type type) const;
        void SetType(Type value);

        float GetGravityScale() const;
        void SetGravityScale(float value);

        Vector2F GetPosition() const;
        void SetPosition(const Vector2F& value);

        float GetAngle() const;
        void SetAngle(float value);

        Vector2F GetVelocity() const;
        void SetVelocity(const Vector2F& value);

        float GetAngularVelocity() const;
        void SetAngularVelocity(float value);

        void AddForce(const Vector2F& force);
        void AddForce(const Vector2F& force, const Vector2F& point);
        void AddTorque(float torque);

        std::unique_ptr<Physics2DCircleShape> CreateCircleShape(const Guid& colliderId, const Vector2F& offset,
                                                                float radius);
        std::unique_ptr<Physics2DBoxShape> CreateBoxShape(const Guid& colliderId, const Vector2F& offset,
                                                          const Vector2F& size);

        void* GetNativeBody() const;
    };
}
