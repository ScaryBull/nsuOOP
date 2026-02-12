#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{
public:
    static T& Instance()
    {
        static T instance;
        return instance;
    }
protected:
    Singleton() {}
    ~Singleton() {}
private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
};

#endif
