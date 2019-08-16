#pragma once

namespace pluto
{
    template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>>
    T& ServiceCollection::AddService(std::unique_ptr<T> instance)
    {
        return static_cast<T&>(AddService(typeid(T), std::move(instance)));
    }

    template <typename T, typename ... Args, std::enable_if_t<
                  std::is_base_of_v<BaseService, T> && std::is_constructible_v<T, Args...>, bool>>
    T& ServiceCollection::EmplaceService(Args&& ... args)
    {
        return AddService(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>>
    void ServiceCollection::RemoveService()
    {
        RemoveService(typeid(T));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseService, T>, bool>>
    T& ServiceCollection::GetService() const
    {
        return static_cast<T&>(GetService(typeid(T)));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseFactory, typename T::Factory>, bool>>
    typename T::Factory& ServiceCollection::AddFactory(std::unique_ptr<typename T::Factory> instance)
    {
        return static_cast<typename T::Factory&>(AddFactory(typeid(T), std::move(instance)));
    }

    template <typename T, typename F, typename ... Args, std::enable_if_t<
                  std::is_base_of_v<typename T::Factory, F> && std::is_constructible_v<F, Args...>, bool>>
    F& ServiceCollection::EmplaceFactory(Args&& ... args)
    {
        return AddFactory<T>(std::make_unique<F>(std::forward<Args>(args)...));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseFactory, typename T::Factory>, bool>>
    void ServiceCollection::RemoveFactory()
    {
        RemoveFactory(typeid(T));
    }

    template <typename T, std::enable_if_t<std::is_base_of_v<BaseFactory, typename T::Factory>, bool>>
    typename T::Factory& ServiceCollection::GetFactory() const
    {
        return static_cast<typename T::Factory&>(GetFactory(typeid(T)));
    }
}
