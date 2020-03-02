# Grammar

There are several choices which need special consideration while designing the grammar. These are:
* Context sensitive equality/inequality: at the top-level, this is a statement, but at lower levels it is a test
* Implicit multiplication: `5a(x+y)` parses the same as `5*a*(x+y)`
* Context sensitive superscripts / subscripts: `xᵢ` is probably meant to be parsed as an index while `pₓ` is probably meant to be parsed as a single identifier
* Context sensitive accents: `x̂` could be an operator or part of an identifier. `ẋ` is unique. `x̅` could be an identifier or could refer to the mean.
* Newline could terminate a statement or go to a binary operator

How do you define functions?

Is the symbol `π` a number literal or identifier? How do you handle variables with default meanings which may be overridden? MATLAB handles this with assignment, but I think you want an approach more like overriding. At what stage would you resolve the reference? Compile time? Run time?

Some symbols have a unique interpretation and are immune to overriding, e.g. `∞`. In some cases we may define symbols to fall into this category, e.g. use differentiation `ⅆy/ⅆx` instead of `dy/dx`. But does this impose unnecessary burden to access that special differential symbol?

## Scripts

Different contexts in which scripts may be used:

<b>Superscripts</b>
* Exponentiation `x²`
* \*-modifier `x*`
* Set dimensions `ℝ³ˣ³`
* Set signs `ℝ⁺`
* Unary operators `x⁺` `Aᵀ`

<b>Subscripts</b>
* Index `xᵢ`
* \*-modifier
* ID-modifier `pₓ` `u = Kₚ * e`
* Derivative `y,ₓ`
* Norm modifier `⁜_⏴‖e‖⏵⏴L1⏵`
* Function/Operator argument `I₃ₓ₃` `∇ₓT`

Dual-scripts combine these.

## Newlines

Every statement except for the last statement must be terminated. The terminator is either a comma or a newline. However, binary operators may also contain newlines between the arguments and the operator, so the following is valid:
```
x
=
a
+
b
```
So while the first `x = a \n` part would be a valid terminated statement, we need to parse the full statement.

Is this "look ahead"?

## Implicit multiplication

To the extent it can be unambiguous, we want to allow implicit multiplication, e.g. the expression `5a(x+y)` parses the same as `5*a*(x+y)`.

There can be only one number, and if present it must be the leading element. The expression `5a` is acceptable, but `a5`, `5 10`, and `(a+b)5` are examples of invalid implicit multiplications.

Mixing implicit and explicit multiplication should be either a warning or forbidden. It is poor form to write `xy*z`.

What can be implicitly multiplied?
* Optional leading number
* Identifier
* Grouping
* Fractions
* Scripts
* Derivatives
* Integrals

Does implicit multiplication span newlines?

Do expression statements clash with implicit multiplication?
```
x = (a + b + c + d + e)
    (c / d)
```
That would certainly make top-level expression statements ambiguous.

How do you trigger the implicit multiplication rule?

Order of operations: How should `ab/cd` be parsed? I would tend to make it parse as `(ab)/(cd)`.

Type information may be needed to resolve the difference between a single argument function call and an implicit multiplication, e.g. `f(x+y)` is probably a function, but `a(x+y)` is probably an implicit multiplication. The parsed result depends on the types of `f` and `a`, if they are callable.
