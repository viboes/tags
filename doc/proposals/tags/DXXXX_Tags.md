<table border="0" cellpadding="0" cellspacing="0" style="border-collapse: collapse" bordercolor="#111111" width="607">
    <tr>
        <td width="172" align="left" valign="top">Document number:</td>
        <td width="435"><span style="background-color: #FFFF00">DXXXX</span>=yy-nnnn</td>
    </tr>
    <tr>
        <td width="172" align="left" valign="top">Date:</td>
        <td width="435">2015-05-17</td>
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

C++ tags library
================

Experimental tags library for C++17.

# Introduction

This paper present a proposal for a tags C++ library that can be used to name types. 
These tags can be used to name the types of the `std::tuple`, `std::experimental::variant` or `std::experimental::any`. 

The aim of a named types is to provide compile time access to elements by name, as a classic struct or union would, 
and compile time access to elements by their index, as a `std::tuple` would.

`tuple` can be adapted with no overhead to provide access by name.


# Motivation and Scope


A named type makes code cleaner since it provides more meaningful access for attributes and 
let code using them more robust when attributes are inserted or deleted.



# Tutorial

## Tags

We can see a tag as a selector/getter of a type that contains tagged types.

```c++
struct _first : tag<_first> {}; 
```

It is useful to define tag constants so that syntax is less cumbersome. 

```c++
constexpr struct _key : tag<_key> {} key; 
constexpr struct _value : tag<_value> {} value; 
```

Given a tag aware type `X`, we can select one of its tagged types using the tag. 

```c++
X x;

// using a function style
auto k = key(x); // the decltype of k depends on the type X 

// if uniform call syntax is adopted
auto k = x.key(); // the decltype of k depends on the type X 

// using operator% as a member like selection
auto v = x%value; // the decltype of k depends on the type X 
```

Note that the tag constants are not absolutely needed. The user can always construct them explicitly

```c++
auto k = key{}(x);
auto v = x%value{};

```

A tagged type is a type that wraps a value type `T` adding a specific tag type.
It can be constructor using the `operator=`on the specific tag and the value type.

```c++
tagged<int, _first>  t = (first=1);
```

As tag types, tagged types can also be used as selectors with the same syntax. 
The main difference is that as a tagged type contains already the value type, 
the result type of the selection doesn't depend on the type to which it is applied. 
As we can see late, this kind of selectors is useful when working with erased types that 
don't know about the types they can contain.    


## Tags selection customization

Both `tag<Tag>`or `tagged<T, Tag>` define the selection operations using `operator()`and `operator%`.
But a tag and a tagged types are unable to do by itself the selection. The selection depends also on the type on which the selection is applied.
This mean that the implementation has to dispatch to this type. We have chosen the `select` function for this purpose.

Next follows a possible implementation of the tag class template.

```c++
template <class Tag>
struct tag {
    template < class T >
    auto operator=(T const& v) { return tagged<T, Tag>(v); }
    template < class T >
    auto operator()(T const& v) { return select(*this, v); }
    template < class T >
    friend auto operator%(T const& v, tag) { return select(*this, v); }
};
```

A tag aware type `X`, should define an overload of the select function. 

```c++
template <class Tag>
auto select(tag<Tag>, X const& x);
```

## Tagged type and function types 

As tags aware types are meant to store data indexed by a tag, it is important to keep a 
readable declarative syntax to use them as members or as non-templated function arguments if needed.

The following usage is not too friendly. 

```c++
tuple<tagged<std::string,_name>, tagged<int,_age>> test;
```

We could replace the redondant `tagged` and replace it by a function style signature `T(Tag)` meaning that the access by the tag `Tag` is of type `T`.

```c++
tuple<std::string(_name), int(_age)> test;
```



## Tags and `tuple` or `pair`

```c++
auto p = make_pair(key=1, value="Hello"_s);
auto q = make_tuple(key=1, value="Hello"_s);

p%key = 2;
f(q%value);

```


## Tags and `variant`

```c++
using std::experimental;

variant<int(_t1)>, int(_t2)> a = 1;

v = (t1=1);

v.get<_t1>;
v%t1;
t1(v);

```

## Tags and `any`

```c++
std::experimental::any a = 1;

a = (name="Vicente"_s);

auto t = any_cast<string(_name)>(a); // decltype(t) is tagged<string, _name>.
auto r = tag_cast(any_cast<string(_name)>(a)); // decltype(r) is string.
auto s = any_tag_cast<string(_name)>(a); // decltype(s) is string.

```

It would be great if we could use the tagged type as selector

```c++
auto s = a%tagged<string, _name>{}; // decltype(s) is string.

```

However, the difference between `tag<Tag>` and `tagged<T, Tag>` is that the first don't store anything, while the last wraps a value type.
We could make use of the `type<T>` class template to avoid the expensive constructions

```c++
auto s = a%type<tagged<string, _name>>{}; // decltype(s) is string.

```

But this is not friendly. Oe we could use the `types` class template directly.


```c++
auto s = a%types<string, _name>{}; // decltype(s) is string.

```

But this doesn't  

Associated to the `tagged<T,Tag>` class template there is a class template `named<T,Tag>` that doesn't contains anything 
except a nested type that is `tagged<T,Tag>`

```c++
auto s = a%named<string, _name>{}; // decltype(s) is string.

```

There is yet an alternative using the tag and a nested alias template `_` that is `named<T, Tag>`.

```c++
auto s = a%name._<string>{};

```


# Design Rationale

## Report a compile time error when possible

When the selection is applied to a type that can check at compile time whether the selection can be done or not, it is normal that a compile error is reported. 
However type to which is applied the selection can be type erased type and so the check can be done only at run-time. In this casan exception is expected.

E.g. selecting a a tag from a tagged `tuple` can be checked at compile time but the check will be done always at run time on the class `any`.

## `constexpr`and `noexcept`

The selection could be `constexpr` and `noexcept` depending of the type the selection is applied to. 
E.g. selecting a tag from a tagged `tuple`, can be `constexpr`and `noexcept`, however, the selection on `any` can not be neither `constexpr` nor `noexcept`


## Implicit conversion between named and not named types


# Open Points

* Do we want `tag` and `tagged` types? 

* Do we want both the functional style `tag(x)`and the member like style `x%tag`?

* Do we want the `tag` assignment operator as factory of `tagged` type instances?

* Do we want the `named<T, Tag>` class?

* Do we want the `tag._<T>` as factory of `tagged` types?

* Do we want to customize `tuple`, `pair`, `variant` and/or `any`? 

* Do we want to use the function style `T(Tag)` as template parameters to mean tagged<T,Tag>?

* Do we want to use `x%tag._<T>` as selector? 

# Technical Specification

## Header `<experimental/tag> Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

    template< class Tag >
    struct tag;

    template< class Type, class Tag >
    struct named;
       
    template< class Type, class Tag >
    struct tagged;
    template< class Type, class Tag >
    struct tagged<Type &, Tag>;
    template< class Type, class Tag >
    struct tagged<Type *, Tag>;
        
}
}
}
```


## Class template `tag` 

## Class template `tagged` 

## Class template `named` 


## Header `<experimental/tuple> Synopsis 

## Header `<experimental/any> Synopsis 

## Header `<experimental/variant> Synopsis 

# Implementation

There is an implementation at https://github.com/viboes/tags.

# Acknowledgements 

Many thanks to XXX. 

# References

* DXXXX - Fundamental TS

