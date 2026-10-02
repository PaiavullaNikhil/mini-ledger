#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

template <typename Key, typename Value>
class Index
{
private:
    std::unordered_map<Key, Value> data;

public:
    void insert(
        const Key &key,
        const Value &value)
    {
        data.insert_or_assign(key, value);
    }

    bool contains(
        const Key &key) const
    {
        return data.find(key) != data.end();
    }

    Value *find(
        const Key &key)
    {
        auto it = data.find(key);

        if (it == data.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    const Value *find(
        const Key &key) const
    {
        auto it = data.find(key);

        if (it == data.end())
        {
            return nullptr;
        }

        return &it->second;
    }

    bool erase(
        const Key &key)
    {
        return data.erase(key) > 0;
    }

    std::size_t size() const
    {
        return data.size();
    }

    std::vector<Value> values() const
    {
        std::vector<Value> result;
        result.reserve(data.size());

        for (const auto &[key, value] : data)
        {
            result.push_back(value);
        }

        return result;
    }
};