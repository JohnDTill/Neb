# Ambiguities

Unfortunately, conventional mathematical syntax contains ambiguities which are dependent on some higher level understanding of the context.

#### Sequence vs. Grouping or Open Range

A sequence is typically written as a list of members enclosed in parenthesis, e.g. `(3,2,1)`. For 1 member sequences, this conflicts with the grouping rule `(3)`, and for 2 member sequences, this may conflict with the range rule `(2,3)`. Groupings and open ranges are more commonly encountered, but sequences are fundamental to programming tasks.

#### Horizontal Vector in Plain Text
In plain text we might imagine writing a horizontal vector in the style `x = ⁜⊞⏴1⏵⏴3⏵⏴a⏵⏴b⏵⏴c⏵` as `x = [a b c]`. Neb would parse this as an implicit multiplication in a bracket grouping. Neb leans on YAWYSIWYGEE syntax to avoid ambiguity with matrices, but one consequence is that simple ASCII vectors are unusable.

#### Implicit Multiplication by Absolute Value

Groupings with identical open and close symbols can introduce ambiguities. For example, consider the expression `|a|b|c|`. This could correctly be parsed as `abs(a*abs(b)*c)` or `abs(a)*b*abs(c)`, which are not mathematically equivalent. One solution is to disallow implicit multiplication for grouping with identical open and close symbols. Then one would have to write `|a*|b|*c|` or `|a|*b*|c|`, which have clear meanings.

As per a [discussion with Reddit user "raiph"](https://www.reddit.com/r/ProgrammingLanguages/comments/fnka0s/how_to_parse_setbuilder_notation/), it would be possible to create a parser which handles hairy cases such as
* |x|y||
* ||x|y|
* {x | |x| > 3}

and recognize ambiguities where they exist, such as the `|a|b|c|` example above. One approach might be to separate the expression into a list of statements delineated by `|` (including null statements), then analyze the possible meanings.

#### Language Level Interpretation
Some parsing decisions should be relegated to whatever language is using Neb's parse tree output. For instance, Neb can easily parse `⁜^⏴A⏵⏴⊤⏵` as a matrix transpose due to the unicode `⊤` symbol. `⁜^⏴A⏵⏴T⏵` with the letter 'T' is unclear since 'T' is a valid identifier, so this could be exponentiation, but using the letter 'T' to indicate a transpose is quite common. The language might want to always implement this as a transpose. Alternatively, the language could interpret this as a transpose if 'T' is not defined as a variable, an exponent if it is defined, and even issue a warning that raising to the power of 'T' is bad practice. The point is that some ambiguities are best handled at the language level.

Another example is identifier subscripts: Is `⁜_⏴r⏵⏴c⏵` a single unified identifier, or do we want to access element 'c' of vector 'r'? That kind of interpretation is best left to the language.
