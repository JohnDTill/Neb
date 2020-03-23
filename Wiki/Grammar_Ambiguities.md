# Ambiguities

Unfortunately, conventional mathematical syntax contains ambiguities which are dependent on some higher level understanding of the context.

#### Sequence vs. Grouping or Open Range

A sequence is typically written as a list of members enclosed in parenthesis, e.g. `(3,2,1)`. For 1 member sequences, this conflicts with the grouping rule `(3)`, and for 2 member sequences, this may conflict with the range rule `(2,3)`. Groupings and open ranges are more commonly encountered, but sequences are fundamental to programming tasks.

#### Set Builder Bar

It is difficult to use a bar in set builder notation, e.g. `ℝ⁺ ≔ {x | x > 0}`. The parser sees the bar then wants to do an implicit multiplication, e.g. expecting `{x |x|}`. The easy solution is to use a colon: `ℝ⁺ ≔ {x : x > 0}` parses easily and is valid mathematical syntax.

We hate to omit any valid mathematical syntax such as the bar, and with sufficient lookahead the ambiguity can be resolved. Eventually we may want a process to determine set builder versus set enumerator notation. This needs to work even in the most insidious cases such as:
* `{x | |x| > 3}` (Set builder w/ absolute value in condition)
* `{x ∈ ℝ | |x| > 3}` (Same with member statement)
* `{x |x| |x| |x|, |x|}` (Set enumeration with implicit multiplication by abs val)
* `{x | x ∈ {y | |y| > 3}}` (Nested set builder)

The solution is likely to be hacky, so for the time being, it probably makes sense to require the colon notation until the parser is more stable.
