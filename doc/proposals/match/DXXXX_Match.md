<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2014-11-17</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Project:</td>
        <td width="435">Programming Language C++, Library Evolution Working Group</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Reply-to:</td>
        <td width="435">Vicente J. Botet Escriba &lt;<a href="mailto:vicente.botet@wanadoo.fr">vicente.botet@wanadoo.fr</a>&gt;</td>
    </tr>
</table>

C++ generic match function
=============================

Experimental match function for C++17.

# Introduction

This paper present a proposal for two generic `match` function that allow 
to visit sum types individually or by groups.


# Motivation and Scope

Getting the value stored in a sum types as `variant<Ts...>` or `optional<T>` needs to know 
which type is stored in. Using visitation is a common technique that makes the access 
safer.


# Tutorial

## Customizing the `match` function

The proposed `match` function work for sum types `ST` that have 
customized the following overloaded function

```c++
          temaplate <class R, class F>
          match(type<R>, ST const&, F&& );
```

where the `type` is just a tag class used to make unambiguous the overload and simplify 
the customization.


For example, we could customize `boost::variant`as follows: 

```c++
    namespace boost {
          temaplate <class R, class F, class ...Ts >
          match(type<R>, variant<Ts...> const& v, F&& f) 
          { return v.visit(std::forward<F>(f)); }
    }
```


## Using the `match` function to visit one sum type

Given the `boost::variant` sum type, we could just visit it using the `overload` function 
(See DXXX).


```c++
    boost::variant<int, X> a = 2;
    a.visit(overload(
        [](int i)
        {},
        [](X const& i)
        {
          assert(false);
        },
        [](...)
        {
          assert(false);
        }
    ));
```

or we can use the variadic `match` function

```c++
    boost::variant<int, X> a = 2;
    match<void>(a,
        [](int i)
        {},
        [](X const& i)
        {
          assert(false);
        },
        [](...)
        {
          assert(false);
        }
    );
```

## Using the `match` function to visit one sum type


The `match` function generalize the visitation for several sum types, e.g. we can 
visit `variant` and `optional` at once:

```c++
    boost::variant<int, X> a = 2;
    boost::optional<int> b = 2;
    match<void>(std::make_tuple(a, b),
        [](int const &i, int const &j )
        {
        },
        [](auto const &i, auto const &j )
        {
          assert(false);
        },
        [](...)
        {
          assert(false);
        }
    );
```
    
# Design rationale

## Result type of `match`

We can consider two alternatives:

* explicit return type `R`: the result type is `R` and the result type of the overloads 
must be explicitly convertible to `R`,
* `common_type`: the result type is the `common_type` of the result of the overloads,

Each one of these alternatives would needs a specific interface:

* `explicit_match`
* `common_type_match`

For a sake of simplicity this proposal only contains a `match` version that uses the 
explicit return type. The other can be built on top of this version. 

## Customization point

The customization point

```c++
          temaplate <class R, class F>
          match(type<R>, ST const&, F&& );
```

can be considered unusual, as it contains a parameter type<R>. 
The type<R> parameter is added to force the return type without inhibiting overload resolution. Calling the customization point

```c++
          match<R>(st, f);
```

would not find for overloaded functions.


## Grouping with `overload`

We can also group all the function with the `overload` function and let the variadic part 
for the sum types. 

```c++
    boost::variant<int, X> a = 2;
    boost::optional<int> b = 2;
    match<void>(overload(
        [](int const &i, int const &j )
        {
            //...
        },
        [](auto const &i, auto const &j )
        {
            assert(false);
        },
        [](...)
        {
            assert(false);
        }
    ), a, b;
```

This has the advantage of having a single of resolving the the problem with a single 
function. The liability is much a question of style. The author prefer to give the sum 
types first a,d the overloads later. If we had a language type pattern matching feature it 
would be much more like


```c++
    boost::variant<int, X> a = 2;
    boost::optional<int> b = 2;
    match (a, b) {
        case (int const &i, int const &j ) :
            //...
        case (auto const &i, auto const &j ) :
            assert(false);
        default:
            assert(false);
    }
```

# Technical Specification

## Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

    template <class R, class ST, class... Fs>
    'see below' match(const ST &that, Fs &&... fcts);

    template <class R, class... STs, class... Fs>
    'see below' match(const std::tuple<STs...> &those, Fs &&... fcts);
    
}
}
}
```

## Template function `match` 

```c++
    template <class R, class ST, class... Fs>
    'see below' match(const ST &that, Fs &&... fcts);
```

*Returns:* the result of calling the overloaded functions `fcts` depending on the type 
stored on the sum type as if

```c++
    return match(type<R>, type<ST>, that, overload(std::forward<Fs>(fcts)));
```

*Remarks:* This function will not participate in overload resolution if ST is a tuple type.

*Thows:* Any exception thrown during the construction any internal object or thrown by the call of the selected overloaded function.

## Template function `match` 

```c++
    template <class R, class... STs, class... Fs>
    'see below' match(const std::tuple<STs...> &those, Fs &&... fcts);   
```

*Returns:* the result of calling the overloaded functions `fcts` depending on the type 
stored on the sum types `Sts...`.

*Thows:* Any exception thrown during the construction any internal object or thrown by the call of the selected overloaded function.


## Future Work

If this proposal is supported, it could be completed with the configuration for `std::experimental::optional<T>` and of course improve the wording. 

The authors are working on a proposal to be able to visit on a selected collection of types from `std::experimental::any`.

```c++
    any a = 2;
    //...
    match<void>(select<types<int, std::string>>(a),
        [](int i)
        {},
        [](std::string const& s)
        {
          assert(false);
        },
        [](...)
        {
          assert(false);
        }
    );
```

We are working also on a `tagged` class that would allow to store the same underlying type but with different tags on `tuple`, `variant` or `any`.
The user could then use the tags instead of the types to select the visited types.

    boost::any a = make_tagged<t1>(1);
    match<void>(select<tags<t1, t2>>(a),
        [](tagged<int, t1> i)
        {
        },
        [](tagged<int, t2> i)
        {
        },
        [](...)
        {
          assert(false);
        }
    );
    

## Implementation

There is an implementation at https://github.com/viboes/tags including customization for `boost::variant`, `std::experimental::optional` and the proposed `std::experimental::expected`.

# Acknowledgements 

Many thanks to Mat Calabrese who suggested the `common_type` approach. 

# References

## DXXXX - Overload
## DXXXX - Apply

