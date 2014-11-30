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

C++ generic overload function
=============================

Experimental overload function for C++17.

# Introduction

This paper present a proposal for an `overload` function that allow to overload lambdas 
as well as function or function objects.


# Motivation and Scope

As lambdas are functions and not function objects, can’t be overloaded in the usual 
implicit way, but they can be “explicitly overloaded” using the proposed `overload` 
function:

This function would be especially useful for creating visitors, e.g. for variant.

```c++
     auto visitor = overload(
        [](int i, int j )         {          ...        },
        [](int i, string const &j )         {          ...        },
        [](auto const &i, auto const &j )        {          ...        }
    );
    
    visitor( 1, std::string{"2"} ); // ok - calls (int,std::string) "overload"

```

The `overload` function when there are only two parameters can be defined as follows

```c++
  template<class F1, class F2> struct overloaded : F1, F2
  {
    overloaded(F1 x1, F2 x2) : F1(x1), F2(x2) {}
    using F1::operator();
    using F2::operator();
  };
  template<class F1, class F2>
  overloaded<F1, F2> overload(F1 f1, F2 f2)
  { return overloaded<F1, F2>(f1, f2); }
```


# Design rationale

## Which kind of functions would overload accept

The previous definition of overload is quite simple, however it doesn't accept member 
functions, as std::bind does.


As there is no major problem implementing it and that this inclusion doesn't degrade the 
performances, we opt to include them also. 

## Binary or variadic interface

We could either provide a binary or a variadic `overload` function. 

```c++
     auto visitor = 
     overload([](int i, int j )         {          ...        },
     overload([](int i, string const &j )         {          ...        },
        [](auto const &i, auto const &j )        {          ...        }
     ));

```

The binary function needs to repeat the overload word for each new overloaded function.


We think that the variadic version is not much more complex to implement and makes user 
code simpler.


## Passing parameters by value or by forward reference

The function `overload` must store the passed parameters. If the interface is by value, 
the user will be forced to `move` movable but non-copyable function objects. Using forward 
references has not this inconvenient, and the implementation can optimize when the 
function object is copyable.

## Result type of `overload`

The proposed `overload` function doesn't adds any contraint on the result type of the 
overloaded functions. The result type depends on the one of the overloaded function.

We can consider 3 other alternatives:

* explicit return type `R`: the result type is `R` and the result type of the overloads must be explicitly convertible to `R`,
* `common_type`: the result type is the `common_type` of the result of the overloads,
* `variant`: the result type is the `variant` of the unique result types of the overloads

Each one of these alternatives would needs a specific interface:

* `overload`
* `explicit_overload`
* `common_type_overload`
* `variant_overload`

For a sake of simplicity this proposal only contains the `overload` version.

# Technical Specification

## Synopsis 


```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{
    template <class R, class ... Fs>
    'see below' overload(Fs &&... fcts);
}
}
}
```

## Template function `overload` 

```c++
    template <class R, class ... Fs>
    'see below' overload(Fs &&... fcts);

```

*Requires:* there is a `common_type` of the result of the overloads, which will be the result type of the `overload` function.

*Returns:* A function object that behaves as if all the parameters were overloaded when 
calling it.

*Thows:* Any exception throw during the construction of the resulting function object.

## Implementation

There is an implementation of the explicit return type version at https://github.com/viboes/tags.

# Acknowledgements 

Many thanks to Matt Calabrese who suggested the `common_type` alternative as the more common. 

# References

