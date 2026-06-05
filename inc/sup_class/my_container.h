#pragma once

#include <vector>
#include <initializer_list>

template<typename T> class MyContainer {
protected:
    std::vector<T> storage;

public:
    MyContainer() = default;

    MyContainer(size_t size) {
        storage.resize(size);
    }

    MyContainer(std::initializer_list<T> list) {
        storage = list;
    }

    void Add(const T& value) {
        storage.push_back(value);
    }

    void Clear() {
        storage.clear();
    }

    void Reserve(size_t size) {
        storage.reserve(size);
    }

    T& operator[](size_t index) {
        return storage[index];
    }

    const T& operator[](size_t index) const {
        return storage[index];
    }

    size_t Size() const {
        return storage.size();
    }

    MyContainer& operator=(const MyContainer&) = default;

    auto begin() {
        return storage.begin();
    }

    auto end() {
        return storage.end();
    }

    auto begin() const {
        return storage.begin();
    }

    auto end() const {
        return storage.end();
    }
};