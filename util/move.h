#ifndef TINYSTL__MOVE_H
#define TINYSTL__MOVE_H

#include <type_traits>

namespace zxy{
    template<typename T>
    constexpr decltype(auto) move(T&& t) noexcept{
        return static_cast<std::remove_reference_t<T>&&>(t);
    }

    template<typename T>
    constexpr T&& forward(std::remove_reference_t<T>& t) noexcept{
        return static_cast<T&&>(t);
    }//overload 1

    //rvalue->lvalue，触发static_assert
    template<typename T>
    constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept{
        static_assert(!std::is_lvalue_reference<T>::value,
                "template argument substituting T is an lvalue reference type");

        return static_cast<T&&>(t);
    }//overload 2
}

#endif //TINYSTL__MOVE_H
