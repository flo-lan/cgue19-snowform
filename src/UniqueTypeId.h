#pragma once

#include <stdint.h>

// Credits & Source:
// https://codereview.stackexchange.com/questions/44936/unique-type-id-in-c

class UniqueType
{
private:
    static uint32_t Counter;

public:
    template<typename T> static uint32_t Id()
    {
        static uint32_t Id = Counter++;
        return Id;
    }
};

template<typename T> uint32_t UniqueTypeId()
{
    return UniqueType::Id<T>();
}
