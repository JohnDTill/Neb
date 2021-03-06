# ID (Identifier)

The scanner has two modes to define identifiers. There is a standard mode with identifiers like you would see in most programming language, but also a single-character mode which you would typically see in mathematical contexts. For the latter, source like `Cxy` would parse as an [implicit multiplication](./Grammar_ImplicitMultiplication) of three identifiers, whereas the standard mode would require spaces `C x y` to achieve the same result.
A short formalization of the ID grammar is given below:

### Multiple-character identifiers
```
ID ⇒ Letter (Letter | Digit)*
Letter ⇒ '_' | 'a-z' | 'A-Z' | 'α-ω' | 'Α-Ω'
Digit ⇒ '0'-'9'
```
### Single-character identifiers
```
ID ⇒ Letter
Letter ⇒ 'a-z' | 'A-Z' | 'α-ω' | 'Α-Ω'
```

One complication of the single-character identifier scheme is that multi-character functions must be explicitly recognized by the parser.
