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

Experimental overload function for C++17.

# Introduction

This paper present a proposal for an `match` function.


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

    template <class R, class ST, class... Fs>
    'see below' match(const ST &that, Fs &&... fcts);

    template <class R, class... STs, class... Fs>
    'see below' match_all(const std::tuple<STs...> &those, Fs &&... fcts);
    
}
}
}
```

## Template function `match` 

## Template function `match_all` 

## Implementation

There is an implementation at https://github.com/viboes/tags.

# Acknowledgements 

Many thanks to XXX. 

# References

## DXXXX - Overload
## DXXXX - Apply

