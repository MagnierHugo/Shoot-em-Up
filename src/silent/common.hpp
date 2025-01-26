// common.hpp

/*
    Contains everything like type definitions, some utilities, etc
*/

#pragma once

#include <cstdint>
#include <memory>

#include <SFML/Graphics.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef int i32;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef i8 b8; // NOTE(ame): For some reason the default bool type of C++ is the size of an int. Wtf strousup?

namespace sl
{
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    u64 UUID();
    i32 RandomInt(i32 min, i32 max);

    template<typename ContainerType, typename ElementType>
    bool Contains(const ContainerType& container, const ElementType& element) {
        return std::find(container.begin(), container.end(), element) != container.end();
    }
}
