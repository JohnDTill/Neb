# Grammar

There are several choices which need special consideration while designing the grammar. These are:
* Context sensitive equality/inequality: at the top-level, this is a statement, but at lower levels it is a test
* Context sensitive superscripts / subscripts: `xᵢ` is probably meant to be parsed as an index while `pₓ` is probably meant to be parsed as a single identifier
* Context sensitive accents: `x̂` could be an operator or part of an identifier. `ẋ` is unique. `x̅` could be an identifier or could refer to the mean.
* Newline could terminate a statement or go to a binary operator

How do you define functions?

Is the symbol `π` a number literal or identifier? How do you handle variables with default meanings which may be overridden? MATLAB handles this with assignment, but I think you want an approach more like overriding. At what stage would you resolve the reference? Compile time? Run time?

Some symbols have a unique interpretation and are immune to overriding, e.g. `∞`. In some cases we may define symbols to fall into this category, e.g. use differentiation `ⅆy/ⅆx` instead of `dy/dx`. But does this impose unnecessary burden to access that special differential symbol?

Set subtraction `A - B` is okay, but `A / B` is a horror because it gives the slash a different precedence than scalar division. You would hate to have the precedence dependent on the type system. However, set expressions typically use parenthesis with every nested binary operation. If you enforce that rule, then `A / B` is okay. But you still end up with trouble detecting sets: `A / B ∪ C` is an error, but you need to look at the LHS of the union operation to see it is invalid.

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