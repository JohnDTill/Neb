# Implicit Multiplication

As in mathematics, consecutive identifiers are understood to be multiplied. For example, the source `5a(x+y)` parses the same as `5*a*(x+y)`.

### Order of Operations
There is a question of what precedence implicit multiplication should have compared to explicit multiplication precedence. For example, how should `ab/cd` be parsed? I would tend to make it parse as `(ab)/(cd)`.

### Restrictions
There are a few restrictions which improve readability and align with conventional math notation. There can be only one number, and if present it must be the leading element. The expression `5a` is acceptable, but `a5`, `5 10`, and `(a+b)5` are examples of invalid implicit multiplications.

Mixing implicit and explicit multiplication should be either a warning or forbidden. It is poor form to write `xy*z`.

Implicit multiplications cannot span newlines. The following source would parse as an equality statement followed on the next line by an expression statement:
```
x = (a + b + c + d + e)
    (c / d)
```

### Difficulty Grouping
Implicit multiplication is disallowed for groupings with identical open and close symbols, e.g. absolute value. See [the page on ambiguities](./GrammarAmbiguities) for the rationale behind this decision.

### Grammar
ImplicitMult ⇒ LeftUnary ImplicitMultArg*

ImplicitMultArg ⇒ ID | Grouping | EscapeExpr | Derivative

Note: We will peek at the next token to see if it triggers an implicit multiplication.

### Dependence on Type
Type information may be needed to resolve the difference between a single argument function call and an implicit multiplication, e.g. `f(x+y)` is probably a function, but `a(x+y)` is probably an implicit multiplication. The parsed result depends on the types of `f` and `a`, if they are callable.

Note there is also a possibility for implicit function calls e.g. `sin θ`.
