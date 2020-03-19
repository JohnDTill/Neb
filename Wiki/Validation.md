# Stages

* Scanner
* Grammar errors
* Type errors
* Dimension errors
* Value errors
* Runtime errors (HATE THESE)

# Type Errors

Due to context sensitivity regarding types, there are some syntax errors which cannot be caught at the parsing stage. Consider set subtraction: the notation `A - B` works well with the grammar for numbers, but `A / B` does not because it has a different precedence than scalar division. Fortunately, set expressions typically use parenthesis with every nested binary operation. If you enforce that rule, then the grammar is okay since combined set operations use grouping to avoid precedence. However, you still end up with trouble detecting sets: `A / B ∪ C` is an error, but this depends on knowledge of types which is not available in the parsing phase. This motivates Neb to use a type-resolution phase after the parsing phase.

# Thoughts

Your desire to generally and correctly parse everything gets you into trouble. Sets have a very different grammar from numbers. This leads to needing to analyze type and possible **annotate type**, which is a programming hurdle. This leads to needing to make decisions about the type system which detract from generality.

* Is Neb statically typed?
 * Are there default types?
* Is Neb strongly typed?
