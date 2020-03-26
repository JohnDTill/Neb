# Ambiguities

Unfortunately, conventional mathematical syntax contains ambiguities which are dependent on some higher level understanding of the context.

#### Sequence vs. Grouping or Open Range

A sequence is typically written as a list of members enclosed in parenthesis, e.g. `(3,2,1)`. For 1 member sequences, this conflicts with the grouping rule `(3)`, and for 2 member sequences, this may conflict with the range rule `(2,3)`. Groupings and open ranges are more commonly encountered, but sequences are fundamental to programming tasks.

#### Implicit Multiplication by Absolute Value

Groupings with identical open and close symbols can introduce ambiguities. For example, consider the expression `|a|b|c|`. This could correctly be parsed as `abs(a*abs(b)*c)` or `abs(a)*b*abs(c)`, which are not mathematically equivalent. One solution is to disallow implicit multiplication for grouping with identical open and close symbols. Then one would have to write `|a*|b|*c|` or `|a|*b*|c|`, which have clear meanings.

As per a [discussion with Reddit user "raiph"](https://www.reddit.com/r/ProgrammingLanguages/comments/fnka0s/how_to_parse_setbuilder_notation/), it would be possible to create a parser which handles hairy cases such as
* |x|y||
* ||x|y|
* {x | |x| > 3}

and recognize ambiguities where they exist, such as the `|a|b|c|` example above. One approach might be to separate the expression into a list of statements delineated by `|` (including null statements), then analyze the possible meanings.
