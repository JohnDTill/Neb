# Grammar

There are several choices which need special consideration while designing the grammar. These are:
* Context sensitive equality/inequality: at the top-level, this is a statement, but at lower levels it is a test
* Newline could terminate a statement or go to a binary operator

How do you define functions?

Is the symbol `π` a number literal or identifier? How do you handle variables with default meanings which may be overridden? MATLAB handles this with assignment, but I think you want an approach more like overriding. At what stage would you resolve the reference? Compile time? Run time?

Some symbols have a unique interpretation and are immune to overriding, e.g. `∞`. In some cases we may define symbols to fall into this category, e.g. use differentiation `ⅆy/ⅆx` instead of `dy/dx`. But does this impose unnecessary burden to access that special differential symbol?

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