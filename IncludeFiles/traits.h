// Compiler from Wolfram Language to C++
// 
// Copyright 2019 Tianhuan Lu
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <complex>

#include <type_traits>

namespace wl
{

template<typename T, size_t R>
struct ndarray;

template<typename T, size_t ArrayRank, size_t ViewRank, bool Const>
struct simple_view;

template<typename T, size_t ArrayRank, size_t ViewRank, size_t StrideRank, bool Const>
struct regular_view;

template<typename T, size_t ArrayRank, size_t ViewRank, size_t StrideRank, typename IndexersTuple, bool Const>
struct general_view;

template<typename T>
using complex = std::complex<T>;

union u8string;
using string = u8string;

struct u8string_view;
using string_view = u8string_view;

struct void_type;

struct all_type;

struct list_type;

struct boolean;

template<typename F>
struct function;

template<typename ArgIter, bool HasStride = false>
struct argument_pack;

template<typename Normal, typename Variadic>
struct variadic;

template<typename...>
constexpr auto always_false_v = false;

template<typename...>
struct undefined_type;

template<size_t...>
struct undefined_integral;

template<typename T, size_t...>
struct identity_type
{
    using type = T;
};

template<int64_t I>
struct const_int;

template<int64_t... Is>
struct const_ints;

template<typename T>
struct is_const_int : std::false_type {};

template<int64_t I>
struct is_const_int<const_int<I>> : std::true_type {};

template<typename T>
constexpr bool is_const_int_v = is_const_int<T>::value;

template<int64_t Id, typename Pattern>
struct _named_pattern;

template<int64_t Id>
struct _named_replacement;

template<typename... Head>
struct _pattern_blank;

template<typename... Head>
struct _pattern_blank_sequence;

template<typename... Head>
struct _pattern_blank_null_sequence;

template<typename... Patterns>
struct _pattern_alternatives;

template<typename Pattern>
struct _pattern_repeated;

template<typename Pattern>
struct _pattern_except;

template<typename Pattern>
struct _pattern_longest;

template<typename Pattern>
struct _pattern_shortest;

template<typename Left, typename Right>
struct _pattern_rule;

template<typename... Patterns>
struct _string_expression;

template<typename Pattern, typename Condition>
struct _condition;

template<int64_t... Ids>
struct _pattern_id_list;

namespace strexp
{

template<typename ConditionList, typename PatternIdList_>
struct compiled_pattern;

template<typename CompiledPattern, typename CompiledReplacement>
struct compiled_pattern_rule;

}

enum class stream_mode { Read, Write, Append };

template<stream_mode Mode, bool Binary>
struct file_stream;

template<typename T>
constexpr auto is_integral_v = std::is_integral_v<T>;

template<typename T>
constexpr auto is_float_v = std::is_floating_point_v<T>;

template<typename T>
constexpr auto is_string_v = std::is_same_v<T, string>;

template<typename T>
constexpr auto is_string_view_v =
    std::is_same_v<T, string> || std::is_same_v<T, string_view>;

template<typename T>
constexpr auto is_boolean_v = std::is_same_v<T, boolean>;

template<typename T>
struct is_complex : std::false_type {};
template<typename T>
struct is_complex<complex<T>> : std::true_type {};
template<typename T>
constexpr auto is_complex_v = is_complex<T>::value;

template<typename T>
constexpr auto is_real_v = is_integral_v<T> || is_float_v<T>;

template<typename T>
constexpr auto is_arithmetic_v = is_real_v<T> || is_complex_v<T>;

template<typename T>
struct is_pattern : std::false_type {};

template<int64_t Id, typename Pattern>
struct is_pattern<_named_pattern<Id, Pattern>> : std::true_type {};

template<int64_t Id>
struct is_pattern<_named_replacement<Id>> : std::true_type {};

template<typename... Head>
struct is_pattern<_pattern_blank<Head...>> : std::true_type {};

template<typename... Head>
struct is_pattern<_pattern_blank_sequence<Head...>> : std::true_type {};

template<typename... Head>
struct is_pattern<_pattern_blank_null_sequence<Head...>> : std::true_type {};

template<typename... Patterns>
struct is_pattern<_pattern_alternatives<Patterns...>> : std::true_type {};

template<typename Pattern>
struct is_pattern<_pattern_repeated<Pattern>> : std::true_type {};

template<typename Pattern>
struct is_pattern<_pattern_except<Pattern>> : std::true_type {};

template<typename Pattern>
struct is_pattern<_pattern_longest<Pattern>> : std::true_type {};

template<typename Pattern>
struct is_pattern<_pattern_shortest<Pattern>> : std::true_type {};

template<typename Left, typename Right>
struct is_pattern<_pattern_rule<Left, Right>> : std::true_type {};

template<typename Pattern, typename Condition>
struct is_pattern<_condition<Pattern, Condition>> : std::true_type {};

#define WL_DEFINE_STRING_PATTERN_CONSTANT(pattern_type)         \
struct pattern_type {};                                         \
template<> struct is_pattern<pattern_type> : std::true_type {};

WL_DEFINE_STRING_PATTERN_CONSTANT(_whitespace_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_number_string_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_word_character_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_digit_character_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_hexadecimal_character_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_letter_character_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_whitespace_character_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_punctuation_character_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_word_boundary_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_start_of_line_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_end_of_line_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_start_of_string_type)
WL_DEFINE_STRING_PATTERN_CONSTANT(_end_of_string_type)

template<typename T>
constexpr auto is_pattern_v = is_pattern<T>::value;

template<typename T>
struct is_file_stream : std::false_type {};

template<stream_mode Mode, bool Binary>
struct is_file_stream<file_stream<Mode, Binary>> : std::true_type {};

template<typename T>
constexpr auto is_file_stream_v = is_file_stream<T>::value;


template<typename T>
struct promote_integral
{
    using type = std::conditional_t<is_integral_v<T>, double, T>;
};

template<typename T>
using promote_integral_t = typename promote_integral<T>::type;

template<typename T, typename U>
struct _common_type_impl
{
    using type = std::conditional_t<is_float_v<T>, T, U>;
};

template<> struct _common_type_impl<double, double> { using type = double; };
template<> struct _common_type_impl<float, double> { using type = double; };
template<> struct _common_type_impl<double, float> { using type = double; };
template<> struct _common_type_impl<float, float> { using type = float; };


template<typename... Ts>
struct common_type;

template<typename T, typename U>
struct common_type<T, U>
{
    using type = std::conditional_t<
        is_float_v<T> || is_float_v<U>,
        typename _common_type_impl<T, U>::type,
        std::conditional_t<(sizeof(T) > sizeof(U)), T,
        std::conditional_t<(sizeof(U) > sizeof(T)), U,
        std::conditional_t<std::is_unsigned_v<T>, T, U
        >>>>;
};

template<typename T, typename U>
struct common_type<T, complex<U>> : common_type<complex<U>, T> {};

template<typename T, typename U>
struct common_type<complex<T>, U>
{
    using type = std::conditional_t<is_integral_v<U>,
        complex<T>, complex<typename common_type<T, U>::type>>;
};

template<typename T, typename U>
struct common_type<complex<T>, complex<U>>
{
    using type = complex<typename common_type<T, U>::type>;
};

template<typename T1, typename T2, typename T3, typename... Ts>
struct common_type<T1, T2, T3, Ts...>
{
    using type = typename common_type<
        typename common_type<T1, T2>::type, T3, Ts...>::type;
};

template<typename... Ts>
using common_type_t = typename common_type<Ts...>::type;

template<typename T> struct make_signed { using type = T; };
template<> struct make_signed<uint8_t> { using type = int8_t; };
template<> struct make_signed<uint16_t> { using type = int16_t; };
template<> struct make_signed<uint32_t> { using type = int32_t; };
template<> struct make_signed<uint64_t> { using type = int64_t; };

template<typename T>
using make_signed_t = typename make_signed<T>::type;

template<typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template<typename T>
struct is_array : std::false_type {};

template<typename T, size_t R>
struct is_array<ndarray<T, R>> : std::true_type {};

template<typename T>
constexpr auto is_array_v = is_array<T>::value;

template<typename T>
struct is_array_view : std::false_type {};

template<typename T, size_t A, size_t V, bool C>
struct is_array_view<simple_view<T, A, V, C>> : std::true_type {};

template<typename T, size_t A, size_t V, size_t S, bool C>
struct is_array_view<regular_view<T, A, V, S, C>> : std::true_type {};

template<typename T, size_t A, size_t V, size_t S, typename IT, bool C>
struct is_array_view<general_view<T, A, V, S, IT, C>> : std::true_type {};

template<typename T>
constexpr auto is_array_view_v = is_array_view<T>::value;

template<typename T, typename = void>
struct value_type { using type = T; };

template<typename T>
struct value_type<T, std::void_t<typename T::value_type>>
{
    using type = typename T::value_type;
};

template<typename T>
using value_type_t = typename value_type<T>::type;


template<typename T>
struct array_rank : std::integral_constant<size_t, 0u> {};

template<typename T, size_t R>
struct array_rank<ndarray<T, R>> :
    std::integral_constant<size_t, R> {};

template<typename T, size_t A, size_t V, bool C>
struct array_rank<simple_view<T, A, V, C>> :
    std::integral_constant<size_t, V> {};

template<typename T, size_t A, size_t V, size_t S, bool C>
struct array_rank<regular_view<T, A, V, S, C>> :
    std::integral_constant<size_t, V> {};

template<typename T, size_t A, size_t V, size_t S, typename IT, bool C>
struct array_rank<general_view<T, A, V, S, IT, C>> :
    std::integral_constant<size_t, V> {};

template<typename T>
constexpr auto array_rank_v = array_rank<T>::value;


template<typename T>
constexpr auto is_numerical_type_v = (array_rank_v<T> == 0u ?
    is_arithmetic_v<T> : is_arithmetic_v<value_type_t<T>>);

template<typename T>
constexpr auto is_boolean_type_v = is_boolean_v<value_type_t<T>>;

template<typename T>
constexpr auto is_string_type_v =
    is_string_v<value_type_t<T>> || is_string_view_v<T>;

template<typename T>
constexpr auto is_value_type_v = is_arithmetic_v<T> || is_array_v<T> ||
    is_array_view_v<T> || is_boolean_v<T> || is_string_view_v<T> ||
    std::is_same_v<T, void_type> || std::is_same_v<T, all_type>;


template<typename T>
struct is_movable : std::false_type {};

template<typename T, size_t R>
struct is_movable<ndarray<T, R>&&> : std::true_type {};

template<typename T>
constexpr auto is_movable_v = is_movable<T>::value;

template<typename T>
constexpr auto array_is_const_v = std::is_const_v<
    std::remove_pointer_t<decltype(std::declval<T>().data())>>;

template<typename T, typename U>
struct _is_convertible_impl
{
    static constexpr bool value =
        (is_arithmetic_v<T> && is_complex_v<U>) ||
        (is_integral_v<T> && is_arithmetic_v<U>) ||
        (is_float_v<T> && is_float_v<U>) ||
        (is_string_view_v<T> && is_string_v<U>);
};

template<typename T, typename U>
struct is_convertible
{
    static constexpr bool value =
        _is_convertible_impl<T, U>::value ||
        (is_array_v<T> && is_array_v<U> &&
            array_rank_v<T> == array_rank_v<U> &&
            _is_convertible_impl<value_type_t<T>, value_type_t<U>>::value);
};

template<typename T, typename Ret, typename... Args>
struct is_convertible<T, function<Ret(Args...)>> : std::true_type {};

template<typename T>
struct is_convertible<T, T> : std::true_type {};

template<typename T, typename U>
constexpr auto is_convertible_v = is_convertible<T, U>::value;


template<typename... Ts>
struct all_is_integral;

template<>
struct all_is_integral<> : std::true_type {};

template<typename T1, typename... Ts>
struct all_is_integral<T1, Ts...> :
    std::bool_constant<is_integral_v<T1> && all_is_integral<Ts...>::value> {};

template<typename... Ts>
constexpr auto all_is_integral_v = all_is_integral<Ts...>::value;

template<typename... Ts>
struct all_is_real;

template<>
struct all_is_real<> : std::true_type {};

template<typename T1, typename... Ts>
struct all_is_real<T1, Ts...> :
    std::bool_constant<is_real_v<T1> && all_is_real<Ts...>::value> {};

template<typename... Ts>
constexpr auto all_is_real_v = all_is_real<Ts...>::value;


template<typename F>
struct is_variadic_function : std::false_type {};

template<typename Normal, typename Variadic>
struct is_variadic_function<variadic<Normal, Variadic>> : std::true_type {};

template<>
struct is_variadic_function<list_type> : std::true_type {};

template<typename Fn>
constexpr auto is_variadic_function_v = is_variadic_function<Fn>::value;

template<typename T>
struct is_argument_pack : std::false_type {};

template<typename Iter, bool HasStride>
struct is_argument_pack<argument_pack<Iter, HasStride>> : std::true_type {};

template<typename Iter>
constexpr auto is_argument_pack_v = is_argument_pack<Iter>::value;

}
