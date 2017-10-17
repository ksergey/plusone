/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_tuple_171017172633
#define KSERGEY_tuple_171017172633

#include <tuple>
#include <type_traits>
#include <plusone/compiler.hpp>

namespace plusone {

/** Key and Value type holder. */
template< class K, class V >
struct pair
{
    using first_type = K;
    using second_type = V;
};

namespace detail {

/* Return index of type in tuple */
template< class T, class Tuple >
struct index_of_impl;
template< class T, class... Ts >
struct index_of_impl< T, std::tuple< T, Ts... > >
    : std::integral_constant< std::size_t, 0 >
{};
template< class T, class U, class... Ts >
struct index_of_impl< T, std::tuple< U, Ts... > >
    : std::integral_constant< std::size_t, 1 + index_of_impl< T, std::tuple< Ts... > >::value >
{};
template< class T >
struct index_of_impl< T, std::tuple<> >
    : std::integral_constant< int, -1 >
{
    static_assert( (sizeof(T) + 1) == 0, "Type T not found in tuple" );
};
template< class T, class Tuple >
constexpr std::size_t index_of = index_of_impl< T, Tuple >::value;

/* Return true if type exists in tuple */
template< class T, class Tuple >
struct exists_impl
    : std::false_type
{};
template< class T, class... Ts >
struct exists_impl< T, std::tuple< T, Ts... > >
    : std::true_type
{};
template< class T, class U, class... Ts >
struct exists_impl< T, std::tuple< U, Ts... > >
    : std::integral_constant< bool, exists_impl< T, std::tuple< Ts... > >::value >
{};
template< class T, class Tuple >
constexpr std::size_t exists = exists_impl< T, Tuple >::value;

/* Return true if tuple contain duplicates types */
template< class... Ts >
struct has_duplicates_impl;
template<>
struct has_duplicates_impl< std::tuple<> >
    : std::false_type
{};
template< class T, class... Ts >
struct has_duplicates_impl< std::tuple< T, Ts...> >
    : std::integral_constant< bool, exists< T, std::tuple< Ts... > > || has_duplicates_impl< std::tuple< Ts... > >::value >
{};
template< class Tuple >
constexpr bool has_duplicates = has_duplicates_impl< Tuple >::value;

/* Return key index */
template< class Key, class TaggedTuple >
constexpr std::size_t key_index = index_of< Key, typename TaggedTuple::keys >;

/* Return type of element at key */
template< class Key, class TaggedTuple >
using type_of = typename std::tuple_element<
    key_index< Key, TaggedTuple >,
    typename TaggedTuple::values
>::type;

template< class Tuple, class F, std::size_t... Indices >
__force_inline constexpr void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence< Indices... >)
{
    using swallow = int[];
    swallow{1, (f(std::get< Indices >(std::forward< Tuple >(tuple))), void(), int{})...};
}

} /* namespace detail */

/**
 * Improved tuple type.
 *
 * Example:
 *  using xtuple = plusone::tagged_tuple<
 *      plusone::pair< struct tag3, int >,
 *      plusone::pair< struct tag4, std::string >,
 *      plusone::pair< struct tag5, std::string >
 *  >;
 *  xtuple test{999, "hello", "zzzz"};
 *  plusone::get< tag3 >(test) = 64;
 *
 *  std::cout
 *      << plusone::get< tag3 >(test) << ' '
 *      << plusone::get< tag4 >(test) << ' '
 *      << plusone::get< tag5 >(test) << '\n';
 */
template< class... Ts >
struct tagged_tuple
    : std::tuple< typename Ts::second_type... >
{
    using keys = std::tuple< typename Ts::first_type... >;
    using values = std::tuple< typename Ts::second_type... >;

    using tuple_type = std::tuple< typename Ts::second_type... >;
    using tuple_type::tuple_type;
    using tuple_type::swap;
    using tuple_type::operator=;
};

/** Access tuple element. */
template< class Key, class... Ts >
__force_inline detail::type_of< Key, tagged_tuple< Ts... > >&
get(tagged_tuple< Ts... >& t)
{
    return std::get< detail::key_index< Key, tagged_tuple< Ts... > > >(t);
}

/** @overload */
template< class Key, class... Ts >
__force_inline const detail::type_of< Key, tagged_tuple< Ts... > >&
get(const tagged_tuple< Ts... >& t)
{
    return std::get< detail::key_index< Key, tagged_tuple< Ts... > > >(t);
}

/** @overload */
template< class Key, class... Ts >
__force_inline const detail::type_of< Key, tagged_tuple< Ts... > >&&
get(const tagged_tuple< Ts... >&& t)
{
    return std::get< detail::key_index< Key, tagged_tuple< Ts... > > >(t);
}

/** Foreach over tuple elements. */
template< typename Tuple, typename F >
__force_inline constexpr void for_each(Tuple&& tuple, F&& f)
{
    constexpr std::size_t N = std::tuple_size< std::remove_reference_t< Tuple > >();
    detail::for_each_impl(std::forward< Tuple >(tuple), std::forward< F >(f), std::make_index_sequence< N >{});
}

} /* namespace plusone */

#endif /* KSERGEY_tuple_171017172633 */
