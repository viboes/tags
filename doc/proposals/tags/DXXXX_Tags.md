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

Experimental tags library for C++17.

# Introduction

This paper present a proposal for a tags C++ library that can be used to name types.


# Motivation and Scope


# Tutorial

# Design rationale

# Technical Specification

## Synopsis 

```c++
namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

    template< class Tag >
    struct tag_type;

    template< class Tag >
    using tag_type_t = typename tag_type<Tag>::type;
    
    template< class Type, class Tag >
    struct tagged;
    template< class Type, class Tag >
    struct tagged<Type &, Tag>;
    template< class Type, class Tag >
    struct tagged<Type *, Tag>;
    
    template< class Tag, class Type >
    tagged<tag_type_t<Tag>, Tag> make_tagged(Type &&);
    

    template< class ...Tags >
    struct types;
    
    template< class ...Tags >
    using tags = types<tagged<tag_type_t<Tags>, Tags> ...>;
    
    template < class Types, class T>
    'see below' select(T& v)
    
}
}
}
```

## Template class `tag_type` 

## Template class `tagged` 

## Template function `make_tagged` 

## Template class `types` 

## Template function `select` 

## Implementation

There is an implementation at https://github.com/viboes/tags.

# Acknowledgements 

Many thanks to XXX. 

# References

## DXXXX - Any
## DXXXX - Match
## DXXXX - Overload

