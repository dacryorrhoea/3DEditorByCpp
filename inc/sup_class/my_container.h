#pragma once

#include <vector>
#include <utility>

template<typename T>
class MyContainer
{
protected:
    std::vector<T> storage;

public:
    using value_type = T;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

public:
    MyContainer() = default;
    virtual ~MyContainer() = default;

    virtual void Add(const T& value)
    {
        // custom logic

        storage.push_back(value);
    }

    virtual void Add(T&& value)
    {
        // custom logic

        storage.push_back(std::move(value));
    }

    void Remove(size_t index)
    {
        storage.erase(storage.begin() + index);
    }

    T& operator[](size_t index)
    {
        return storage[index];
    }

    const T& operator[](size_t index) const
    {
        return storage[index];
    }

    size_t Size() const
    {
        return storage.size();
    }

    bool Empty() const
    {
        return storage.empty();
    }

    iterator begin()
    {
        return storage.begin();
    }

    iterator end()
    {
        return storage.end();
    }

    const_iterator begin() const
    {
        return storage.begin();
    }

    const_iterator end() const
    {
        return storage.end();
    }
};