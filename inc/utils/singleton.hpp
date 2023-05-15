#pragma once

namespace sunset
{
template <class String>
struct Singleton
{
    static String& get()
    {
        static String instance;
        return instance;
    }

protected:
    Singleton()                            = default;
    Singleton(const Singleton&)            = delete;
    Singleton(Singleton&&)                 = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&)      = delete;
};
}
