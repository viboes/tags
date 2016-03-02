<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">D0XXXR0</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2016-03-02</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">ISO/IEC JTC1 SC22 WG21 Programming Language C++</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Audience:</td>
        <td width="435">Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escriba &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

`variant` is not a Tuple-like type
==================================


This paper presents a proposal to change the way we can get the types associated to a `variant`.

The current specification for `variant` use the Tuple-like related traits `std::tuple_size` and `std::tuple_element` to get the size en type elements. [D0144R2] defines structure binding based on `get<I>` and `std::tuple_size<T>`.

However variant is not a Tuple-like type and cannot work with structured bindings, so it should use an alternative approach. The alternative consists in adding a trait to get the variant alternatives that should behave almost as a Tuple-like type.

# Table of Contents

1. [Introduction](#introduction)
2. [Motivation](#motivation)
3. [Proposal](#proposal)
4. [Design rationale](#design-rationale)
5. [Open points](#open-points)
6. [Proposed wording](#proposed-wording)
7. [Implementability](#implementability)
8. [Acknowledgements](#acknowledgements)
9. [References](#references)


# Introduction

This paper presents a proposal to change the way we can get the types associated to a `variant`.

The current specification for `variant` use the Tuple-like related traits `std::tuple_size` and `std::tuple_element` to get the size en type elements. [D0144R2] defines structure binding based on `get<I>` and `std::tuple_size<T>`.

However variant is not a Tuple-like type and cannot work with structured bindings, so it should use an alternative approach. The alternative consists in adding a trait to get the variant alternatives that should behave almost as a Tuple-like type.


# Motivation

The issue was raised in a post to EWG and LEWG mailing list "structured binding and variant proposal".

There is an incompatibility between [P0144R1][D0144R2] and [P0088R1].

With structured binding it will be possible to structured bind some variables from a `variant`resulting always in a exception thrown

```c++
variant <int, string> f();
auto {x, y} = f();
```

While we can just say to the user to don't do that, we can avoid this just by don't considering a `variant`as a Tuple-like type don't reusing the `tuple_size` that should be preserved for product-types.

# Proposal

## Add a customization point to get the alternative types of a sum type

Instead of the of specializing `tuple_size` and `tuple_element` for `variant` we propose to specialize a new type trait `sum_type_alternatives` given the product-type of all the alternatives.


```c++
namespace std {

temaplate <class ST>
struct sum_type_alternatives;

}
```

For example, we could customize `std::variant`as follows: 


```c++
template <class ...Ts >
  struct sum_type_alternatives<variant<Ts...>> 
{
  using type = ...;
}
```

There must be defined specializations of the tuple-like helper meta-functions

`std::tuple_size` and `std::tuple_element` for the nested typedef `type. 

    
# Design rationale


## `sum_type_alternatives`

[P0088R1] variant proposal specializes `tuple_element` and `tuple_size` for `variant<Types...>`. The author think that as variant is not a product type, specializing these function is weird. However the alternatives of a sum type are a product type and so we can specialize `tuple_element` and `tuple_size` on `sum_type_alternatives<ST>::type`. 

## `sum_type_alternatives`is in line with `hold_alternatives`

The use if the `_alternatives` suffix is in line with the function `holds_alternative`. 

## Considering any type as a sum type with a single alternative

We could consider any type as a sum type with a single alternative.

```c++
temaplate <class T>
struct sum_type_alternatives { using type = T; }
```


## Open Points
The authors would like to have an answer to the following points if there is at all an interest in this proposal:

* `sum_type_` or `variant_` prefix.

* Do we want to see a type `T` as a sum type with a unique alternative.
 
* Which file for `sum_type_alternatives`?

* Do we want `__type_list`? And if yes, where?

# Technical Specification

### Header <sum_type> Synopsis

```c++
namespace std
{

    template <class ST>
        struct sum_type_alternatives; // undefined
    template <class ST>
        struct sum_type_alternatives<ST&> : sum_type_alternatives<ST> {}
    template <class ST>
        struct sum_type_alternatives<ST&&> : sum_type_alternatives<ST> {}
    template <class ST>
        struct sum_type_alternatives<const ST> : sum_type_alternatives<ST> {}
    template <class ST>
        struct sum_type_alternatives<volatile ST> : sum_type_alternatives<ST> {}
    template <class ST>
        using sum_type_alternatives_t = typename sum_type_alternatives<ST>::type;
    template <class ST, int N>
        using sum_type_alternative = tuple_element<sum_type_alternatives_t<ST>, N>;
    template <class ST, int N>
        using sum_type_alternative_t = tuple_element_t<sum_type_alternatives_t<ST>, N>;
    template <class ST>
        using sum_type_size = tuple_size<sum_type_alternatives_t<ST>>;   
    template <class ST>
        using sum_type_size_v = tuple_size_v<sum_type_alternatives_t<ST>>;   
   
}
```

## Type trait `sum_type_alternatives`

The nested type `sum_type_alternatives<ST>::type` must define the tuple-like helper meta-functions `std::tuple_size` and `std::tuple_element`.


### Header <optional> Synopsis

```c++
namespace std {
    template <class T >
        struct sum_type_alternatives<optional<T>> 
    {
        using type = __type_list<nullopt_t, T>; // implementation defined
    };
}}}
```

### Header <variant> Synopsis

Replace the following

?.12 tuple interface to class template `variant`

```c++
  // ?.12, tuple interface
  template <class... Types>
    struct tuple_size<variant<Types...>>;
  template <size_t I, class... Types>
    struct tuple_element<I, variant<Types...>>;
```

by

?.12 Sum type alternatives specialization for class template `variant`

```c++
  // ?.12, sum-type interface

    template <class ...Ts >
        struct sum_type_alternatives<variant<Ts...>> // implementation defined
    {
        using type = __type_list<Ts...>;
    };
```

Replace any occurrence of `tuple_element_t` by `sum_type_element_t` and `tuple_size_v` by `sum_type_size_v`.


## Implementation

There is an implementation of the traits at https://github.com/viboes/tags including customization for `boost::variant`, `std::experimental::optional` and the proposed `std::experimental::expected`.

There is no adaptation of the `variant` implementation, but this is trivial.


# Acknowledgements

Thanks to Agustín K-ballo Bergé that raised the size issue in [P0144R1]. Thanks to all that have supported addition of `tuple_size` in [D0144R2].


# References

[P0144R1]: http://wiki.edg.com/bin/view/Wg21jacksonville/P0144R1 "Structured bindings"

[D0144R2]: http://wiki.edg.com/bin/view/Wg21jacksonville/P0144R1 "Structured bindings"

[P0088R1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0088r1.html "Variant: a type-safe union (v6)"


* [P0088R1] Variant: a type-safe union (v6)

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0088r1.html

* [P0144R1] Structured bindings

    http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0144r1.pdf

* [D0144R2] Structured bindings

    http://wiki.edg.com/bin/view/Wg21jacksonville/P0144R1
