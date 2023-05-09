namespace sunset
{
template <class T>
struct Singleton
{
    static T& get()
    {
        static T instance;
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