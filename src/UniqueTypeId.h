#pragma once

#include <stdint.h>

// Credits & Source:
// https://codereview.stackexchange.com/questions/44936/unique-type-id-in-c

template<typename T> struct UniqueType
{
    static void Id()
    {
    }
};

template<typename T> uint32_t UniqueTypeId()
{
    return reinterpret_cast<uint32_t>(&UniqueType<T>::Id);
}
